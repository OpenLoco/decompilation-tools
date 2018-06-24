#include "common.idc"
#include <idc.idc>

static loco_getType(type)
{
    if (value_is_string(type))
        return type;

    if (type == U8 || type == S8)
        return "u8";
    if (type == U16 || type == S16)
        return "u16";
    if (type == U32 || type == S32)
        return "u32";
    if (type == U64 || type == S64)
        return "u64";
}

static loco_makeArrayMaybe(offset, count)
{
    if (count == 1)
        return;

    make_array(offset, count);
}

static loco_setType(offset, type, count)
{
    if (type == "u8" || type == "s8") {
        MakeUnknown(offset, count * 1, 1 | 2);
        create_byte(offset);
        loco_makeArrayMaybe(offset, count);
        return;
    } else if (type == "u16" || type == "s16") {
        MakeUnknown(offset, count * 2, 1 | 2);
        create_word(offset);
        loco_makeArrayMaybe(offset, count);
        return;
    } else if (type == "u32" || type == "s32") {
        MakeUnknown(offset, count * 4, 1 | 2);
        create_dword(offset);
        loco_makeArrayMaybe(offset, count);
        return;
    } else if (type == "void*") {
        create_dword(offset);
        op_plain_offset(offset, -1, 0);
        loco_makeArrayMaybe(offset, count);
        return;
    }

    auto id;
    id = get_struc_id(type);
    if (id == -1) {
        warning("Error: struct ‘%s’ was not found\n", type);
    }

    auto size;
    size = GetStrucSize(id);

    MakeUnknown(offset, count * size, 1 | 2);
    create_struct(offset, -1, type);
    loco_makeArrayMaybe(offset, count);
    return;
}

static loco_setVariable(offset, format, name)
{
    if (format != "") {
        auto type, count;

        type = loco_getType(format);
        count = 1;
        if (type[-1] == "]") {
            auto pos;
            pos = strstr(type, "[");
            count = 0 + substr(type, pos + 1, -2);
            type = substr(type, 0, pos);
        }

        loco_setType(offset, type, count);
    }

    if (name != "") {
        MakeName(offset, name);
    }
}

static initVariables()
{
    loco_setVariable(0x004FD120, "void*[4]", "AddToPlotList1");
    loco_setVariable(0x004FD130, "void*[4]", "AddToPlotList2");
    loco_setVariable(0x004FD140, "void*[4]", "AddToPlotList3");
    loco_setVariable(0x004FD150, "void*[4]", "AddToPlotList4");
    loco_setVariable(0x00523360, U16, "textinput::_callingWidget");
    loco_setVariable(0x00523362, U16, "textinput::_windowNumber");
    loco_setVariable(0x00523364, "u8", "textinput::_windowType");
    loco_setVariable(0x0113653C, "u8[16]", "formatargs");
    loco_setVariable(0x011365A0, U16, "textinput::_title");
    loco_setVariable(0x011366A0, U16, "textinput::_message");
    loco_setVariable(0x011366A0, U16, "textinput::_message");
    loco_setVariable(0x011369A0, "u8[512]", "textinput_buffer");
    loco_setVariable(0x01136D90, "u8[512]", "");
    loco_setVariable(0x01136FA4, S16, "txtutils::word_1136FA4");
}
