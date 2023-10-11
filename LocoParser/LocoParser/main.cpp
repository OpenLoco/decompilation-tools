#include <algorithm>
#include <clang-c/Index.h>
#include <codecvt>
#include <cstring>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <vector>

#ifdef _MSC_VER
#    include <execution>
#endif

#include <filesystem>
namespace stdfs = std::filesystem;

static std::mutex sMtx;

static constexpr auto kParsingOptions = CXTranslationUnit_DetailedPreprocessingRecord | CXTranslationUnit_Incomplete
    | CXTranslationUnit_IncludeBriefCommentsInCodeCompletion | CXTranslationUnit_SkipFunctionBodies
    | CXTranslationUnit_KeepGoing;

static constexpr const char* kParseArgs[] = {
    "-fparse-all-comments", //
    "-std=c++20",           //
    "-xc++",                //
    "-m32",                 //
};

struct Options
{
    std::string inputFileOrFolder;
    std::string srcPath;
    std::string modules;
    bool showErrors{};
};

static std::string getString(CXString s)
{
    const auto* cstr = clang_getCString(s);
    if (cstr == nullptr)
        return {};

    std::string res(cstr);
    clang_disposeString(s);
    return res;
}

static std::vector<std::string> split(const std::string& s, const std::string& delim)
{
    const size_t searchLen = delim.length();

    size_t startIdx = 0;
    size_t endIdx = 0;
    std::string token;
    std::vector<std::string> res;
    while ((endIdx = s.find(delim, startIdx)) != std::string::npos)
    {
        token = s.substr(startIdx, endIdx - startIdx);
        startIdx = endIdx + searchLen;
        res.push_back(token);
    }
    res.push_back(s.substr(startIdx));
    return res;
}

static std::vector<std::string> tokenize(const std::string& s)
{
    std::vector<std::string> res;
    std::string cur;
    for (size_t i = 0; i < s.size(); i++)
    {
        auto chr = s[i];
        if (isalnum(chr))
        {
            cur += chr;
        }
        else
        {
            if (!cur.empty())
            {
                res.push_back(cur);
                cur.clear();
            }

            cur += chr;
            res.push_back(cur);
            cur.clear();
        }
    }
    if (!cur.empty())
    {
        res.push_back(cur);
    }
    return res;
}

static bool isCommentLine(const std::string_view line)
{
    if (line.find("* 0x") != line.npos || line.find("* 0X") != line.npos)
        return true;
    if (line.find("// 0x") != line.npos || line.find("// 0X") != line.npos)
        return true;
    return false;
}

static bool isValidHex(const std::string_view token)
{
    if (!token.starts_with("0x") && !token.starts_with("0X"))
        return false;
    const auto hex = token.substr(2);
    for (auto chr : hex)
    {
        if (!isxdigit(chr))
            return false;
    }
    return true;
}

static std::vector<std::string> extractAddresses(const std::string& txt)
{
    auto lines = split(txt, "\n");
    if (lines.empty())
        return {};

    // Find the line with the address
    std::string line;
    for (auto& l : lines)
    {
        if (isCommentLine(l))
        {
            line = l;
            break;
        }
    }
    if (line.empty())
        return {};

    std::vector<std::string> res;
    auto tokens = tokenize(line);
    for (auto& token : tokens)
    {
        if (isValidHex(token))
        {
            res.push_back(token);
        }
    }

    return res;
}

static std::vector<stdfs::path> findFiles(const std::string_view path)
{
    std::vector<stdfs::path> res;

    using iter = stdfs::recursive_directory_iterator;
    for (auto& entry : iter(stdfs::path(path)))
    {
        if (!entry.is_regular_file() || entry.is_symlink())
            continue;

        stdfs::path filePath = entry;
        auto ext = filePath.extension();
        if (ext != ".cpp")
            continue;

        res.push_back(filePath);
    }

    return res;
}

struct FunctionDef
{
    std::string address;
    std::string name;
};

struct VarDef
{
    std::string address;
    std::string name;
};

struct ParserContext
{
    std::vector<FunctionDef> funcs;
    std::vector<VarDef> vars;
};

