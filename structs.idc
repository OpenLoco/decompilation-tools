#include "common.idc"
#include <idc.idc>

static initStructs(void)
{
    loco_initTile();
    loco_initWindow();
    loco_initViewport();
    loco_initCompany();
    loco_initTown();
    loco_initIndustries();
    loco_initStation();
    loco_initEntities();
    loco_initG1Elements();
    loco_initMapAnimations();
    loco_initUserStrings();
    loco_initDropdowns();
    loco_initGFX();
    loco_initColours();
    loco_initMessage();
    loco_initPaintStructs();
}

static loco_setStructMember(id, offset, name, type, size)
{
    DelStrucMember(id, offset);
    AddStrucMember(id, name, offset, type, 0, size);
}

static loco_makeStructArray(offset, type, count, name)
{
    auto size;
    size = GetStrucSize(GetStrucIdByName(type));

    MakeUnknown(offset, count * size, 1 | 2);
    MakeStruct(offset, type);
    MakeName(offset, name);
    if (count > 1)
        MakeArray(offset, count);
    SetColor(offset, CIC_ITEM, 0xFFFFFF);
}

static loco_makeStruct(offset, type, name)
{
    loco_makeStructArray(offset, type, 1, name);
}

static loco_makeStubStruct(name, size)
{
    auto id;

    id = GetStrucIdByName(name);
    if (id == -1) {
        id = AddStruc(-1, name);
    }

    loco_setStructMember(id, size - 1, "end", FF_BYTE | FF_DATA, 1);

    return id;
}

static loco_setStructFldArray(id, offset, type, name, count)
{
    if (type == U8 || type == S8) {
        loco_DelStrucMember(id, offset, 1 * count);
        loco_setStructMember(id, offset, name, FF_BYTE | FF_DATA, 1 * count);
    } else if (type == U16 || type == S16) {
        loco_DelStrucMember(id, offset, 2 * count);
        loco_setStructMember(id, offset, name, FF_WORD | FF_DATA, 2 * count);
    } else if (type == U32 || type == S32) {
        loco_DelStrucMember(id, offset, 4 * count);
        loco_setStructMember(id, offset, name, FF_DWRD | FF_DATA, 4 * count);
    } else if (type == U64 || type == S64) {
        loco_DelStrucMember(id, offset, 8 * count);
        loco_setStructMember(id, offset, name, FF_QWRD | FF_DATA, 8 * count);
    }
}

static loco_setStructFld(id, offset, type, name)
{
    loco_setStructFldArray(id, offset, type, name, 1);
}

static loco_setStructVar(id, offset, type)
{
    loco_setStructFld(id, offset, type, form("var_%03X", offset));
}

static loco_DelStrucMember(id, offset, size)
{
    auto i;

    for (i = 0; i < size; i++) {
        DelStrucMember(id, offset + i);
    }
}

static loco_setStructSub(id, offset, type, name, count)
{
    auto bytes;

    bytes = GetStrucSize(GetStrucIdByName(type)) * count;
    loco_DelStrucMember(id, offset, bytes);
    AddStrucMember(id, name, offset, FF_STRU | FF_DATA, GetStrucIdByName(type), bytes);
}

