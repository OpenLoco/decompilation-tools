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

static constexpr auto kClangParseOptions = CXTranslationUnit_IncludeBriefCommentsInCodeCompletion | CXTranslationUnit_KeepGoing
    | CXTranslationUnit_Incomplete;

static constexpr const char* kClangOptions[] = {
    //"-std=c++17",
    "-fparse-all-comments",
    "-ferror-limit=0",
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

static bool isHex(const std::string_view token)
{
    if (token.size() < 2)
        return false;

    if (token[0] != '0' || (token[1] != 'x' && token[1] != 'X'))
        return false;

    for (size_t i = 2; i < std::min<size_t>(token.size(), 10u); i++)
    {
        if (!isxdigit(token[i]))
            return false;
    }
    
    return true;
}

static std::vector<std::string> extractAddresses(const std::string& txt)
{
    auto lines = split(txt, "\n");
    if (lines.empty())
        return {};

    auto line = lines[0];
    while (!line.empty() && (line.front() == '/' || line.front() == ' '))
    {
        line = line.substr(1);
    }
    if (line.empty())
        return {};

    std::vector<std::string> res;
    auto tokens = tokenize(line);
    for (auto& token : tokens)
    {
        if (isHex(token))
        {
            if (token.size() > 10)
            {
                // Because someone left a comment like this:
                // -> // 0x004BC701activatedWidgets
                token.resize(10);
            }
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
    std::string type;
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

    auto commaPos = typeStr.find_last_of(',');
    auto gtPos = typeStr.find_first_of('>', commaPos);
    auto ltPos = typeStr.find_first_of('<');

    auto innerTypeVal = typeStr.substr(ltPos + 1, commaPos - ltPos - 1);

    auto skip = 1;
    while (typeStr[commaPos + skip] == ' ')
    {
        skip++;
    }
    auto addrVal = typeStr.substr(commaPos + skip, (gtPos - commaPos - skip));
    auto addr = atol(addrVal.c_str());

    {
        char addrString[64]{};
        sprintf(addrString, "0x%08X", static_cast<uint32_t>(addr));

        std::lock_guard lock(sMtx);
        ctx.vars.push_back({ addrString, varName, innerTypeVal });
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

static bool parseFiles(ParserContext& ctx, const std::vector<stdfs::path>& files)
{
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

        CXTranslationUnit unit = nullptr;
        auto err = clang_parseTranslationUnit2(
            index, filePath.c_str(), kClangOptions, (int)std::size(kClangOptions), nullptr, 0, kClangParseOptions, &unit);

        if (err != CXError_Success || unit == nullptr)
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
                return CXChildVisit_Recurse;
            },
            &ctx);

        clang_disposeTranslationUnit(unit);
    });

    std::sort(ctx.funcs.begin(), ctx.funcs.end(), [](auto& a, auto& b) { return a.address < b.address; });
    ctx.funcs.erase(
        std::unique(ctx.funcs.begin(), ctx.funcs.end(), [](auto& a, auto& b) { return a.address == b.address; }),
        ctx.funcs.end());

    std::sort(ctx.vars.begin(), ctx.vars.end(), [](auto& a, auto& b) { return a.address < b.address; });
    ctx.vars.erase(
        std::unique(ctx.vars.begin(), ctx.vars.end(), [](auto& a, auto& b) { return a.address == b.address; }), ctx.vars.end());

    clang_disposeIndex(index);

    return true;
}

static void dumpCursor(CXCursor c, CXCursor parent)
{
    auto parentSpelling = getString(clang_getCursorSpelling(parent));
    auto displayName = getString(clang_getCursorDisplayName(c));
    auto spelling = getString(clang_getCursorSpelling(c));
    auto cursorKind = clang_getCursorKind(c);
    auto kindSpelling = getString(clang_getCursorKindSpelling(cursorKind));
    auto prettyString = getString(clang_getCursorPrettyPrinted(c, nullptr));

    auto cursorType = clang_getCursorType(parent);
    auto canonicalType = clang_getCanonicalType(cursorType);
    auto typeStr = getString(clang_getTypeSpelling(canonicalType));
    auto strComment = getString(clang_Cursor_getRawCommentText(c));

    std::cout << "NODE: " << displayName << "\n";
    std::cout << "-> Parent: " << parentSpelling << "\n";
    std::cout << "-> Kind: " << kindSpelling << "\n";
    // std::cout << "-> Pretty: " << prettyString << "\n";
    std::cout << "-> Comment: " << strComment << "\n";
}

static bool dumpAST(const std::string& filePath)
{
    CXIndex index = clang_createIndex(1, 1);

    CXTranslationUnit unit = nullptr;
    auto err = clang_parseTranslationUnit2(
        index, filePath.c_str(), kClangOptions, (int)std::size(kClangOptions), nullptr, 0, kClangParseOptions, &unit);

    if (err != CXError_Success || unit == nullptr)
    {
        std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        return false;
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

static bool dumpIdcNames(ParserContext& ctx)
{
    std::cout << "Dumping name.idc... ";

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

static bool dumpIdcTypes(ParserContext& ctx)
{
    std::cout << "Dumping types.idc... ";

    std::ofstream nameFile("types.idc");
    if (!nameFile.is_open())
    {
        std::cerr << "Unable to open names.idc\n";
        return false;
    }

    nameFile << "#include <idc.idc>\n";
    nameFile << "\n";
    nameFile << "static main(void)\n";
    nameFile << "{\n";
    nameFile << "    // Types\n";
    for (auto& entry : ctx.vars)
    {
        nameFile << "    SetType(" << entry.address << ", \"" << entry.type << "\");\n";
    }
    nameFile << "}\n";

    std::cout << "OK\n";
    return true;
}

static bool dumpIdc(ParserContext& ctx)
{
    if (!dumpIdcNames(ctx))
        return false;
    if (!dumpIdcTypes(ctx))
        return false;
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

    std::string inputFileOrFolder;

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
                std::cerr << "Missing argument for -f\n";
                return EXIT_FAILURE;
            }
            inputFileOrFolder = argv[++i];
        }
    }

    if (action == Action::Invalid)
    {
        std::cerr << "No action specified (-d, -p)\n";
        return EXIT_FAILURE;
    }

    if (inputFileOrFolder.empty())
    {
        std::cerr << "Missing argument for -f\n";
        return EXIT_FAILURE;
    }

    if (action == Action::MakeIDC)
    {
        auto files = findFiles(inputFileOrFolder);
        if (files.empty())
        {
            std::cerr << "No files found to process\n";
            return EXIT_FAILURE;
        }

        ParserContext ctx;
        if (!parseFiles(ctx, files))
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
        if (!dumpAST(inputFileOrFolder))
        {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}