static void checkFuncDecl(ParserContext& ctx, CXCursor c)
{
    auto strMangled = getString(clang_Cursor_getMangling(c));
    auto strComment = getString(clang_Cursor_getRawCommentText(c));
    if (strComment.empty())
        return;

    auto addrs = extractAddresses(strComment);
    if (addrs.empty())
        return;

    for (size_t i = 0; i < addrs.size(); i++)
    {
        auto name = strMangled;
        if (i > 0)
            name += "_" + std::to_string(i);

        {
            std::lock_guard lock(sMtx);
            ctx.funcs.push_back(FunctionDef{ addrs[i], name });
        }
    }
}

static void checkVarDecl(ParserContext& ctx, CXCursor c, CXCursor parent)
{
    auto spelling = getString(clang_getCursorSpelling(c));
    if (spelling != "loco_global")
        return;

    auto varName = getString(clang_getCursorSpelling(parent));

    auto cursorType = clang_getCursorType(parent);
    auto canonicalType = clang_getCanonicalType(cursorType);
    auto typeStr = getString(clang_getTypeSpelling(canonicalType));

    auto n1 = typeStr.find_first_of(',') + 1;
    while (typeStr[n1] == ' ')
        n1++;
    auto n2 = typeStr.find_first_of('>', n1);
    auto addrVal = typeStr.substr(n1, (n2 - n1));
    auto addr = atol(addrVal.c_str());

    {
        char addrString[64]{};
        sprintf(addrString, "0x%08X", static_cast<uint32_t>(addr));

        std::lock_guard lock(sMtx);
        ctx.vars.push_back({ addrString, varName });
    }
}

// 100% horrible.
static std::string toMb(const auto* str)
{
    std::string res;
    while (*str != 0)
    {
        res += static_cast<char>(*str);
        str++;
    }
    return res;
}

static std::string normalizePathSeparator(std::string path)
{
    for (auto& chr : path)
    {
        if (chr == '\\' || chr == '/')
        {
#ifdef _WIN32
            chr = '\\';
#else
            chr = '/';
#endif
        }
    }
    return path;
}

static std::string appendPath(const std::string lhs, const std::string& rhs)
{
    if (lhs.empty())
        return rhs;
    if (rhs.empty())
        return lhs;

    if (lhs.back() == '/' || lhs.back() == '\\')
        return lhs + rhs;
    return lhs + "/" + rhs;
}

static std::vector<std::string> buildIncludePaths(const std::string& rootPath, const std::string& modules)
{
    std::vector<std::string> res;

    const auto modulesSplit = split(modules, ";");
    for (const auto& mod : modulesSplit)
    {
        if (mod.empty())
            continue;

        // src
        {
            const auto modPath = mod + "/src";
            const auto includePath = normalizePathSeparator(appendPath(rootPath, modPath));
            res.push_back("-I" + includePath);
        }

        // include
        {
            const auto modPath = mod + "/include";
            const auto includePath = normalizePathSeparator(appendPath(rootPath, modPath));
            res.push_back("-I" + includePath);
        }

        // include/OpenLoco/<mod>
        {
            const auto modPath = mod + "/include/OpenLoco/" + mod;
            const auto includePath = normalizePathSeparator(appendPath(rootPath, modPath));
            res.push_back("-I" + includePath);
        }
    }

    return res;
}

static auto buildArgs(const std::string& rootPath, const std::string& modules)
{
    auto includePathsArgs = buildIncludePaths(rootPath, modules);
    auto numArgs = (int)(std::size(kParseArgs) + includePathsArgs.size());
    auto args = std::make_unique<const char*[]>(numArgs);
    for (size_t i = 0; i < std::size(kParseArgs); i++)
    {
        args[i] = kParseArgs[i];
    }
    for (size_t i = 0; i < includePathsArgs.size(); i++)
    {
        // This is a leak but don't care, shitty clang API.
        char* arg = new char[includePathsArgs[i].size() + 1];
        strcpy(arg, includePathsArgs[i].c_str());

        args[i + std::size(kParseArgs)] = arg;
    }
    return std::make_pair(std::move(args), (int)numArgs);
}