static loco_initWindow(void)
{
    auto id;

    id = loco_makeStubStruct("scroll_t", 0x12);
    loco_setStructFld(id, 0x00, U16, "flags");
    loco_setStructFld(id, 0x02, U16, "h_left");
    loco_setStructFld(id, 0x04, U16, "h_right");
    loco_setStructFld(id, 0x06, U16, "h_thumb_left");
    loco_setStructFld(id, 0x08, U16, "h_thumb_right");
    loco_setStructFld(id, 0x0A, U16, "v_top");
    loco_setStructFld(id, 0x0C, U16, "v_bottom");
    loco_setStructFld(id, 0x0E, U16, "v_thumb_top");
    loco_setStructFld(id, 0x10, U16, "v_thumb_bottom");

    id = loco_makeStubStruct("viewport_config_t", 0x6);
    loco_setStructFld(id, 0x00, U16, "viewport_target_sprite");
    loco_setStructFld(id, 0x02, U16, "saved_view_x");
    loco_setStructFld(id, 0x04, U16, "saved_view_y");

    id = loco_makeStubStruct("SavedView_t", 0x8);
    loco_setStructFld(id, 0x00, U16, "mapX");
    loco_setStructFld(id, 0x02, U16, "mapY");
    loco_setStructFld(id, 0x04, U8, "zoomLevel");
    loco_setStructFld(id, 0x05, U8, "rotation");
    loco_setStructFld(id, 0x06, U16, "surfaceZ");

    id = loco_makeStubStruct("window_t", 0x88E);
    loco_setStructFld(id, 0x000, U32, "event_handlers");
    loco_setStructFldArray(id, 0x004, U32, "viewports", 2);
    loco_setStructFld(id, 0x00C, U64, "enabled_widgets");
    loco_setStructFld(id, 0x014, U64, "disabled_widgets");
    loco_setStructFld(id, 0x01C, U64, "activated_widgets");
    loco_setStructFld(id, 0x024, U64, "holdable_widgets");
    loco_setStructFld(id, 0x02C, U32, "widgets");
    loco_setStructFld(id, 0x030, U16, "x");
    loco_setStructFld(id, 0x032, U16, "y");
    loco_setStructFld(id, 0x034, U16, "width");
    loco_setStructFld(id, 0x036, U16, "height");
    loco_setStructFld(id, 0x038, U16, "min_width");
    loco_setStructFld(id, 0x03A, U16, "max_width");
    loco_setStructFld(id, 0x03C, U16, "min_height");
    loco_setStructFld(id, 0x03E, U16, "max_height");
    loco_setStructFld(id, 0x040, U16, "number");
    loco_setStructFld(id, 0x042, U32, "flags");
    loco_setStructSub(id, 0x046, "scroll_t", "scroll", 2);
    loco_setStructFldArray(id, 0x06A, U16, "row_info", 1000);
    loco_setStructFld(id, 0x83A, U16, "row_count");
    loco_setStructFld(id, 0x83E, U16, "row_height");
    loco_setStructFld(id, 0x840, U16, "row_hover");
    loco_setStructFld(id, 0x844, U16, "sort_mode");
    loco_setStructSub(id, 0x848, "SavedView_t", "saved_view", 1);
    loco_setStructFld(id, 0x870, U16, "current_tab");
    loco_setStructFld(id, 0x872, U16, "frame_no");
    loco_setStructFld(id, 0x874, U16, "current_secondary_tab");
    loco_setStructSub(id, 0x876, "viewport_config_t", "viewport_configurations", 2);
    loco_setStructFld(id, 0x882, U8, "type");
    loco_setStructFld(id, 0x884, U8, "owner");
    loco_setStructFldArray(id, 0x886, U8, "colours", 4);

    // sort on
    op_stroff(0x004290A7, 0, id, 0);
    op_stroff(0x004290AE, 0, id, 0);
    op_stroff(0x004290B2, 0, id, 0);
    op_stroff(0x004290B6, 0, id, 0);
    op_stroff(0x004290BF, 0, id, 0);
    op_stroff(0x00429150, 0, id, 0);
    op_stroff(0x00429157, 0, id, 0);
    op_stroff(0x0042915B, 0, id, 0);
    op_stroff(0x0042915F, 0, id, 0);
    op_stroff(0x00429168, 0, id, 0);
    op_stroff(0x004291D3, 0, id, 0);
    op_stroff(0x004291DA, 0, id, 0);
    op_stroff(0x004291EE, 0, id, 0);
    op_stroff(0x004291F4, 0, id, 0);
    op_stroff(0x00429F03, 0, id, 0);
    op_stroff(0x00429F0C, 0, id, 0);
    op_stroff(0x00429F1F, 0, id, 0);
    op_stroff(0x00429F2B, 0, id, 0);
    op_stroff(0x0042A53E, 0, id, 0);
    op_stroff(0x0042A744, 0, id, 0);
    op_stroff(0x0042A74B, 0, id, 0);
    op_stroff(0x0042A754, 0, id, 0);
    op_stroff(0x0042A75D, 1, id, 0);
    op_stroff(0x0042A770, 0, id, 0);
    op_stroff(0x0042A77A, 0, id, 0);
    op_stroff(0x0042A784, 0, id, 0);
    op_stroff(0x0042A78E, 0, id, 0);
    op_stroff(0x0042A790, 0, id, 0);
    op_stroff(0x0042A79E, 0, id, 0);
    op_stroff(0x0042A7AB, 1, id, 0);
    op_stroff(0x0042A7B9, 0, id, 0);
    op_stroff(0x0042A7BF, 0, id, 0);
    op_stroff(0x0042A7C5, 0, id, 0);
    op_stroff(0x0042A7CB, 0, id, 0);
    op_stroff(0x0042A7D1, 0, id, 0);
    op_stroff(0x0042A7D7, 0, id, 0);
    op_stroff(0x0042A7DD, 0, id, 0);
    op_stroff(0x0042A7E8, 0, id, 0);
    op_stroff(0x0042A7EE, 0, id, 0);
    op_stroff(0x0042A7F4, 0, id, 0);
    op_stroff(0x0042A7FA, 0, id, 0);
    op_stroff(0x0042A800, 0, id, 0);
    op_stroff(0x0042A806, 0, id, 0);
    op_stroff(0x00431E9B, 0, id, 0);
    op_stroff(0x00431EAA, 1, id, 0);
    op_stroff(0x00431EB6, 0, id, 0);
    op_stroff(0x00431EBB, 1, id, 0);
    op_stroff(0x00431EC9, 1, id, 0);
    op_stroff(0x00431ECE, 0, id, 0);
    op_stroff(0x00431ED6, 1, id, 0);
    op_stroff(0x00431EDF, 1, id, 0);
    op_stroff(0x00431EEC, 0, id, 0);
    op_stroff(0x00431EEF, 1, id, 0);
    op_stroff(0x00431F06, 1, id, 0);
    op_stroff(0x00431F0F, 0, id, 0);
    op_stroff(0x00431F1B, 1, id, 0);
    op_stroff(0x00431F38, 0, id, 0);
    op_stroff(0x00431F3B, 1, id, 0);
    op_stroff(0x00431F3F, 1, id, 0);
    op_stroff(0x00431F7D, 1, id, 0);
    op_stroff(0x00431FAC, 1, id, 0);
    op_stroff(0x00431FBA, 1, id, 0);
    op_stroff(0x00431FD2, 1, id, 0);
    op_stroff(0x00431FEA, 1, id, 0);
    op_stroff(0x00432009, 1, id, 0);
    op_stroff(0x00432060, 1, id, 0);
    op_stroff(0x004320A7, 1, id, 0);
    op_stroff(0x004320AB, 1, id, 0);
    op_stroff(0x004320B5, 1, id, 0);
    op_stroff(0x004320DD, 1, id, 0);
    op_stroff(0x004320E1, 1, id, 0);
    op_stroff(0x00432102, 1, id, 0);
    op_stroff(0x00432106, 1, id, 0);
    op_stroff(0x0043215A, 1, id, 0);
    op_stroff(0x0043215E, 1, id, 0);
    op_stroff(0x0043219D, 1, id, 0);
    op_stroff(0x004321A1, 1, id, 0);
    op_stroff(0x004321CA, 1, id, 0);
    op_stroff(0x004321F5, 1, id, 0);
    op_stroff(0x004321F9, 1, id, 0);
    op_stroff(0x0043222E, 0, id, 0);
    op_stroff(0x004322AF, 1, id, 0);
    op_stroff(0x004322E2, 0, id, 0);
    op_stroff(0x004322E6, 0, id, 0);
    op_stroff(0x00432315, 1, id, 0);
    op_stroff(0x00432323, 1, id, 0);
    op_stroff(0x00432336, 1, id, 0);
    op_stroff(0x0043233C, 1, id, 0);
    op_stroff(0x0043234A, 0, id, 0);
    op_stroff(0x00432351, 0, id, 0);
    op_stroff(0x0043235A, 0, id, 0);
    op_stroff(0x00432363, 1, id, 0);
    op_stroff(0x00432376, 0, id, 0);
    op_stroff(0x00432380, 0, id, 0);
    op_stroff(0x0043238A, 0, id, 0);
    op_stroff(0x00432394, 0, id, 0);
    op_stroff(0x00432396, 0, id, 0);
    op_stroff(0x004323A4, 0, id, 0);
    op_stroff(0x004323B1, 1, id, 0);
    op_stroff(0x004323E8, 0, id, 0);
    op_stroff(0x004323EE, 0, id, 0);
    op_stroff(0x004323F4, 0, id, 0);
    op_stroff(0x004323FA, 0, id, 0);
    op_stroff(0x00432400, 0, id, 0);
    op_stroff(0x00432406, 0, id, 0);
    op_stroff(0x00432411, 0, id, 0);
    op_stroff(0x00432417, 0, id, 0);
    op_stroff(0x0043241D, 0, id, 0);
    op_stroff(0x00432423, 0, id, 0);
    op_stroff(0x00432429, 0, id, 0);
    op_stroff(0x0043242F, 0, id, 0);
    op_stroff(0x00432437, 0, id, 0);
    op_stroff(0x0043243D, 0, id, 0);
    op_stroff(0x00432443, 0, id, 0);
    op_stroff(0x00432449, 0, id, 0);
    op_stroff(0x0043244F, 0, id, 0);
    op_stroff(0x00432455, 0, id, 0);
    op_stroff(0x0043245D, 0, id, 0);
    op_stroff(0x00432463, 0, id, 0);
    op_stroff(0x00432469, 0, id, 0);
    op_stroff(0x0043246F, 0, id, 0);
    op_stroff(0x00432475, 0, id, 0);
    op_stroff(0x0043247B, 0, id, 0);
    op_stroff(0x00432483, 0, id, 0);
    op_stroff(0x00432489, 0, id, 0);
    op_stroff(0x0043248F, 0, id, 0);
    op_stroff(0x00432495, 0, id, 0);
    op_stroff(0x0043249B, 0, id, 0);
    op_stroff(0x004324A1, 0, id, 0);
    op_stroff(0x004324A9, 1, id, 0);
    op_stroff(0x004324C2, 0, id, 0);
    op_stroff(0x00432538, 1, id, 0);
    op_stroff(0x00432561, 1, id, 0);
    op_stroff(0x004325F1, 1, id, 0);
    op_stroff(0x0043264F, 1, id, 0);
    op_stroff(0x004326AD, 1, id, 0);
    op_stroff(0x0043270A, 0, id, 0);
    op_stroff(0x00432711, 1, id, 0);
    op_stroff(0x0043271A, 1, id, 0);
    op_stroff(0x00432729, 0, id, 0);
    op_stroff(0x0043272F, 0, id, 0);
    op_stroff(0x00432735, 0, id, 0);
    op_stroff(0x0043273B, 0, id, 0);
    op_stroff(0x00432741, 1, id, 0);
    op_stroff(0x00432745, 1, id, 0);
    op_stroff(0x0043274B, 0, id, 0);
    op_stroff(0x00432754, 1, id, 0);
    op_stroff(0x00432758, 1, id, 0);
    op_stroff(0x00432763, 0, id, 0);
    op_stroff(0x00432767, 1, id, 0);
    op_stroff(0x0043276B, 1, id, 0);
    op_stroff(0x00432771, 0, id, 0);
    op_stroff(0x0043277A, 1, id, 0);
    op_stroff(0x0043277E, 1, id, 0);
    op_stroff(0x00432789, 0, id, 0);
    op_stroff(0x0043278D, 1, id, 0);
    op_stroff(0x00432794, 1, id, 0);
    op_stroff(0x00432798, 1, id, 0);
    op_stroff(0x004327CF, 1, id, 0);
    op_stroff(0x004327DD, 1, id, 0);
    op_stroff(0x004327E2, 0, id, 0);
    op_stroff(0x004327EA, 1, id, 0);
    op_stroff(0x004327F3, 1, id, 0);
    op_stroff(0x00432800, 0, id, 0);
    op_stroff(0x00432803, 1, id, 0);
    op_stroff(0x0043281A, 1, id, 0);
    op_stroff(0x0043283B, 1, id, 0);
    op_stroff(0x00432854, 0, id, 0);
    op_stroff(0x00432857, 1, id, 0);
    op_stroff(0x0043285B, 1, id, 0);
    op_stroff(0x004328AE, 1, id, 0);
    op_stroff(0x004328CD, 1, id, 0);
    op_stroff(0x00432924, 1, id, 0);
    op_stroff(0x0043296B, 1, id, 0);
    op_stroff(0x0043296F, 1, id, 0);
    op_stroff(0x00432979, 1, id, 0);
    op_stroff(0x00432983, 1, id, 0);
    op_stroff(0x00432987, 1, id, 0);
    op_stroff(0x00432A21, 1, id, 0);
    op_stroff(0x00432B59, 1, id, 0);
    op_stroff(0x00432B5D, 1, id, 0);
    op_stroff(0x00432BA0, 1, id, 0);
    op_stroff(0x00432BA4, 1, id, 0);
    op_stroff(0x00432BC7, 0, id, 0);
    op_stroff(0x00432D85, 0, id, 0);
    op_stroff(0x00432D8C, 1, id, 0);
    op_stroff(0x00432D95, 1, id, 0);
    op_stroff(0x00432DA4, 0, id, 0);
    op_stroff(0x00432DAA, 0, id, 0);
    op_stroff(0x00432DB0, 0, id, 0);
    op_stroff(0x00432DB6, 0, id, 0);
    op_stroff(0x00432DBC, 1, id, 0);
    op_stroff(0x00432DC0, 1, id, 0);
    op_stroff(0x00432DC6, 0, id, 0);
    op_stroff(0x00432DCF, 1, id, 0);
    op_stroff(0x00432DD3, 1, id, 0);
    op_stroff(0x00432DDE, 0, id, 0);
    op_stroff(0x00432DE2, 1, id, 0);
    op_stroff(0x00432DE6, 1, id, 0);
    op_stroff(0x00432DEC, 0, id, 0);
    op_stroff(0x00432DF5, 1, id, 0);
    op_stroff(0x00432DF9, 1, id, 0);
    op_stroff(0x00432E04, 0, id, 0);
    op_stroff(0x00432E0F, 1, id, 0);
    op_stroff(0x00432E1D, 1, id, 0);
    op_stroff(0x00432E22, 0, id, 0);
    op_stroff(0x00432E2A, 1, id, 0);
    op_stroff(0x00432E33, 1, id, 0);
    op_stroff(0x00432E40, 0, id, 0);
    op_stroff(0x00432E43, 1, id, 0);
    op_stroff(0x00432E5A, 1, id, 0);
    op_stroff(0x00432E5E, 1, id, 0);
    op_stroff(0x00432E9C, 1, id, 0);
    op_stroff(0x00432EB9, 1, id, 0);
    op_stroff(0x00432F02, 0, id, 0);
    op_stroff(0x00432F10, 0, id, 0);
    op_stroff(0x0043311A, 1, id, 0);
    op_stroff(0x0043425D, 0, id, 0);
    op_stroff(0x00434270, 1, id, 0);
    op_stroff(0x004342BE, 0, id, 0);
    op_stroff(0x004342C4, 1, id, 0);
    op_stroff(0x004342CC, 1, id, 0);
    op_stroff(0x004342D6, 1, id, 0);
    op_stroff(0x004342FD, 0, id, 0);
    op_stroff(0x00434303, 0, id, 0);
    op_stroff(0x0043430F, 1, id, 0);
    op_stroff(0x00434320, 0, id, 0);
    op_stroff(0x00434328, 1, id, 0);
    op_stroff(0x00434369, 1, id, 0);
    op_stroff(0x00434797, 0, id, 0);
    op_stroff(0x0043479F, 0, id, 0);
    op_stroff(0x004347A7, 0, id, 0);
    op_stroff(0x004347AF, 0, id, 0);
    op_stroff(0x004347B9, 0, id, 0);
    op_stroff(0x004347E5, 0, id, 0);
    op_stroff(0x004347F1, 0, id, 0);
    op_stroff(0x004347F9, 0, id, 0);
    op_stroff(0x004347FD, 0, id, 0);
    op_stroff(0x00434801, 0, id, 0);
    op_stroff(0x00434807, 0, id, 0);
    op_stroff(0x00434810, 0, id, 0);
    op_stroff(0x00434819, 0, id, 0);
    op_stroff(0x0043482C, 0, id, 0);
    op_stroff(0x00434832, 0, id, 0);
    op_stroff(0x00435008, 0, id, 0);
    op_stroff(0x00435024, 1, id, 0);
    op_stroff(0x00435D07, 1, id, 0);
    op_stroff(0x00435D15, 1, id, 0);
    op_stroff(0x00435D1A, 0, id, 0);
    op_stroff(0x00435D22, 1, id, 0);
    op_stroff(0x00435D2B, 1, id, 0);
    op_stroff(0x00435D38, 0, id, 0);
    op_stroff(0x00435D3B, 1, id, 0);
    op_stroff(0x00435D3F, 1, id, 0);
    op_stroff(0x00435D90, 1, id, 0);
    op_stroff(0x00435E45, 1, id, 0);
    op_stroff(0x00435EA8, 1, id, 0);
    op_stroff(0x00435ED2, 1, id, 0);
    op_stroff(0x00438AB6, 0, id, 0);
    op_stroff(0x00438AC2, 1, id, 0);
    op_stroff(0x00438AC5, 1, id, 0);
    op_stroff(0x00438B46, 0, id, 0);
    op_stroff(0x00438B4D, 0, id, 0);
    op_stroff(0x00438B51, 0, id, 0);
    op_stroff(0x00438B55, 0, id, 0);
    op_stroff(0x00438B59, 0, id, 0);
    op_stroff(0x00438B5D, 0, id, 0);
    op_stroff(0x00438B61, 0, id, 0);
    op_stroff(0x00438B68, 0, id, 0);
    op_stroff(0x00438B6F, 0, id, 0);
    op_stroff(0x00438B73, 0, id, 0);
    op_stroff(0x00438B7A, 0, id, 0);
    op_stroff(0x00438B7E, 0, id, 0);
    op_stroff(0x00438B85, 0, id, 0);
    op_stroff(0x00438B8C, 0, id, 0);
    op_stroff(0x00438BA6, 0, id, 0);
    op_stroff(0x00438BAF, 0, id, 0);
    op_stroff(0x00438BB8, 0, id, 0);
    op_stroff(0x00438BC1, 0, id, 0);
    op_stroff(0x00438BE8, 0, id, 0);
    op_stroff(0x00438BEF, 0, id, 0);
    op_stroff(0x00438BF3, 0, id, 0);
    op_stroff(0x00438BF7, 0, id, 0);
    op_stroff(0x00438BFB, 0, id, 0);
    op_stroff(0x00438C19, 0, id, 0);
    op_stroff(0x00438C1F, 0, id, 0);
    op_stroff(0x00438C50, 0, id, 0);
    op_stroff(0x00438C57, 0, id, 0);
    op_stroff(0x00438C5B, 0, id, 0);
    op_stroff(0x00438C5F, 0, id, 0);
    op_stroff(0x00438C63, 0, id, 0);
    op_stroff(0x00438C67, 0, id, 0);
    op_stroff(0x00438C6B, 0, id, 0);
    op_stroff(0x00438C72, 0, id, 0);
    op_stroff(0x00438C7B, 0, id, 0);
    op_stroff(0x00438C99, 0, id, 0);
    op_stroff(0x00438C9F, 0, id, 0);
    op_stroff(0x00438CDF, 0, id, 0);
    op_stroff(0x00438CFB, 0, id, 0);
    op_stroff(0x00438D06, 0, id, 0);
    op_stroff(0x00438D40, 0, id, 0);
    op_stroff(0x00438D47, 0, id, 0);
    op_stroff(0x00438D4B, 0, id, 0);
    op_stroff(0x00438D4F, 0, id, 0);
    op_stroff(0x00438D53, 0, id, 0);
    op_stroff(0x00438D57, 0, id, 0);
    op_stroff(0x00438D5B, 0, id, 0);
    op_stroff(0x00438D64, 0, id, 0);
    op_stroff(0x00438D6B, 0, id, 0);
    op_stroff(0x00438D72, 0, id, 0);
    op_stroff(0x00438DA6, 0, id, 0);
    op_stroff(0x00438DAD, 0, id, 0);
    op_stroff(0x00438DB6, 0, id, 0);
    op_stroff(0x00438DBD, 0, id, 0);
    op_stroff(0x00438DE3, 0, id, 0);
    op_stroff(0x00438DEA, 0, id, 0);
    op_stroff(0x00438DF3, 0, id, 0);
    op_stroff(0x00438DF7, 0, id, 0);
    op_stroff(0x00438DFE, 0, id, 0);
    op_stroff(0x00438E0D, 0, id, 0);
    op_stroff(0x00438EE3, 1, id, 0);
    op_stroff(0x00438EE7, 1, id, 0);
    op_stroff(0x00438F04, 1, id, 0);
    op_stroff(0x00438F41, 1, id, 0);
    op_stroff(0x00438F45, 1, id, 0);
    op_stroff(0x00438F62, 1, id, 0);
    op_stroff(0x0043903D, 1, id, 0);
    op_stroff(0x00439048, 1, id, 0);
    op_stroff(0x0043904C, 1, id, 0);
    op_stroff(0x0043912E, 1, id, 0);
    op_stroff(0x00439154, 1, id, 0);
    op_stroff(0x00439158, 1, id, 0);
    op_stroff(0x004391F6, 0, id, 0);
    op_stroff(0x00439242, 1, id, 0);
    op_stroff(0x0043924D, 1, id, 0);
    op_stroff(0x00439251, 1, id, 0);
    op_stroff(0x00439299, 1, id, 0);
    op_stroff(0x0043929D, 1, id, 0);
    op_stroff(0x004392E0, 1, id, 0);
    op_stroff(0x004392E9, 0, id, 0);
    op_stroff(0x004392ED, 0, id, 0);
    op_stroff(0x0043933B, 0, id, 0);
    op_stroff(0x00439352, 0, id, 0);
    op_stroff(0x00439369, 0, id, 0);
    op_stroff(0x00439380, 0, id, 0);
    op_stroff(0x0043939B, 0, id, 0);
    op_stroff(0x004393A6, 0, id, 0);
    op_stroff(0x004393BD, 0, id, 0);
    op_stroff(0x004393C8, 0, id, 0);
    op_stroff(0x0043940B, 1, id, 0);
    op_stroff(0x0043940F, 1, id, 0);
    op_stroff(0x00439413, 1, id, 0);
    op_stroff(0x00439417, 1, id, 0);
    op_stroff(0x00439670, 0, id, 0);
    op_stroff(0x00439677, 0, id, 0);
    op_stroff(0x00439681, 0, id, 0);
    op_stroff(0x0043B2B3, 0, id, 0);
    op_stroff(0x0043B2BA, 0, id, 0);
    op_stroff(0x0043B2BE, 0, id, 0);
    op_stroff(0x0043B2C2, 0, id, 0);
    op_stroff(0x0043B2D4, 0, id, 0);
    op_stroff(0x0043B2DD, 0, id, 0);
    op_stroff(0x0043C2D1, 0, id, 0);
    op_stroff(0x0043C2D8, 0, id, 0);
    op_stroff(0x0043C2DC, 0, id, 0);
    op_stroff(0x0043C2E0, 0, id, 0);
    op_stroff(0x0043C2E4, 0, id, 0);
    op_stroff(0x0043C2ED, 0, id, 0);
    op_stroff(0x0043C2F4, 0, id, 0);
    op_stroff(0x004421FB, 1, id, 0);
    op_stroff(0x004421FE, 0, id, 0);
    op_stroff(0x0044220D, 0, id, 0);
    op_stroff(0x0044221A, 0, id, 0);
    op_stroff(0x00443A50, 1, id, 0);
    op_stroff(0x00443D02, 1, id, 0);
    op_stroff(0x00443D3E, 1, id, 0);
    op_stroff(0x00443D87, 1, id, 0);
    op_stroff(0x00443D9D, 1, id, 0);
    op_stroff(0x00443EB7, 0, id, 0);
    op_stroff(0x00443EC9, 0, id, 0);
    op_stroff(0x00443F01, 1, id, 0);
    op_stroff(0x004464C3, 1, id, 0);
    op_stroff(0x004464D9, 1, id, 0);
    op_stroff(0x004464E6, 0, id, 0);
    op_stroff(0x00457F55, 0, id, 0);
    op_stroff(0x00457FA1, 0, id, 0);
    op_stroff(0x00457FBC, 1, id, 0);
    op_stroff(0x00458708, 0, id, 0);
    op_stroff(0x00458712, 0, id, 0);
    op_stroff(0x0045EFDB, 1, id, 0);
    op_stroff(0x0045F001, 0, id, 0);
    op_stroff(0x0045F008, 0, id, 0);
    op_stroff(0x0045F015, 1, id, 0);
    op_stroff(0x0045F03B, 0, id, 0);
    op_stroff(0x0045F042, 0, id, 0);
    op_stroff(0x0045F0C5, 0, id, 0);
    op_stroff(0x0045F0CC, 0, id, 0);
    op_stroff(0x0045F191, 1, id, 0);
    op_stroff(0x0045FCF0, 1, id, 0);
    op_stroff(0x0046B4FC, 0, id, 0);
    op_stroff(0x0046B503, 0, id, 0);
    op_stroff(0x0046B507, 0, id, 0);
    op_stroff(0x0046B50B, 0, id, 0);
    op_stroff(0x0046B50F, 0, id, 0);
    op_stroff(0x0046B513, 0, id, 0);
    op_stroff(0x0046B51A, 0, id, 0);
    op_stroff(0x0046B526, 0, id, 0);
    op_stroff(0x0046B53E, 0, id, 0);
    op_stroff(0x0046B54B, 0, id, 0);
    op_stroff(0x0046B55C, 0, id, 0);
    op_stroff(0x0046B8E6, 1, id, 0);
    op_stroff(0x0046B8F0, 1, id, 0);
    op_stroff(0x0046B8FA, 1, id, 0);
    op_stroff(0x0046B907, 1, id, 0);
    op_stroff(0x0046B914, 1, id, 0);
    op_stroff(0x0046E742, 0, id, 0);
    op_stroff(0x0046E74A, 0, id, 0);
    op_stroff(0x0046E74D, 0, id, 0);
    op_stroff(0x0046E753, 0, id, 0);
    op_stroff(0x0046E75C, 0, id, 0);
    op_stroff(0x0046E765, 0, id, 0);
    op_stroff(0x0046E76E, 0, id, 0);
    op_stroff(0x0046E775, 0, id, 0);
    op_stroff(0x0046EACE, 1, id, 0);
    op_stroff(0x0046EAD4, 1, id, 0);
    op_stroff(0x0046F116, 0, id, 0);
    op_stroff(0x0049196F, 0, id, 0);
    op_stroff(0x0049A4D0, 0, id, 0);
    op_stroff(0x004B3C1B, 0, id, 0);
    op_stroff(0x004B3F88, 1, id, 0);
    op_stroff(0x004B3FD0, 1, id, 0);
    op_stroff(0x004B45DD, 0, id, 0);
    op_stroff(0x004B55A7, 0, id, 0);
    op_stroff(0x004B92B9, 0, id, 0);
    op_stroff(0x004B92C2, 0, id, 0);
    op_stroff(0x004B93B3, 1, id, 0);
    op_stroff(0x004B93BD, 1, id, 0);
    op_stroff(0x004B93C3, 0, id, 0);
    op_stroff(0x004BB4DE, 0, id, 0);
    op_stroff(0x004BB4E6, 0, id, 0);
    op_stroff(0x004BB4EE, 0, id, 0);
    op_stroff(0x004BB4F6, 0, id, 0);
    op_stroff(0x004BB4FE, 0, id, 0);
    op_stroff(0x004BB500, 0, id, 0);
    op_stroff(0x004BB51B, 0, id, 0);
    op_stroff(0x004BB524, 0, id, 0);
    op_stroff(0x004BBEDF, 0, id, 0);
    op_stroff(0x004BBEE9, 0, id, 0);
    op_stroff(0x004BC377, 0, id, 0);
    op_stroff(0x004BC381, 0, id, 0);
    op_stroff(0x004BF15C, 0, id, 0);
    op_stroff(0x004BF16B, 0, id, 0);
    op_stroff(0x004BF176, 1, id, 0);
    op_stroff(0x004BF17E, 1, id, 0);
    op_stroff(0x004BF7CB, 0, id, 0);
    op_stroff(0x004BF7D1, 0, id, 0);
    op_stroff(0x004BF7D7, 0, id, 0);
    op_stroff(0x004BF7EA, 0, id, 0);
    op_stroff(0x004BF7F2, 0, id, 0);
    op_stroff(0x004BF7FA, 0, id, 0);
    op_stroff(0x004BF802, 0, id, 0);
    op_stroff(0x004BF80A, 0, id, 0);
    op_stroff(0x004BF80C, 0, id, 0);
    op_stroff(0x004BF813, 1, id, 0);
    op_stroff(0x004BF818, 1, id, 0);
    op_stroff(0x004BF885, 0, id, 0);
    op_stroff(0x004BF88B, 0, id, 0);
    op_stroff(0x004BF894, 0, id, 0);
    op_stroff(0x004BF89D, 0, id, 0);
    op_stroff(0x004BF8AF, 0, id, 0);
    op_stroff(0x004BF8B8, 0, id, 0);
    op_stroff(0x004BFCC7, 0, id, 0);
    op_stroff(0x004BFCCD, 0, id, 0);
    op_stroff(0x004BFCD3, 0, id, 0);
    op_stroff(0x004BFCD9, 0, id, 0);
    op_stroff(0x004BFCDF, 0, id, 0);
    op_stroff(0x004BFCE5, 0, id, 0);
    op_stroff(0x004BFCEB, 0, id, 0);
    op_stroff(0x004BFCF9, 0, id, 0);
    op_stroff(0x004BFCFF, 0, id, 0);
    op_stroff(0x004BFD05, 0, id, 0);
    op_stroff(0x004BFD0B, 0, id, 0);
    op_stroff(0x004BFD11, 0, id, 0);
    op_stroff(0x004BFD17, 0, id, 0);
    op_stroff(0x004BFD1D, 0, id, 0);
    op_stroff(0x004BFD2B, 0, id, 0);
    op_stroff(0x004BFD31, 0, id, 0);
    op_stroff(0x004BFD37, 0, id, 0);
    op_stroff(0x004BFD3D, 0, id, 0);
    op_stroff(0x004BFD43, 0, id, 0);
    op_stroff(0x004BFD49, 0, id, 0);
    op_stroff(0x004BFD4F, 0, id, 0);
    op_stroff(0x004BFD5A, 0, id, 0);
    op_stroff(0x004BFD60, 0, id, 0);
    op_stroff(0x004BFD66, 0, id, 0);
    op_stroff(0x004BFD6C, 0, id, 0);
    op_stroff(0x004BFD72, 0, id, 0);
    op_stroff(0x004BFD78, 0, id, 0);
    op_stroff(0x004BFD80, 0, id, 0);
    op_stroff(0x004BFD86, 0, id, 0);
    op_stroff(0x004BFD8C, 0, id, 0);
    op_stroff(0x004BFD92, 0, id, 0);
    op_stroff(0x004BFD98, 0, id, 0);
    op_stroff(0x004BFD9E, 0, id, 0);
    op_stroff(0x004BFDA6, 0, id, 0);
    op_stroff(0x004BFDAC, 0, id, 0);
    op_stroff(0x004BFDB2, 0, id, 0);
    op_stroff(0x004BFDB8, 0, id, 0);
    op_stroff(0x004BFDBE, 0, id, 0);
    op_stroff(0x004BFDC4, 0, id, 0);
    op_stroff(0x004C1A0C, 0, id, 0);
    op_stroff(0x004C1A1B, 0, id, 0);
    op_stroff(0x004C1A22, 0, id, 0);
    op_stroff(0x004C1A28, 0, id, 0);
    op_stroff(0x004C1A2E, 0, id, 0);
    op_stroff(0x004C1A37, 0, id, 0);
    op_stroff(0x004C1A40, 0, id, 0);
    op_stroff(0x004C1A5C, 0, id, 0);
    op_stroff(0x004C1A66, 0, id, 0);
    op_stroff(0x004C1A70, 0, id, 0);
    op_stroff(0x004C1A7A, 0, id, 0);
    op_stroff(0x004C1A84, 0, id, 0);
    op_stroff(0x004C1A86, 0, id, 0);
    op_stroff(0x004C1A92, 1, id, 0);
    op_stroff(0x004C1A97, 1, id, 0);
    op_stroff(0x004C1AB7, 0, id, 0);
    op_stroff(0x004C1AC3, 0, id, 0);
    op_stroff(0x004C1ACB, 0, id, 0);
    op_stroff(0x004C1ACF, 0, id, 0);
    op_stroff(0x004C1AD3, 0, id, 0);
    op_stroff(0x004C1AD9, 0, id, 0);
    op_stroff(0x004C1AEB, 0, id, 0);
    op_stroff(0x004C1B53, 0, id, 0);
    op_stroff(0x004C1B59, 0, id, 0);
    op_stroff(0x004C1B8E, 0, id, 0);
    op_stroff(0x004C1B9D, 0, id, 0);
    op_stroff(0x004C1BA4, 0, id, 0);
    op_stroff(0x004C1BAD, 0, id, 0);
    op_stroff(0x004C1BB6, 0, id, 0);
    op_stroff(0x004C1BC9, 0, id, 0);
    op_stroff(0x004C1BD1, 0, id, 0);
    op_stroff(0x004C1BD9, 0, id, 0);
    op_stroff(0x004C1BE1, 0, id, 0);
    op_stroff(0x004C1BEB, 0, id, 0);
    op_stroff(0x004C1C81, 0, id, 0);
    op_stroff(0x004C1C8A, 0, id, 0);
    op_stroff(0x004C1C92, 0, id, 0);
    op_stroff(0x004C1C9A, 0, id, 0);
    op_stroff(0x004C1CA0, 0, id, 0);
    op_stroff(0x004C1CBE, 0, id, 0);
    op_stroff(0x004C1CC8, 0, id, 0);
    op_stroff(0x004C1CFC, 0, id, 0);
    op_stroff(0x004C1D0F, 0, id, 0);
    op_stroff(0x004C1D4F, 0, id, 0);
    op_stroff(0x004C24D8, 1, id, 0);
    op_stroff(0x004C24E5, 0, id, 0);
    op_stroff(0x004C2525, 0, id, 0);
    op_stroff(0x004C2537, 0, id, 0);
    op_stroff(0x004C253E, 0, id, 0);
    op_stroff(0x004C2547, 1, id, 0);
    op_stroff(0x004C2570, 1, id, 0);
    op_stroff(0x004C2640, 0, id, 0);
    op_stroff(0x004C27F8, 1, id, 0);
    op_stroff(0x004C2819, 1, id, 0);
    op_stroff(0x004C28D4, 1, id, 0);
    op_stroff(0x004C28ED, 0, id, 0);
    op_stroff(0x004C2D8B, 1, id, 0);
    op_stroff(0x004C35CC, 0, id, 0);
    op_stroff(0x004C35DE, 0, id, 0);
    op_stroff(0x004C35E5, 0, id, 0);
    op_stroff(0x004C360B, 1, id, 0);
    op_stroff(0x004C361E, 0, id, 0);
    op_stroff(0x004C3628, 0, id, 0);
    op_stroff(0x004C3632, 0, id, 0);
    op_stroff(0x004C363C, 0, id, 0);
    op_stroff(0x004C363E, 0, id, 0);
    op_stroff(0x004C364C, 0, id, 0);
    op_stroff(0x004C366D, 0, id, 0);
    op_stroff(0x004C3676, 0, id, 0);
    op_stroff(0x004C367F, 0, id, 0);
    op_stroff(0x004C36AB, 1, id, 0);
    op_stroff(0x004C36B8, 0, id, 0);
    op_stroff(0x004C36D7, 0, id, 0);
    op_stroff(0x004C36E0, 0, id, 0);
    op_stroff(0x004C36E9, 0, id, 0);
    op_stroff(0x004C6093, 0, id, 0);
    op_stroff(0x004C609C, 0, id, 0);
    op_stroff(0x004C60A5, 1, id, 0);
    op_stroff(0x004C60B2, 0, id, 0);
    op_stroff(0x004C60B8, 1, id, 0);
    op_stroff(0x004C60C0, 1, id, 0);
    op_stroff(0x004C6109, 0, id, 0);
    op_stroff(0x004C625D, 0, id, 0);
    op_stroff(0x004C6285, 0, id, 0);
    op_stroff(0x004C6290, 1, id, 0);
    op_stroff(0x004C62C8, 1, id, 0);
    op_stroff(0x004C62F0, 1, id, 0);
    op_stroff(0x004C6319, 1, id, 0);
    op_stroff(0x004C632B, 0, id, 0);
    op_stroff(0x004C6338, 1, id, 0);
    op_stroff(0x004C6340, 1, id, 0);
    op_stroff(0x004C634C, 1, id, 0);
    op_stroff(0x004C6375, 1, id, 0);
    op_stroff(0x004C6387, 0, id, 0);
    op_stroff(0x004C6394, 1, id, 0);
    op_stroff(0x004C639C, 1, id, 0);
    op_stroff(0x004C641F, 0, id, 0);
    op_stroff(0x004C6456, 0, id, 0);
    op_stroff(0x004C6465, 1, id, 0);
    op_stroff(0x004C6470, 1, id, 0);
    op_stroff(0x004C64CD, 1, id, 0);
    op_stroff(0x004C64D4, 1, id, 0);
    op_stroff(0x004C65AA, 0, id, 0);
    op_stroff(0x004C65B1, 0, id, 0);
    op_stroff(0x004C65B8, 1, id, 0);
    op_stroff(0x004C65BF, 1, id, 0);
    op_stroff(0x004C65C6, 0, id, 0);
    op_stroff(0x004C6604, 0, id, 0);
    op_stroff(0x004C662B, 0, id, 0);
    op_stroff(0x004C663A, 1, id, 0);
    op_stroff(0x004C6645, 1, id, 0);
    op_stroff(0x004C66A2, 1, id, 0);
    op_stroff(0x004C66A9, 1, id, 0);
    op_stroff(0x004C677F, 0, id, 0);
    op_stroff(0x004C6786, 0, id, 0);
    op_stroff(0x004C678D, 1, id, 0);
    op_stroff(0x004C6794, 1, id, 0);
    op_stroff(0x004C679B, 0, id, 0);
    op_stroff(0x004C67D9, 0, id, 0);
    op_stroff(0x004C691B, 0, id, 0);
    op_stroff(0x004C6A5A, 1, id, 0);
    op_stroff(0x004C6A63, 1, id, 0);
    op_stroff(0x004C6A73, 1, id, 0);
    op_stroff(0x004C6A7D, 1, id, 0);
    op_stroff(0x004C6A81, 1, id, 0);
    op_stroff(0x004C6A93, 1, id, 0);
    op_stroff(0x004C6A99, 1, id, 0);
    op_stroff(0x004C6A9D, 1, id, 0);
    op_stroff(0x004C6AA8, 1, id, 0);
    op_stroff(0x004C6AAC, 1, id, 0);
    op_stroff(0x004C6AB0, 1, id, 0);
    op_stroff(0x004C6AB4, 1, id, 0);
    op_stroff(0x004C719C, 1, id, 0);
    op_stroff(0x004C7310, 1, id, 0);
    op_stroff(0x004C7313, 0, id, 0);
    op_stroff(0x004C735D, 1, id, 0);
    op_stroff(0x004C7782, 0, id, 0);
    op_stroff(0x004C778F, 0, id, 0);
    op_stroff(0x004C779C, 1, id, 0);
    op_stroff(0x004C77A0, 1, id, 0);
    op_stroff(0x004C77A4, 0, id, 0);
    op_stroff(0x004C77AB, 0, id, 0);
    op_stroff(0x004C77BF, 1, id, 0);
    op_stroff(0x004C77C3, 1, id, 0);
    op_stroff(0x004C77C7, 1, id, 0);
    op_stroff(0x004C77CB, 1, id, 0);
    op_stroff(0x004C77E1, 0, id, 0);
    op_stroff(0x004C781C, 1, id, 0);
    op_stroff(0x004C7823, 1, id, 0);
    op_stroff(0x004C782A, 1, id, 0);
    op_stroff(0x004C782E, 1, id, 0);
    op_stroff(0x004C7840, 0, id, 0);
    op_stroff(0x004C7875, 0, id, 0);
    op_stroff(0x004C7881, 1, id, 0);
    op_stroff(0x004C7885, 1, id, 0);
    op_stroff(0x004C7889, 1, id, 0);
    op_stroff(0x004C788F, 1, id, 0);
    op_stroff(0x004C7893, 1, id, 0);
    op_stroff(0x004C7899, 1, id, 0);
    op_stroff(0x004C789D, 1, id, 0);
    op_stroff(0x004C78A3, 1, id, 0);
    op_stroff(0x004C78A7, 1, id, 0);
    op_stroff(0x004C78AD, 1, id, 0);
    op_stroff(0x004C78B1, 0, id, 0);
    op_stroff(0x004C78B5, 0, id, 0);
    op_stroff(0x004C78B9, 0, id, 0);
    op_stroff(0x004C78CA, 0, id, 0);
    op_stroff(0x004C78D0, 0, id, 0);
    op_stroff(0x004C78D6, 0, id, 0);
    op_stroff(0x004C78DC, 0, id, 0);
    op_stroff(0x004C79A6, 0, id, 0);
    op_stroff(0x004C79AA, 0, id, 0);
    op_stroff(0x004C7A0B, 0, id, 0);
    op_stroff(0x004C7A1B, 1, id, 0);
    op_stroff(0x004C7A29, 1, id, 0);
    op_stroff(0x004C7A46, 1, id, 0);
    op_stroff(0x004C7AA5, 0, id, 0);
    op_stroff(0x004C7AA9, 0, id, 0);
    op_stroff(0x004C7AAD, 1, id, 0);
    op_stroff(0x004C7ABC, 1, id, 0);
    op_stroff(0x004C7B32, 1, id, 0);
    op_stroff(0x004C7B41, 1, id, 0);
    op_stroff(0x004C7B58, 0, id, 0);
    op_stroff(0x004C7BA0, 1, id, 0);
    op_stroff(0x004C7BDD, 0, id, 0);
    op_stroff(0x004C7BF0, 1, id, 0);
    op_stroff(0x004C7BFA, 1, id, 0);
    op_stroff(0x004C7C0A, 1, id, 0);
    op_stroff(0x004C7C36, 1, id, 0);
    op_stroff(0x004C7CA8, 1, id, 0);
    op_stroff(0x004C7CBA, 1, id, 0);
    op_stroff(0x004C7D8C, 1, id, 0);
    op_stroff(0x004C7D9B, 1, id, 0);
    op_stroff(0x004C7E03, 1, id, 0);
    op_stroff(0x004C7E22, 1, id, 0);
    op_stroff(0x004C7E2E, 1, id, 0);
    op_stroff(0x004C7E4F, 0, id, 0);
    op_stroff(0x004C7E64, 1, id, 0);
    op_stroff(0x004C7E7E, 1, id, 0);
    op_stroff(0x004C7E86, 1, id, 0);
    op_stroff(0x004C7E9F, 0, id, 0);
    op_stroff(0x004C7EC2, 0, id, 0);
    op_stroff(0x004C7EE3, 1, id, 0);
    op_stroff(0x004C7F40, 0, id, 0);
    op_stroff(0x004C7F53, 1, id, 0);
    op_stroff(0x004C7F5D, 1, id, 0);
    op_stroff(0x004C7F6D, 1, id, 0);
    op_stroff(0x004C7F95, 1, id, 0);
    op_stroff(0x004C80A4, 1, id, 0);
    op_stroff(0x004C80AA, 1, id, 0);
    op_stroff(0x004C80FC, 1, id, 0);
    op_stroff(0x004C8163, 1, id, 0);
    op_stroff(0x004C81A3, 0, id, 0);
    op_stroff(0x004C8302, 1, id, 0);
    op_stroff(0x004C8311, 1, id, 0);
    op_stroff(0x004C8352, 1, id, 0);
    op_stroff(0x004C8358, 1, id, 0);
    op_stroff(0x004C83BD, 0, id, 0);
    op_stroff(0x004C83D6, 1, id, 0);
    op_stroff(0x004C83E1, 1, id, 0);
    op_stroff(0x004C8444, 1, id, 0);
    op_stroff(0x004C844F, 1, id, 0);
    op_stroff(0x004C8469, 1, id, 0);
    op_stroff(0x004C84C6, 1, id, 0);
    op_stroff(0x004C84CC, 1, id, 0);
    op_stroff(0x004C8576, 1, id, 0);
    op_stroff(0x004C8593, 1, id, 0);
    op_stroff(0x004C85DE, 1, id, 0);
    op_stroff(0x004C85E2, 1, id, 0);
    op_stroff(0x004C85E8, 1, id, 0);
    op_stroff(0x004C85EC, 1, id, 0);
    op_stroff(0x004C85F6, 1, id, 0);
    op_stroff(0x004C85FA, 1, id, 0);
    op_stroff(0x004C860B, 1, id, 0);
    op_stroff(0x004C860F, 1, id, 0);
    op_stroff(0x004C863A, 1, id, 0);
    op_stroff(0x004C8645, 1, id, 0);
    op_stroff(0x004C8665, 1, id, 0);
    op_stroff(0x004C8671, 1, id, 0);
    op_stroff(0x004C872C, 1, id, 0);
    op_stroff(0x004C8738, 1, id, 0);
    op_stroff(0x004C8797, 1, id, 0);
    op_stroff(0x004C87A2, 1, id, 0);
    op_stroff(0x004C909C, 1, id, 0);
    op_stroff(0x004C90A8, 1, id, 0);
    op_stroff(0x004C9517, 1, id, 0);
    op_stroff(0x004C951E, 1, id, 0);
    op_stroff(0x004C954B, 1, id, 0);
    op_stroff(0x004C9558, 1, id, 0);
    op_stroff(0x004C9565, 1, id, 0);
    op_stroff(0x004C9572, 1, id, 0);
    op_stroff(0x004C9AA9, 1, id, 0);
    op_stroff(0x004C9AB2, 1, id, 0);
    op_stroff(0x004C9ABB, 1, id, 0);
    op_stroff(0x004C9AC4, 1, id, 0);
    op_stroff(0x004C9ACD, 0, id, 0);
    op_stroff(0x004C9AD6, 0, id, 0);
    op_stroff(0x004C9B0E, 1, id, 0);
    op_stroff(0x004C9B17, 1, id, 0);
    op_stroff(0x004C9B20, 1, id, 0);
    op_stroff(0x004C9B29, 1, id, 0);
    op_stroff(0x004C9B32, 0, id, 0);
    op_stroff(0x004C9B6A, 1, id, 0);
    op_stroff(0x004C9B72, 1, id, 0);
    op_stroff(0x004C9B78, 1, id, 0);
    op_stroff(0x004C9B8D, 1, id, 0);
    op_stroff(0x004C9B95, 1, id, 0);
    op_stroff(0x004CA0FA, 0, id, 0);
    op_stroff(0x004CA115, 1, id, 0);
    op_stroff(0x004CA141, 1, id, 0);
    op_stroff(0x004CA14A, 0, id, 0);
    op_stroff(0x004CA158, 1, id, 0);
    op_stroff(0x004CA161, 0, id, 0);
    op_stroff(0x004CA180, 1, id, 0);
    op_stroff(0x004CA191, 0, id, 0);
    op_stroff(0x004CA19B, 1, id, 0);
    op_stroff(0x004CA1A7, 0, id, 0);
    op_stroff(0x004CA1AE, 0, id, 0);
    op_stroff(0x004CA1B3, 0, id, 0);
    op_stroff(0x004CA1BA, 0, id, 0);
    op_stroff(0x004CA1C8, 0, id, 0);
    op_stroff(0x004CA1D7, 0, id, 0);
    op_stroff(0x004CA1EE, 0, id, 0);
    op_stroff(0x004CA202, 0, id, 0);
    op_stroff(0x004CA20E, 1, id, 0);
    op_stroff(0x004CA215, 1, id, 0);
    op_stroff(0x004CA221, 0, id, 0);
    op_stroff(0x004CA231, 0, id, 0);
    op_stroff(0x004CA23D, 1, id, 0);
    op_stroff(0x004CA242, 1, id, 0);
    op_stroff(0x004CA259, 0, id, 0);
    op_stroff(0x004CA25E, 0, id, 0);
    op_stroff(0x004CA272, 0, id, 0);
    op_stroff(0x004CA27E, 1, id, 0);
    op_stroff(0x004CA285, 1, id, 0);
    op_stroff(0x004CA2B2, 1, id, 0);
    op_stroff(0x004CA320, 0, id, 0);
    op_stroff(0x004CA32B, 0, id, 0);
    op_stroff(0x004CA360, 0, id, 0);
    op_stroff(0x004CA36E, 0, id, 0);
    op_stroff(0x004CA375, 0, id, 0);
    op_stroff(0x004CA3E5, 0, id, 0);
    op_stroff(0x004CA41A, 0, id, 0);
    op_stroff(0x004CA428, 0, id, 0);
    op_stroff(0x004CA42F, 0, id, 0);
    op_stroff(0x004CA5C5, 1, id, 0);
    op_stroff(0x004CA5CD, 1, id, 0);
    op_stroff(0x004CA5D5, 1, id, 0);
    op_stroff(0x004CA5DD, 1, id, 0);
    op_stroff(0x004CA5E5, 1, id, 0);
    op_stroff(0x004CA5ED, 1, id, 0);
    op_stroff(0x004CA602, 1, id, 0);
    op_stroff(0x004CA622, 1, id, 0);
    op_stroff(0x004CA68B, 1, id, 0);
    op_stroff(0x004CA68F, 1, id, 0);
    op_stroff(0x004CA693, 1, id, 0);
    op_stroff(0x004CA697, 1, id, 0);
    op_stroff(0x004CA6C0, 1, id, 0);
    op_stroff(0x004CA6C4, 1, id, 0);
    op_stroff(0x004CA6C8, 1, id, 0);
    op_stroff(0x004CA6CC, 1, id, 0);
    op_stroff(0x004CA70C, 1, id, 0);
    op_stroff(0x004CA710, 1, id, 0);
    op_stroff(0x004CA773, 1, id, 0);
    op_stroff(0x004CA7BD, 1, id, 0);
    op_stroff(0x004CA819, 1, id, 0);
    op_stroff(0x004CA863, 1, id, 0);
    op_stroff(0x004CA8AE, 1, id, 0);
    op_stroff(0x004CA8F8, 1, id, 0);
    op_stroff(0x004CAACE, 1, id, 0);
    op_stroff(0x004CAAD2, 1, id, 0);
    op_stroff(0x004CAB9B, 1, id, 0);
    op_stroff(0x004CAB9F, 1, id, 0);
    op_stroff(0x004CABA3, 1, id, 0);
    op_stroff(0x004CABA9, 1, id, 0);
    op_stroff(0x004CAC4B, 1, id, 0);
    op_stroff(0x004CAC4F, 1, id, 0);
    op_stroff(0x004CACAE, 1, id, 0);
    op_stroff(0x004CACB2, 1, id, 0);
    op_stroff(0x004CACFC, 1, id, 0);
    op_stroff(0x004CAD00, 1, id, 0);
    op_stroff(0x004CAD04, 1, id, 0);
    op_stroff(0x004CAD08, 1, id, 0);
    op_stroff(0x004CADF0, 1, id, 0);
    op_stroff(0x004CADF4, 1, id, 0);
    op_stroff(0x004CB021, 1, id, 0);
    op_stroff(0x004CB09A, 1, id, 0);
    op_stroff(0x004CB176, 1, id, 0);
    op_stroff(0x004CB17A, 1, id, 0);
    op_stroff(0x004CB17E, 1, id, 0);
    op_stroff(0x004CB182, 1, id, 0);
    op_stroff(0x004CB1E7, 1, id, 0);
    op_stroff(0x004CB1EB, 1, id, 0);
    op_stroff(0x004CB221, 1, id, 0);
    op_stroff(0x004CB2AE, 1, id, 0);
    op_stroff(0x004CB2B2, 1, id, 0);
    op_stroff(0x004CB2B6, 1, id, 0);
    op_stroff(0x004CB2BA, 1, id, 0);
    op_stroff(0x004CB2EA, 1, id, 0);
    op_stroff(0x004CB2EE, 1, id, 0);
    op_stroff(0x004CB932, 0, id, 0);
    op_stroff(0x004CB980, 1, id, 0);
    op_stroff(0x004CB988, 1, id, 0);
    op_stroff(0x004CB9AE, 1, id, 0);
    op_stroff(0x004CB9B6, 1, id, 0);
    op_stroff(0x004CB9BD, 1, id, 0);
    op_stroff(0x004CB9E5, 1, id, 0);
    op_stroff(0x004CB9E9, 1, id, 0);
    op_stroff(0x004CB9ED, 1, id, 0);
    op_stroff(0x004CB9F1, 1, id, 0);
    op_stroff(0x004CBA16, 1, id, 0);
    op_stroff(0x004CBA23, 1, id, 0);
    op_stroff(0x004CC6A7, 1, id, 0);
    op_stroff(0x004CC6AF, 1, id, 0);
    op_stroff(0x004CC6CE, 1, id, 0);
    op_stroff(0x004CC6EF, 1, id, 0);
    op_stroff(0x004CC6F5, 1, id, 0);
    op_stroff(0x004CC767, 0, id, 0);
    op_stroff(0x004CC77A, 1, id, 0);
    op_stroff(0x004CC77D, 1, id, 0);
    op_stroff(0x004CC793, 1, id, 0);
    op_stroff(0x004CC797, 1, id, 0);
    op_stroff(0x004CC7A5, 1, id, 0);
    op_stroff(0x004CC7A9, 0, id, 0);
    op_stroff(0x004CC7AD, 1, id, 0);
    op_stroff(0x004CC7B8, 1, id, 0);
    op_stroff(0x004CC7CB, 1, id, 0);
    op_stroff(0x004CC7D2, 1, id, 0);
    op_stroff(0x004CC7E8, 1, id, 0);
    op_stroff(0x004CC7F0, 1, id, 0);
    op_stroff(0x004CC930, 0, id, 0);
    op_stroff(0x004CC943, 0, id, 0);
    op_stroff(0x004CC947, 0, id, 0);
    op_stroff(0x004CC9E5, 1, id, 0);
    op_stroff(0x004CC9F0, 1, id, 0);
    op_stroff(0x004CCA19, 1, id, 0);
    op_stroff(0x004CD2A7, 1, id, 0);
    op_stroff(0x004CD2B8, 1, id, 0);
    op_stroff(0x004CD2C0, 0, id, 0);
    op_stroff(0x004CD2D6, 1, id, 0);
    op_stroff(0x004CD2DA, 1, id, 0);
    op_stroff(0x004CD2DE, 0, id, 0);
    op_stroff(0x004CD2E9, 0, id, 0);
    op_stroff(0x004CD2F1, 1, id, 0);
    op_stroff(0x004CD2F5, 1, id, 0);
    op_stroff(0x004CD2F9, 1, id, 0);
    op_stroff(0x004CD300, 0, id, 0);
    op_stroff(0x004CD304, 0, id, 0);
    op_stroff(0x004CD308, 1, id, 0);
    op_stroff(0x004CD30F, 0, id, 0);
    op_stroff(0x004CD313, 0, id, 0);
    op_stroff(0x004CD317, 1, id, 0);
    op_stroff(0x004CD718, 1, id, 0);
    op_stroff(0x004CE445, 0, id, 0);
    op_stroff(0x004CE44E, 1, id, 0);
    op_stroff(0x004CE526, 1, id, 0);
    op_stroff(0x004CE52C, 1, id, 0);
    op_stroff(0x004CE63B, 1, id, 0);
    op_stroff(0x004CE641, 0, id, 0);
    op_stroff(0x004CE647, 1, id, 0);
    op_stroff(0x004CE64D, 0, id, 0);
    op_stroff(0x004CE653, 1, id, 0);
    op_stroff(0x004CE659, 0, id, 0);
    op_stroff(0x004CE65F, 0, id, 0);
    op_stroff(0x004CE671, 0, id, 0);
    op_stroff(0x004CE67A, 0, id, 0);
    op_stroff(0x004CE687, 0, id, 0);
    op_stroff(0x004CE699, 0, id, 0);
    op_stroff(0x004CE6A4, 0, id, 0);
    op_stroff(0x004CE6B1, 0, id, 0);
    op_stroff(0x004CE760, 1, id, 0);
    op_stroff(0x004CE76E, 1, id, 0);
    op_stroff(0x004CE776, 1, id, 0);
    op_stroff(0x004CE7D5, 1, id, 0);
    op_stroff(0x004CE7D9, 1, id, 0);
    op_stroff(0x004CE89F, 1, id, 0);
    op_stroff(0x004CE8EF, 1, id, 0);
    op_stroff(0x004CEE0C, 1, id, 0);
    op_stroff(0x004CEE10, 1, id, 0);
    op_stroff(0x004CEE14, 1, id, 0);
    op_stroff(0x004CEE18, 1, id, 0);
    op_stroff(0x004CEE33, 0, id, 0);
    op_stroff(0x004CEE3C, 1, id, 0);
    op_stroff(0x004CEE45, 1, id, 0);
    op_stroff(0x004CEE4E, 1, id, 0);
    op_stroff(0x004CEE57, 1, id, 0);
    op_stroff(0x004CEE79, 1, id, 0);
    op_stroff(0x004CEE7D, 0, id, 0);
    op_stroff(0x004CEE86, 1, id, 0);
    op_stroff(0x004CEE8D, 0, id, 0);
    op_stroff(0x004CEE91, 1, id, 0);
    op_stroff(0x004CEFD8, 0, id, 0);
    op_stroff(0x004CEFE1, 0, id, 0);
    // sort off
}

