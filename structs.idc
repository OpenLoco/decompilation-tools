#include <idc.idc>

static initStructs(void)
{
    loco_initWindow();
    loco_initCompany();
    loco_initTown();
    loco_initIndustries();
    loco_initStation();
    loco_initObjects();
    loco_initEntities();
    loco_initG1Elements();
    loco_initUserStrings();
    loco_initDropdowns();

    loco_makePointerArray(0xE40134, 0x30000, "tile_map_element_pointers");
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
    MakeArray(offset, count);
    SetColor(offset, CIC_ITEM, 0xFFFFFF);
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

#define U8 0
#define U16 1
#define U32 2
#define S8 3
#define S16 4
#define S32 5

static loco_setStructFld(id, offset, type, name)
{
    if (type == U8 || type == S8) {
        loco_setStructMember(id, offset, name, FF_BYTE | FF_DATA, 1);
    } else if (type == U16 || type == S16) {
        loco_setStructMember(id, offset, name, FF_WORD | FF_DATA, 2);
    } else if (type == U32 || type == S32) {
        loco_setStructMember(id, offset, name, FF_DWRD | FF_DATA, 4);
    }
}

static loco_setStructVar(id, offset, type)
{
    loco_setStructFld(id, offset, type, form("var_%03X", offset));
}

static loco_setStructSub(id, offset, type, name, count)
{
    DelStrucMember(id, offset);
    AddStrucMember(id, name, offset, FF_STRU | FF_DATA, GetStrucIdByName(type), GetStrucSize(GetStrucIdByName(type)) * count);
}

static loco_initWindow(void)
{
    auto id;

    id = loco_makeStubStruct("scroll_t", 0x12);
    loco_setStructFld(id, 0x00, U16, "flags");
    loco_setStructVar(id, 0x02, U16);
    loco_setStructVar(id, 0x04, U16);
    loco_setStructVar(id, 0x06, U16);
    loco_setStructVar(id, 0x08, U16);
    loco_setStructVar(id, 0x0A, U16);
    loco_setStructVar(id, 0x0C, U16);
    loco_setStructVar(id, 0x0E, U16);
    loco_setStructVar(id, 0x10, U16);

    id = loco_makeStubStruct("window_t", 0x88E);
    loco_setStructFld(id, 0x000, U32, "event_handlers");
    loco_setStructVar(id, 0x004, U32);
    loco_setStructFld(id, 0x00C, U32, "enabled_widgets");
    loco_setStructVar(id, 0x010, U32);
    loco_setStructVar(id, 0x014, U32);
    loco_setStructVar(id, 0x01c, U32);
    loco_setStructVar(id, 0x024, U32);
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
    loco_setStructVar(id, 0x042, U32);
    loco_setStructSub(id, 0x046, "scroll_t", "scroll", 3);
    loco_setStructVar(id, 0x848, U32);
    loco_setStructVar(id, 0x84C, U32);
    loco_setStructVar(id, 0x870, U16);
    loco_setStructVar(id, 0x872, U16);
    loco_setStructFld(id, 0x876, U16, "thing");
    loco_setStructVar(id, 0x878, U16);
    loco_setStructVar(id, 0x87A, U16);
    loco_setStructFld(id, 0x882, U8, "type");
    loco_setStructVar(id, 0x884, U8);
    loco_setStructVar(id, 0x886, U8);

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
    op_stroff(0x00457F55, 0, id, 0);
    op_stroff(0x00457FA1, 0, id, 0);
    op_stroff(0x00457FBC, 1, id, 0);
    op_stroff(0x00458708, 0, id, 0);
    op_stroff(0x00458712, 0, id, 0);
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
    op_stroff(0x0043281A, 1, id, 0);
    op_stroff(0x00432A21, 1, id, 0);
    op_stroff(0x00435008, 0, id, 0);
    op_stroff(0x00435024, 1, id, 0);
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
    op_stroff(0x004C68EE, 0, id, 0);
    op_stroff(0x004C68F2, 0, id, 0);
    op_stroff(0x004C691B, 0, id, 0);
    op_stroff(0x004C719C, 1, id, 0);
    op_stroff(0x004C7310, 1, id, 0);
    op_stroff(0x004C7313, 0, id, 0);
    op_stroff(0x004C735D, 1, id, 0);
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
    op_stroff(0x004C8302, 1, id, 0);
    op_stroff(0x004C8311, 1, id, 0);
    op_stroff(0x004C8469, 1, id, 0);
    op_stroff(0x004C85DE, 1, id, 0);
    op_stroff(0x004C85E2, 1, id, 0);
    op_stroff(0x004C85F6, 1, id, 0);
    op_stroff(0x004C85FA, 1, id, 0);
    op_stroff(0x004C860B, 1, id, 0);
    op_stroff(0x004C860F, 1, id, 0);
    op_stroff(0x004C909C, 1, id, 0);
    op_stroff(0x004C90A8, 1, id, 0);
    op_stroff(0x004C9AA9, 1, id, 0);
    op_stroff(0x004C9ABB, 1, id, 0);
    op_stroff(0x004C9AC4, 1, id, 0);
    op_stroff(0x004C9ACD, 0, id, 0);
    op_stroff(0x004C9AD6, 0, id, 0);
    op_stroff(0x004CA0FA, 0, id, 0);
    op_stroff(0x004CA115, 1, id, 0);
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
    op_stroff(0x004CBA16, 1, id, 0);
    op_stroff(0x004CBA23, 1, id, 0);
    op_stroff(0x004CC6EF, 1, id, 0);
    op_stroff(0x004CC6F5, 1, id, 0);
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
    op_stroff(0x004CE8EF, 1, id, 0);

    loco_makeStructArray(0x11370AC, "window_t", 12, "_windows");
}

static loco_initCompany(void)
{
    loco_makeStubStruct("company_t", 0x8FA8);
    loco_makeStructArray(0x531784, "company_t", 15, "_companies");
}

static loco_initTown(void)
{
    loco_makeStubStruct("town_t", 0x270);
    loco_makeStructArray(0x5B825C, "town_t", 80, "_towns");
}

static loco_initIndustries(void)
{
    loco_makeStubStruct("industry_t", 0x453);
    loco_makeStructArray(0x5C455C, "industry_t", 128, "_industries");
}

static loco_initStation(void)
{
    loco_makeStubStruct("station_t", 0x3D2);
    loco_makeStructArray(0x5E6EDC, "station_t", 1024, "_stations");
}

static loco_initEntities(void)
{
    loco_makeStubStruct("entity_t", 128);
    loco_makeStructArray(0x6DB6DC, "entity_t", 20000, "_entities");
}

static loco_initMapAnimations(void)
{
    loco_makeStubStruct("map_animation_t", 6);
    loco_makeStructArray(0x94C6DC, "map_animation_t", 0x2000, "_mapAnimations");
}

static loco_initUserStrings(void)
{
    loco_makeStubStruct("str_32", 32);
    loco_makeStructArray(0x95885C, "str_32", 0x800, "_userStrings");
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

    loco_makeStructArray(0x9E2424, "g1_element_t", 0x4201A, "_g1Data");
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

static loco_initObjects(void)
{
    loco_makeStubStruct("object_repository_item", 0x8);
    loco_makeStructArray(0x4FE0B8, "object_repository_item", 64, "object_repository");

    loco_makePointerArray(0x0050C3D0, 1, "_interfaceObjects");
    loco_makePointerArray(0x0050C3D4, 128, "_soundObjects");
    loco_makePointerArray(0x0050C5D4, 1, "_currencyObjects");
    loco_makePointerArray(0x0050C5D8, 32, "_steamObjects");
    loco_makePointerArray(0x0050C658, 8, "_rockObjects");
    loco_makePointerArray(0x0050C678, 1, "_waterObjects");
    loco_makePointerArray(0x0050C67C, 32, "_landObjects");
    loco_makePointerArray(0x0050C6FC, 1, "_townNamesObjects");
    loco_makePointerArray(0x0050C700, 32, "_cargoObjects");
    loco_makePointerArray(0x0050C780, 32, "_wallObjects");
    loco_makePointerArray(0x0050C800, 16, "_trainSignalObjects");
    loco_makePointerArray(0x0050C840, 4, "_levelCrossingObjects");
    loco_makePointerArray(0x0050C850, 1, "_streetLightObjects");
    loco_makePointerArray(0x0050C854, 16, "_tunnelObjects");
    loco_makePointerArray(0x0050C894, 8, "_bridgeObjects");
    loco_makePointerArray(0x0050C8B4, 16, "_trainStationObjects");
    loco_makePointerArray(0x0050C8F4, 8, "_trackExtraObjects");
    loco_makePointerArray(0x0050C914, 8, "_trackObjects");
    loco_makePointerArray(0x0050C934, 16, "_roadStationObjects");
    loco_makePointerArray(0x0050C974, 4, "_roadExtraObjects");
    loco_makePointerArray(0x0050C984, 8, "_roadObjects");
    loco_makePointerArray(0x0050C9A4, 8, "_airportObjects");
    loco_makePointerArray(0x0050C9C4, 8, "_dockObjects");
    loco_makePointerArray(0x0050C9E4, 224, "_vehicleObjects");
    loco_makePointerArray(0x0050CD64, 64, "_treeObjects");
    loco_makePointerArray(0x0050CE64, 1, "_snowObjects");
    loco_makePointerArray(0x0050CE68, 1, "_climateObjects");
    loco_makePointerArray(0x0050CE6C, 1, "_hillShapeObjects");
    loco_makePointerArray(0x0050CE70, 128, "_buildingObjects");
    loco_makePointerArray(0x0050D070, 1, "_scaffoldingObjects");
    loco_makePointerArray(0x0050D074, 16, "_industryObjects");
    loco_makePointerArray(0x0050D0B4, 1, "_regionObjects");
    loco_makePointerArray(0x0050D0B8, 32, "_competitorsObjects");
    loco_makePointerArray(0x0050D138, 1, "_scenarioTextObjects");
}

static loco_initDropdowns(void)
{
    MakeUnknown(0x113D850, 40 * 2, 1 | 2);
    MakeWord(0x113D850);
    MakeArray(0x113D850, 40);
    SetColor(0x113D850, CIC_ITEM, 0xFFFFFF);
    MakeName(0x113D850, "_dropdownItemFormats");

    loco_makeStubStruct("dropdown_format_args_t", 0x8);
    loco_makeStructArray(0x113D8A0, "dropdown_format_args_t", 40, "_dropdownItemFormatArgs1");
    loco_makeStructArray(0x113D9E0, "dropdown_format_args_t", 40, "_dropdownItemFormatArgs2");
}