static bool parseFiles(ParserContext& ctx, const std::vector<stdfs::path>& files, const Options& opts)
{
    auto [args, numArgs] = buildArgs(opts.srcPath, opts.modules);

    CXIndex index = clang_createIndex(0, 0);

    std::vector<size_t> indices(files.size());
    std::iota(indices.begin(), indices.end(), 0);

#ifdef _MSC_VER
    std::for_each(std::execution::par, indices.begin(), indices.end(), [&](auto i) {
#else
    std::for_each(indices.begin(), indices.end(), [&](auto i) {
#endif
        const auto& path = files[i];

        std::string filePath = toMb(path.c_str());

        {
            std::lock_guard lock(sMtx);
            std::cout << "[" << (i + 1) << "/" << files.size() << "] Parsing '" << filePath << "'... \n";
        }

        CXTranslationUnit unit = clang_parseTranslationUnit(
            index, filePath.c_str(), args.get(), numArgs, nullptr, 0, kParsingOptions);

        if (unit == nullptr)
        {
            std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
            return;
        }

        CXCursor cursor = clang_getTranslationUnitCursor(unit);
        clang_visitChildren(
            cursor,
            [](CXCursor c, CXCursor parent, CXClientData client_data) {
                auto& ctx = *reinterpret_cast<ParserContext*>(client_data);
                auto kind = clang_getCursorKind(c);
                if (kind == CXCursor_FunctionDecl || kind == CXCursor_CXXMethod)
                {
                    checkFuncDecl(ctx, c);
                }
                else if (kind == CXCursor_TemplateRef && clang_getCursorKind(parent) == CXCursor_VarDecl)
                {
                    checkVarDecl(ctx, c, parent);
                }
                else if (kind == CXToken_Comment)
                {
                }
                return CXChildVisit_Recurse;
            },
            &ctx);

        if (opts.showErrors)
        {
            const auto numDiagnostics = clang_getNumDiagnostics(unit);
            for (unsigned i = 0; i < numDiagnostics; ++i)
            {
                CXDiagnostic diag = clang_getDiagnostic(unit, i);
                CXString string = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
                std::cout << clang_getCString(string) << std::endl;
                clang_disposeString(string);
            }
        }

        clang_disposeTranslationUnit(unit);
    });

    std::cout << "Sorting data...\n";

    std::sort(ctx.funcs.begin(), ctx.funcs.end(), [](auto& a, auto& b) { return a.address < b.address; });
    ctx.funcs.erase(
        std::unique(ctx.funcs.begin(), ctx.funcs.end(), [](auto& a, auto& b) { return a.address == b.address; }),
        ctx.funcs.end());

    std::sort(ctx.vars.begin(), ctx.vars.end(), [](auto& a, auto& b) { return a.address < b.address; });
    ctx.vars.erase(
        std::unique(ctx.vars.begin(), ctx.vars.end(), [](auto& a, auto& b) { return a.address == b.address; }), ctx.vars.end());

    std::cout << "Cleanup...\n";

    clang_disposeIndex(index);

    return true;
}

static std::string getStorageClassSpelling(CX_StorageClass cls)
{
    switch (cls)
    {
        case CX_SC_Invalid:
            return "";
        case CX_SC_None:
            return "";
        case CX_SC_Extern:
            return "extern";
        case CX_SC_Static:
            return "static";
        case CX_SC_PrivateExtern:
            return "private extern";
        case CX_SC_OpenCLWorkGroupLocal:
            return "OpenCL work group local";
        case CX_SC_Auto:
            return "auto";
        case CX_SC_Register:
            return "register";
    }
    return "";
}

static void dumpCursor(CXCursor c, CXCursor parent)
{
    auto parentSpelling = getString(clang_getCursorSpelling(parent));

    auto spelling = getString(clang_getCursorSpelling(c));
    auto cursorKind = clang_getCursorKind(c);
    auto kindSpelling = getString(clang_getCursorKindSpelling(cursorKind));

    auto storageClass = CX_SC_Invalid;
    if (cursorKind == CXCursor_VarDecl || cursorKind == CXCursor_FunctionDecl || cursorKind == CXCursor_CXXMethod)
    {
        storageClass = clang_Cursor_getStorageClass(c);
    }
    auto cursorType = clang_getCursorType(parent);
    auto canonicalType = clang_getCanonicalType(cursorType);
    auto typeStr = getString(clang_getTypeSpelling(cursorType));
    auto strComment = getString(clang_Cursor_getRawCommentText(c));
    auto strBriefComment = getString(clang_Cursor_getBriefCommentText(c));
    std::cout << "NODE: " << spelling << "\n";
    std::cout << "-> Parent: " << parentSpelling << "\n";
    std::cout << "-> Storage: " << getStorageClassSpelling(storageClass) << "\n";
    std::cout << "-> Kind: " << kindSpelling << "\n";
    std::cout << "-> Type: " << typeStr << "\n";
    std::cout << "-> Comment: " << strComment << "\n";
    std::cout << "-> Brief: " << strBriefComment << "\n";
}

static bool dumpAST(const Options& opts)
{
    auto [args, numArgs] = buildArgs(opts.srcPath, opts.modules);

    CXIndex index = clang_createIndex(0, 0);

    CXTranslationUnit unit = clang_parseTranslationUnit(
        index, opts.inputFileOrFolder.c_str(), args.get(), numArgs, nullptr, 0, kParsingOptions);

    if (unit == nullptr)
    {
        std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        return false;
    }

    if (opts.showErrors)
    {
        const auto numDiagnostics = clang_getNumDiagnostics(unit);
        for (unsigned i = 0; i < numDiagnostics; ++i)
        {
            CXDiagnostic diag = clang_getDiagnostic(unit, i);
            CXString string = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
            std::cout << clang_getCString(string) << std::endl;
            clang_disposeString(string);
        }
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(
        cursor,
        [](CXCursor c, CXCursor parent, CXClientData client_data) {
            dumpCursor(c, parent);
            return CXChildVisit_Recurse;
        },
        nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);

    return true;
}

static bool dumpIdc(ParserContext& ctx)
{
    std::cout << "Dumping name.idc... ";
    std::flush(std::cout);

    std::ofstream nameFile("names.idc");
    if (!nameFile.is_open())
    {
        std::cerr << "Unable to open names.idc\n";
        return false;
    }

    nameFile << "#include <idc.idc>\n";
    nameFile << "\n";
    nameFile << "static main(void)\n";
    nameFile << "{\n";
    nameFile << "    // Functions\n";
    for (auto& entry : ctx.funcs)
    {
        nameFile << "    set_name(" << entry.address << ", \"" << entry.name << "\", SN_FORCE);\n";
    }

    nameFile << "    // Globals\n";
    for (auto& entry : ctx.vars)
    {
        nameFile << "    set_name(" << entry.address << ", \"" << entry.name << "\", SN_FORCE);\n";
    }
    nameFile << "}\n";

    std::cout << "OK\n";
    return true;
}

int main(int argc, const char* argv[])
{
    enum class Action
    {
        Invalid = 0,
        MakeIDC,
        DumpAST,
    } action{};

    Options options;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
            action = Action::DumpAST;
        else if (strcmp(argv[i], "-p") == 0)
            action = Action::MakeIDC;
        else if (strcmp(argv[i], "-f") == 0)
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Missing argument for -f (file)\n";
                return EXIT_FAILURE;
            }
            options.inputFileOrFolder = argv[++i];
        }
        else if (strcmp(argv[i], "-r") == 0)
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Missing argument for -r (root path)\n";
                return EXIT_FAILURE;
            }
            options.srcPath = argv[++i];
        }
        else if (strcmp(argv[i], "-m") == 0)
        {
            if (i + 1 >= argc)
            {
                std::cerr << "Missing argument for -m (modules)\n";
                return EXIT_FAILURE;
            }
            options.modules = argv[++i];
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
            options.showErrors = true;
        }
    }

    if (action == Action::Invalid)
    {
        std::cerr << "No action specified (-d, -p)\n";
        return EXIT_FAILURE;
    }

    if (options.srcPath.empty())
    {
        std::cerr << "Missing argument for -r (root path)\n";
        return EXIT_FAILURE;
    }

    if (action == Action::MakeIDC)
    {
        auto files = findFiles(options.srcPath);
        if (files.empty())
        {
            std::cerr << "No files found to process\n";
            return EXIT_FAILURE;
        }

        ParserContext ctx;
        if (!parseFiles(ctx, files, options))
        {
            return EXIT_FAILURE;
        }

        if (!dumpIdc(ctx))
        {
            return EXIT_FAILURE;
        }
    }
    else if (action == Action::DumpAST)
    {
        if (options.inputFileOrFolder.empty())
        {
            std::cerr << "Missing argument for -f (file)\n";
            return EXIT_FAILURE;
        }

        if (!dumpAST(options))
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}