static loco_initViewport(void)
{
    auto id;

    id = loco_makeStubStruct("viewport_t", 0x14);
    loco_setStructFld(id, 0x00, U16, "width");
    loco_setStructFld(id, 0x02, U16, "height");
    loco_setStructFld(id, 0x04, U16, "x");
    loco_setStructFld(id, 0x06, U16, "y");
    loco_setStructFld(id, 0x08, U16, "view_x");
    loco_setStructFld(id, 0x0A, U16, "view_y");
    loco_setStructFld(id, 0x0C, U16, "view_width");
    loco_setStructFld(id, 0x0E, U16, "view_height");
    loco_setStructFld(id, 0x10, U8, "zoom");
    loco_setStructFld(id, 0x12, U16, "flags");

    // sort on
    op_stroff(0x0045EFE2, 0, id, 0);
    op_stroff(0x0045EFE8, 0, id, 0);
    op_stroff(0x0045EFF3, 1, id, 0);
    op_stroff(0x0045EFF7, 1, id, 0);
    op_stroff(0x0045F05A, 1, id, 0);
    op_stroff(0x0045F05D, 1, id, 0);
    op_stroff(0x0045F067, 1, id, 0);
    op_stroff(0x0045F06B, 1, id, 0);
    op_stroff(0x0045F098, 1, id, 0);
    op_stroff(0x0045F09C, 1, id, 0);
    op_stroff(0x0045F0A6, 1, id, 0);
    op_stroff(0x0045F0AA, 1, id, 0);
    op_stroff(0x0045F0D3, 0, id, 0);
    op_stroff(0x0045F0D7, 0, id, 0);
    op_stroff(0x0045FCF7, 1, id, 0);
    op_stroff(0x0045FCFD, 1, id, 0);
    op_stroff(0x0045FD02, 1, id, 0);
    op_stroff(0x0045FD08, 1, id, 0);
    op_stroff(0x0045FD0E, 1, id, 0);
    op_stroff(0x0045FD19, 1, id, 0);
    op_stroff(0x0045FD1D, 1, id, 0);
    op_stroff(0x004C64BF, 1, id, 0);
    op_stroff(0x004C64C3, 1, id, 0);
    op_stroff(0x004C6596, 1, id, 0);
    op_stroff(0x004C65A0, 1, id, 0);
    op_stroff(0x004C65D2, 1, id, 0);
    op_stroff(0x004C65DF, 1, id, 0);
    op_stroff(0x004C661C, 1, id, 0);
    op_stroff(0x004C6620, 1, id, 0);
    op_stroff(0x004C68E6, 1, id, 0);
    op_stroff(0x004C68EA, 1, id, 0);
    op_stroff(0x004C68EE, 0, id, 0);
    op_stroff(0x004C68F2, 0, id, 0);
    op_stroff(0x004C68F6, 1, id, 0);
    op_stroff(0x004C6924, 1, id, 0);
    op_stroff(0x004C6928, 1, id, 0);
    op_stroff(0x004C692C, 1, id, 0);
    op_stroff(0x004C692F, 1, id, 0);
    op_stroff(0x004C6979, 0, id, 0);
    op_stroff(0x004C697D, 0, id, 0);
    op_stroff(0x004C6980, 0, id, 0);
    op_stroff(0x004C6984, 0, id, 0);
    op_stroff(0x004C6988, 0, id, 0);
    op_stroff(0x004C698C, 0, id, 0);
    op_stroff(0x004C6990, 0, id, 0);
    op_stroff(0x004C6994, 0, id, 0);
    op_stroff(0x004C6998, 1, id, 0);
    op_stroff(0x004C699B, 1, id, 0);
    op_stroff(0x004C69A4, 0, id, 0);
    op_stroff(0x004C69A7, 0, id, 0);
    op_stroff(0x004C69B0, 0, id, 0);
    op_stroff(0x004C69B4, 0, id, 0);
    op_stroff(0x004C69B8, 1, id, 0);
    op_stroff(0x004C69BC, 1, id, 0);
    op_stroff(0x004C69C8, 0, id, 0);
    op_stroff(0x004C69CE, 0, id, 0);
    op_stroff(0x004C69D2, 0, id, 0);
    op_stroff(0x004C69D8, 1, id, 0);
    op_stroff(0x004C69E1, 0, id, 0);
    op_stroff(0x004C69E5, 0, id, 0);
    op_stroff(0x004C69EE, 0, id, 0);
    op_stroff(0x004C69F2, 0, id, 0);
    op_stroff(0x004C69F6, 1, id, 0);
    op_stroff(0x004C69FA, 1, id, 0);
    op_stroff(0x004C6A07, 0, id, 0);
    op_stroff(0x004C6A0E, 0, id, 0);
    op_stroff(0x004C6A12, 0, id, 0);
    op_stroff(0x004C6A20, 0, id, 0);
    op_stroff(0x004C6A24, 0, id, 0);
    op_stroff(0x004C6A28, 0, id, 0);
    op_stroff(0x004C6A2C, 0, id, 0);
    op_stroff(0x004C6A30, 0, id, 0);
    op_stroff(0x004C6A34, 0, id, 0);
    op_stroff(0x004C6A38, 0, id, 0);
    op_stroff(0x004C6A3B, 0, id, 0);
    op_stroff(0x004C6A6C, 1, id, 0);
    op_stroff(0x004C6A70, 1, id, 0);
    op_stroff(0x004C6A85, 1, id, 0);
    op_stroff(0x004C6A8B, 1, id, 0);
    op_stroff(0x004C6A8F, 1, id, 0);
    op_stroff(0x004C6ABE, 1, id, 0);
    op_stroff(0x004C6ACA, 1, id, 0);
    op_stroff(0x004C6AD5, 1, id, 0);
    op_stroff(0x004C6AE1, 1, id, 0);
    op_stroff(0x004CA4A8, 1, id, 0);
    op_stroff(0x004CA4B2, 1, id, 0);
    op_stroff(0x004CEE98, 0, id, 0);
    // sort off
}

static loco_initCompany(void)
{
    auto id;
    id = loco_makeStubStruct("currency48_t", 0x6);

    id = loco_makeStubStruct("company_t", 0x8FA8);
    loco_setStructFld(id, 0x00, U16, "name");
    loco_setStructFld(id, 0x02, U16, "owner_name");
    loco_setStructFld(id, 0x04, U16, "challenge_flags");
    loco_setStructFld(id, 0x08, U32, "cash.var_00");
    loco_setStructFld(id, 0x0C, U16, "cash.var_02");
    loco_setStructFld(id, 0x0E, U32, "current_loan");
    loco_setStructFld(id, 0x12, U32, "update_counter");
    loco_setStructFld(id, 0x16, U16, "performance_index");
    loco_setStructFld(id, 0x18, U8, "competitor_id");
    loco_setStructFld(id, 0x19, U8, "owner_emotion");
    loco_setStructFld(id, 0x1A, U8, "colour.primary");
    loco_setStructFld(id, 0x1B, U8, "colour.secondary");
    loco_setStructFldArray(id, 0x1C, U16, "vehicleColours", 10);
    loco_setStructFld(id, 0x30, U16, "customVehicleColoursSet");
    loco_setStructFldArray(id, 0x34, U32, "unlocked_vehicles", 7);
    loco_setStructFld(id, 0x50, U16, "available_vehicles");
    loco_setStructFld(id, 0x57, U8, "numExpenditureMonths");
    loco_setStructFldArray(id, 0x58, U32, "expenditures", 17*16);
    loco_setStructFld(id, 0x498, U32, "startedDate");
    loco_setStructFld(id, 0x2579, U8, "headquarters_z");
    loco_setStructFld(id, 0x257A, U16, "headquarters_x");
    loco_setStructFld(id, 0x257C, U16, "headquarters_y");
    loco_setStructFldArray(id, 0x85FC, U32, "cargo_units_delivered_history", 120);
    loco_setStructFldArray(id, 0x87DC, U16, "performance_index_history", 120);
    loco_setStructFld(id, 0x88CC, U16, "history_size");
    loco_setStructSub(id, 0x88CE, "currency48_t", "companyValueHistory", 120);
    loco_setStructFld(id, 0x8B9E, U32, "vehicleProfit.var_00");
    loco_setStructFld(id, 0x8BA2, U16, "vehicleProfit.var_04");
    loco_setStructFldArray(id, 0x8BA4, U16, "transportTypeCount", 6);
    loco_setStructFld(id, 0x8BBC, U16, "observation_thing");
    loco_setStructFld(id, 0x8BBE, U16, "observation_x");
    loco_setStructFld(id, 0x8BC0, U16, "observation_y");
    loco_setStructFldArray(id, 0x8BCE, U32, "cargoDelivered", 32);
    loco_setStructFld(id, 0x8C4E, U8, "challengeProgress");
    loco_setStructFldArray(id, 0x8C54, U32, "cargo_units_distance_history", 120);
    loco_setStructFld(id, 0x8E34, U16, "jail_status");
}

static loco_initTown(void)
{
    auto id;

    id = loco_makeStubStruct("town_t", 0x270);
    loco_setStructFld(id, 0x00, U16, "name");
    loco_setStructFld(id, 0x02, U16, "x");
    loco_setStructFld(id, 0x04, U16, "y");
    loco_setStructFld(id, 0x06, U16, "flags");
    loco_setStructFld(id, 0x30, U32, "population");
    loco_setStructFldArray(id, 0x3A, S16, "company_ratings", 15);
    loco_setStructFld(id, 0x58, U16, "companies_with_rating");
    loco_setStructFld(id, 0x5A, U8, "size");
    loco_setStructFld(id, 0x5B, U8, "history_size");
    loco_setStructFldArray(id, 0x5C, U8, "history", 20*12);
    loco_setStructFld(id, 0x14C, S32, "history_min_population");
    loco_setStructFldArray(id, 0x158, S16, "monthly_cargo_delivered", 32);
    loco_setStructFld(id, 0x198, U32, "cargo_influence_flags");
    loco_setStructFld(id, 0x1A4, U8, "build_speed");
    loco_setStructFld(id, 0x1A6, U16, "num_stations");
}

static loco_initIndustries(void)
{
    auto id;

    id = loco_makeStubStruct("industry_t", 0x453);
    loco_setStructFld(id, 0x00, U16, "name");
    loco_setStructFld(id, 0x02, U16, "x");
    loco_setStructFld(id, 0x04, U16, "y");
    loco_setStructFld(id, 0x06, U16, "flags");
    loco_setStructFld(id, 0x08, U32, "prng1");
    loco_setStructFld(id, 0x0C, U32, "prng2");
    loco_setStructFld(id, 0x10, U8, "object_id");
    loco_setStructFld(id, 0x11, U8, "under_construction");
    loco_setStructFld(id, 0xD5, U16, "town");
    loco_setStructFld(id, 0xD7, U32, "tile_loop");
    loco_setStructFld(id, 0xE0, U8, "owner");
    loco_setStructFldArray(id, 0x189, U16, "produced_cargo_quantity", 2);
    loco_setStructFldArray(id, 0x193, U16, "received_cargo_quantity", 3);
    loco_setStructFldArray(id, 0x1A3, U16, "produced_cargo_max", 2);
    loco_setStructFldArray(id, 0x1A7, U8, "produced_cargo_transported", 2);
    loco_setStructFldArray(id, 0x1A9, U8, "history_size", 2);
    loco_setStructFldArray(id, 0x1AB, U8, "history_1", 20*12);
    loco_setStructFldArray(id, 0x29B, U8, "history_2", 20*12);
    loco_setStructFldArray(id, 0x38B, U32, "history_min_production", 2);
}

static loco_initStation(void)
{
    auto id;

    id = loco_makeStubStruct("station_cargo_stats", 0x0D);
    loco_setStructFld(id, 0x00, U16, "quantity");
    loco_setStructFld(id, 0x02, U16, "origin_station");
    loco_setStructFld(id, 0x04, U8, "flags");
    loco_setStructFld(id, 0x05, U8, "age");
    loco_setStructFld(id, 0x06, U8, "rating");
    loco_setStructFld(id, 0x07, U8, "enroute_age");
    loco_setStructFld(id, 0x0B, U8, "industry_id");

    id = loco_makeStubStruct("station_t", 0x3D2);
    loco_setStructFld(id, 0x00, U16, "name");
    loco_setStructFld(id, 0x02, U16, "x");
    loco_setStructFld(id, 0x04, U16, "y");
    loco_setStructFld(id, 0x06, U16, "z");
    loco_setStructFldArray(id, 0x08, U16, "label_left", 4);
    loco_setStructFldArray(id, 0x10, U16, "label_right", 4);
    loco_setStructFldArray(id, 0x18, U16, "label_top", 4);
    loco_setStructFldArray(id, 0x20, U16, "label_bottom", 4);
    loco_setStructFld(id, 0x28, U8, "owner");
    loco_setStructFld(id, 0x2A, U16, "flags");
    loco_setStructFld(id, 0x2C, U16, "town");
    loco_setStructSub(id, 0x2E, "station_cargo_stats", "cargo_stats", 32);
    loco_setStructFld(id, 0x1CE, U16, "stationTileSize");
    loco_setStructSub(id, 0x1D0, "map_pos3", "stationTiles", 80);
}

static loco_initEntities(void)
{
    auto id;
    loco_makeStubStruct("entity_t", 128);

    id = loco_makeStubStruct("EntityBase", 32);
    loco_setStructFld(id, 0x00, U8, "baseType");
    loco_setStructFld(id, 0x01, U8, "type");
    loco_setStructFld(id, 0x02, U16, "nextQuadrantId");
    loco_setStructFld(id, 0x04, U16, "nextEntityId");
    loco_setStructFld(id, 0x0A, U16, "id");
    loco_setStructFld(id, 0x0E, U16, "x");
    loco_setStructFld(id, 0x10, U16, "y");
    loco_setStructFld(id, 0x12, U16, "z");
    loco_setStructFld(id, 0x16, U16, "spriteLeft");
    loco_setStructFld(id, 0x18, U16, "spriteTop");
    loco_setStructFld(id, 0x1A, U16, "spriteRight");
    loco_setStructFld(id, 0x1C, U16, "spriteBottom");
    loco_setStructFld(id, 0x1E, U8, "spriteYaw");
    loco_setStructFld(id, 0x1F, U8, "spritePitch");

    loco_initVehicles();
}

static loco_initVehicles(void)
{
    auto id;

    id = loco_makeStubStruct("VehicleHead", 128);
    loco_setStructSub(id, 0x00, "EntityBase", "base", 1);
    loco_setStructFld(id, 0x21, U8, "owner");
    loco_setStructFld(id, 0x26, U16, "head");
    loco_setStructFld(id, 0x30, U16, "tileX");
    loco_setStructFld(id, 0x32, U16, "tileY");
    loco_setStructFld(id, 0x34, U8, "tileBaseZ");
    loco_setStructFld(id, 0x35, U8, "trackType");
    loco_setStructFld(id, 0x3A, U16, "nextCarId");
    loco_setStructFld(id, 0x42, U8, "mode");
    loco_setStructFld(id, 0x46, U32, "orderTableOffset");
    loco_setStructFld(id, 0x4A, U16, "currentOrder");
    loco_setStructFld(id, 0x4C, U16, "sizeOfOrderTable");
    loco_setStructFld(id, 0x54, U16, "stationId");
    loco_setStructFld(id, 0x5D, U8, "status");
    loco_setStructFld(id, 0x5E, U8, "vehicleType");
    loco_setStructFld(id, 0x77, U16, "lastAverageSpeed");

    id = loco_makeStubStruct("Vehicle1", 128);
    loco_setStructSub(id, 0x00, "EntityBase", "base", 1);
    loco_setStructFld(id, 0x21, U8, "owner");
    loco_setStructFld(id, 0x26, U16, "head");
    loco_setStructFld(id, 0x30, U16, "tileX");
    loco_setStructFld(id, 0x32, U16, "tileY");
    loco_setStructFld(id, 0x34, U8, "tileBaseZ");
    loco_setStructFld(id, 0x35, U8, "trackType");
    loco_setStructFld(id, 0x3A, U16, "nextCarId");
    loco_setStructFld(id, 0x42, U8, "mode");

    id = loco_makeStubStruct("Vehicle2", 128);
    loco_setStructSub(id, 0x00, "EntityBase", "base", 1);
    loco_setStructFld(id, 0x21, U8, "owner");
    loco_setStructFld(id, 0x26, U16, "head");
    loco_setStructFld(id, 0x30, U16, "tileX");
    loco_setStructFld(id, 0x32, U16, "tileY");
    loco_setStructFld(id, 0x34, U8, "tileBaseZ");
    loco_setStructFld(id, 0x35, U8, "trackType");
    loco_setStructFld(id, 0x3A, U16, "nextCarId");
    loco_setStructFld(id, 0x42, U8, "mode");
    loco_setStructFld(id, 0x44, U8, "drivingSoundId");
    loco_setStructFld(id, 0x45, U8, "drivingSoundVolume");
    loco_setStructFld(id, 0x46, U16, "drivingSoundFrequency");
    loco_setStructFld(id, 0x48, U16, "objectId");
    loco_setStructFld(id, 0x4C, U16, "soundWindowNumber");
    loco_setStructFld(id, 0x4E, U8, "soundWindowType");
    loco_setStructFld(id, 0x50, U16, "totalPower");
    loco_setStructFld(id, 0x52, U16, "totalWeight");
    loco_setStructFld(id, 0x54, U16, "maxSpeed");
    loco_setStructFld(id, 0x56, U32, "currentSpeed");
    loco_setStructFld(id, 0x5C, U16, "rackRailMaxSpeed");
    loco_setStructFld(id, 0x62, U32, "refundCost");
    loco_setStructFld(id, 0x72, U8, "reliability");

    id = loco_makeStubStruct("VehicleBody", 128);
    loco_setStructSub(id, 0x00, "EntityBase", "base", 1);
    loco_setStructFld(id, 0x21, U8, "owner");
    loco_setStructFld(id, 0x24, U8, "colourScheme.primary");
    loco_setStructFld(id, 0x25, U8, "colourScheme.secondary");
    loco_setStructFld(id, 0x26, U16, "head");
    loco_setStructFld(id, 0x30, U16, "tileX");
    loco_setStructFld(id, 0x32, U16, "tileY");
    loco_setStructFld(id, 0x34, U8, "tileBaseZ");
    loco_setStructFld(id, 0x35, U8, "trackType");
    loco_setStructFld(id, 0x39, U8, "objectSpriteType");
    loco_setStructFld(id, 0x3A, U16, "nextCarId");
    loco_setStructFld(id, 0x40, U16, "objectId");
    loco_setStructFld(id, 0x42, U8, "mode");
    loco_setStructFld(id, 0x48, U32, "acceptedCargoTypes");
    loco_setStructFld(id, 0x4C, U8, "cargoType");
    loco_setStructFld(id, 0x4D, U8, "maxCargo");
    loco_setStructFld(id, 0x4E, U16, "townCargoFrom");
    loco_setStructFld(id, 0x51, U8, "primaryCargoQty");
    loco_setStructFld(id, 0x54, U8, "bodyIndex");
    loco_setStructFld(id, 0x56, U32, "creationDay");

    id = loco_makeStubStruct("VehicleBogie", 128);
    loco_setStructSub(id, 0x00, "EntityBase", "base", 1);
    loco_setStructFld(id, 0x21, U8, "owner");
    loco_setStructFld(id, 0x24, U8, "colourScheme.primary");
    loco_setStructFld(id, 0x25, U8, "colourScheme.secondary");
    loco_setStructFld(id, 0x26, U16, "head");
    loco_setStructFld(id, 0x30, U16, "tileX");
    loco_setStructFld(id, 0x32, U16, "tileY");
    loco_setStructFld(id, 0x34, U8, "tileBaseZ");
    loco_setStructFld(id, 0x35, U8, "trackType");
    loco_setStructFld(id, 0x39, U8, "objectSpriteType");
    loco_setStructFld(id, 0x3A, U16, "nextCarId");
    loco_setStructFld(id, 0x40, U16, "objectId");
    loco_setStructFld(id, 0x42, U8, "mode");
    loco_setStructFld(id, 0x48, U32, "acceptedCargoTypes");
    loco_setStructFld(id, 0x4C, U8, "cargoType");
    loco_setStructFld(id, 0x4D, U8, "maxCargo");
    loco_setStructFld(id, 0x4E, U16, "townCargoFrom");
    loco_setStructFld(id, 0x51, U8, "secondaryCargoQty");
    loco_setStructFld(id, 0x54, U8, "bodyIndex");
    loco_setStructFld(id, 0x56, U32, "creationDay");
    loco_setStructFld(id, 0x62, U32, "refundCost");
    loco_setStructFld(id, 0x66, U16, "reliability");

    id = loco_makeStubStruct("VehicleTail", 128);
    loco_setStructSub(id, 0x00, "EntityBase", "base", 1);
    loco_setStructFld(id, 0x21, U8, "owner");
    loco_setStructFld(id, 0x26, U16, "head");
    loco_setStructFld(id, 0x30, U16, "tileX");
    loco_setStructFld(id, 0x32, U16, "tileY");
    loco_setStructFld(id, 0x34, U8, "tileBaseZ");
    loco_setStructFld(id, 0x35, U8, "trackType");
    loco_setStructFld(id, 0x3A, U16, "nextCarId");
    loco_setStructFld(id, 0x42, U8, "mode");
    loco_setStructFld(id, 0x44, U8, "drivingSoundId");
    loco_setStructFld(id, 0x45, U8, "drivingSoundVolume");
    loco_setStructFld(id, 0x46, U16, "drivingSoundFrequency");
    loco_setStructFld(id, 0x48, U16, "objectId");
    loco_setStructFld(id, 0x4C, U16, "soundWindowNumber");
    loco_setStructFld(id, 0x4E, U8, "soundWindowType");
    loco_setStructFld(id, 0x4F, U16, "trainDanglingTimeout");

    id = loco_makeStubStruct("Vehicle2or6", 128);
    loco_setStructSub(id, 0x00, "EntityBase", "base", 1);
    loco_setStructFld(id, 0x44, U8, "drivingSoundId");
    loco_setStructFld(id, 0x45, U8, "drivingSoundVolume");
    loco_setStructFld(id, 0x46, U16, "drivingSoundFrequency");
    loco_setStructFld(id, 0x48, U16, "objectId");
    loco_setStructFld(id, 0x4C, U16, "soundWindowNumber");
    loco_setStructFld(id, 0x4E, U8, "soundWindowType");
}

static loco_initMapAnimations(void)
{
    loco_makeStubStruct("map_animation_t", 6);
}

static loco_initUserStrings(void)
{
    loco_makeStubStruct("str_32", 32);
}

static loco_initG1Elements(void)
{
    auto id;

    id = loco_makeStubStruct("g1_element_t", 0x10);
    loco_setStructFld(id, 0x00, U32, "offset");
    loco_setStructFld(id, 0x04, U16, "width");
    loco_setStructFld(id, 0x06, U16, "height");
    loco_setStructFld(id, 0x08, U16, "x");
    loco_setStructFld(id, 0x0A, U16, "y");
    loco_setStructFld(id, 0x0C, U16, "flags");
    loco_setStructFld(id, 0x0E, U16, "pad");
}

static loco_initPaintStructs(void)
{
    auto id;

    id = loco_makeStubStruct("AttachedPaintStruct", 0x12);
    loco_setStructFld(id, 0x00, U32, "imageId");
    loco_setStructFld(id, 0x04, U32, "colourimageId");
    loco_setStructFld(id, 0x08, U16, "x");
    loco_setStructFld(id, 0x0A, U16, "y");
    loco_setStructFld(id, 0x0C, U8, "flags");
    loco_setStructFld(id, 0x0D, U8, "pad_0D");
    loco_setStructFld(id, 0x0E, U32, "next");

    id = loco_makeStubStruct("PaintStringStruct", 0x20);
    loco_setStructFld(id, 0x00, U16, "stringId");
    loco_setStructFld(id, 0x02, U32, "next");
    loco_setStructFld(id, 0x06, U16, "x");
    loco_setStructFld(id, 0x08, U16, "y");
    loco_setStructFldArray(id, 0x0A, U16, "args", 7);
    loco_setStructFld(id, 0x1A, U32, "yOffsets");
    loco_setStructFld(id, 0x1E, U16, "colour");

    id = loco_makeStubStruct("PaintStructBoundBox", 0xC);
    loco_setStructFld(id, 0x00, U16, "x");
    loco_setStructFld(id, 0x02, U16, "y");
    loco_setStructFld(id, 0x04, U16, "z");
    loco_setStructFld(id, 0x06, U16, "zEnd");
    loco_setStructFld(id, 0x08, U16, "xEnd");
    loco_setStructFld(id, 0x0A, U16, "yEnd");

    id = loco_makeStubStruct("PaintStruct", 0x34);
    loco_setStructFld(id, 0x00, U32, "imageId");
    loco_setStructFld(id, 0x04, U32, "colourimageId");
    loco_setStructSub(id, 0x08, "PaintStructBoundBox", "bounds", 1);
    loco_setStructFld(id, 0x14, U16, "x");
    loco_setStructFld(id, 0x16, U16, "y");
    loco_setStructFld(id, 0x18, U16, "quadrantIndex");
    loco_setStructFld(id, 0x1A, U8, "flags");
    loco_setStructFld(id, 0x1B, U8, "quadrantFlags");
    loco_setStructFld(id, 0x1C, U32, "attachedPS");
    loco_setStructFld(id, 0x20, U32, "children");
    loco_setStructFld(id, 0x24, U32, "nextQuadrantPS");
    loco_setStructFld(id, 0x28, U8, "type");
    loco_setStructFld(id, 0x29, U8, "var_29");
    loco_setStructFld(id, 0x2C, U16, "map_x");
    loco_setStructFld(id, 0x2E, U16, "map_y");
    loco_setStructFld(id, 0x30, U32, "tileElement");
}

static loco_makePointerArray(offset, count, name)
{
    MakeUnknown(offset, count * 4, 1 | 2);
    MakeData(offset, FF_DWRD, 4, BADADDR);
    OpOff(offset, 1, 0);
    MakeName(offset, name);
    MakeArray(offset, count);
    SetColor(offset, CIC_ITEM, 0xFFFFFF);
}

static loco_initDropdowns(void)
{
    loco_makeStubStruct("dropdown_format_args_t", 0x8);
}

static loco_initGFX(void)
{
    auto id;

    id = loco_makeStubStruct("drawpixelinfo_t", 0x10);
    loco_setStructFld(id, 0x00, U32, "bits");
    loco_setStructFld(id, 0x04, U16, "x");
    loco_setStructFld(id, 0x06, U16, "y");
    loco_setStructFld(id, 0x08, U16, "width");
    loco_setStructFld(id, 0x0A, U16, "height");
    loco_setStructFld(id, 0x0C, U16, "pitch");
    loco_setStructFld(id, 0x0E, U16, "zoom_level");

    // sort on
    op_stroff(0x004CA5FE, 1, id, 0);
    op_stroff(0x004CA610, 1, id, 0);
    op_stroff(0x004CA61E, 1, id, 0);
    op_stroff(0x004CA630, 1, id, 0);
    op_stroff(0x004CB61A, 1, id, 0);
    op_stroff(0x004CB622, 1, id, 0);
    op_stroff(0x004CB62D, 1, id, 0);
    op_stroff(0x004CB638, 1, id, 0);
    op_stroff(0x004CB643, 1, id, 0);
    op_stroff(0x004CB64E, 1, id, 0);
    op_stroff(0x004CB659, 1, id, 0);
    // sort off
}

static loco_initColours(void)
{
    auto idA, idB, i;

    idA = loco_makeStubStruct("colourmap_a", 0x8);
    idB = loco_makeStubStruct("colourmap_b", 0x8);
    for (i = 0; i < 8; i++) {
        loco_setStructFld(idA, i, U8, sprintf("shade_%d", i));
        loco_setStructFld(idB, i, U8, sprintf("shade_%d", i + 8));
    }
}

static loco_initTile(void)
{
    auto id;

    id = loco_makeStubStruct("map_pos", 0x04);
    loco_setStructFld(id, 0x00, U16, "x");
    loco_setStructFld(id, 0x02, U16, "y");

    id = loco_makeStubStruct("map_pos3", 0x06);
    loco_setStructFld(id, 0x00, U16, "x");
    loco_setStructFld(id, 0x02, U16, "y");
    loco_setStructFld(id, 0x04, U16, "z");
}

static loco_initMessage(void)
{
    auto id;

    id = loco_makeStubStruct("message", 0xD4);
    loco_setStructFld(id, 0x00, U8, "type");
    loco_setStructFldArray(id, 0x01, U8, "messageString", 198);
    loco_setStructFld(id, 0x0C7, U8, "companyId");
    loco_setStructFld(id, 0x0CA, U16, "item_id_1");
    loco_setStructFld(id, 0x0CC, U16, "item_id_2");
    loco_setStructFld(id, 0x0D0, U32, "date");
}
