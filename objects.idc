#include <idc.idc>

static get2(index, a, b)
{
    if (index == 0)
        return a;
    if (index == 1)
        return b;
}

static loco_getObjectType(value, i)
{
    if (value == 0x00)
        return get2(i, "interface_skin", 1);
    if (value == 0x01)
        return get2(i, "sound", 128);
    if (value == 0x02)
        return get2(i, "currency", 1);
    if (value == 0x03)
        return get2(i, "steam", 32);
    if (value == 0x04)
        return get2(i, "rock", 8);
    if (value == 0x05)
        return get2(i, "water", 1);
    if (value == 0x06)
        return get2(i, "land", 32);
    if (value == 0x07)
        return get2(i, "town_names", 1);
    if (value == 0x08)
        return get2(i, "cargo", 32);
    if (value == 0x09)
        return get2(i, "wall", 32);
    if (value == 0x0A)
        return get2(i, "track_signal", 16);
    if (value == 0x0B)
        return get2(i, "level_crossing", 4);
    if (value == 0x0C)
        return get2(i, "street_light", 1);
    if (value == 0x0D)
        return get2(i, "tunnel", 16);
    if (value == 0x0E)
        return get2(i, "bridge", 8);
    if (value == 0x0F)
        return get2(i, "track_station", 16);
    if (value == 0x10)
        return get2(i, "track_extra", 8);
    if (value == 0x11)
        return get2(i, "track", 8);
    if (value == 0x12)
        return get2(i, "road_station", 16);
    if (value == 0x13)
        return get2(i, "road_extra", 4);
    if (value == 0x14)
        return get2(i, "road", 8);
    if (value == 0x15)
        return get2(i, "airport", 8);
    if (value == 0x16)
        return get2(i, "dock", 8);
    if (value == 0x17)
        return get2(i, "vehicle", 224);
    if (value == 0x18)
        return get2(i, "tree", 64);
    if (value == 0x19)
        return get2(i, "snow", 1);
    if (value == 0x1A)
        return get2(i, "climate", 1);
    if (value == 0x1B)
        return get2(i, "hill_shapes", 1);
    if (value == 0x1C)
        return get2(i, "building", 128);
    if (value == 0x1D)
        return get2(i, "scaffolding", 1);
    if (value == 0x1E)
        return get2(i, "industry", 16);
    if (value == 0x1F)
        return get2(i, "region", 1);
    if (value == 0x20)
        return get2(i, "competitors", 32);
    if (value == 0x21)
        return get2(i, "scenario_text", 1);
}

static camelize(input)
{
    auto output, pos;

    output = input;

    while ((pos = strstr(output, "_")) != -1) {
        auto chr;
        chr = strstr("abcdefghijklmnopqrstuvwxyz", output[pos + 1]);
        chr = substr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", chr, chr + 1);
        output [pos:pos + 2] = chr;
    }

    return output;
}

static initObjects(void)
{
    auto id, offset, i, fn, list;

    id = loco_makeStubStruct("pair_ptr", 0x8);
    AddStrucMember(id, "a", 0, FF_DWRD | FF_DATA | FF_0OFF, 0, 4);
    DelStrucMember(id, 0x7);
    AddStrucMember(id, "b", 0x4, FF_DWRD | FF_DATA | FF_0OFF, 0, 4);

    offset = 0x004FE1C8;
    loco_makePointerArray(offset, 34, "paint_entry_table");

    for (i = 0; i < 34; i++) {
        fn = Dword(offset + i * 4);
        MakeName(fn, form("obj_%s", loco_getObjectType(i, 0)));
    }

    offset = 0x004FE0B8;
    loco_makeStructArray(offset, "pair_ptr", 34, "object_repository");

    for (i = 0; i < 34; i++) {
        auto name;

        list = Dword(offset + i * 8);
        name = form("_%sObjects", camelize(loco_getObjectType(i, 0)));
        loco_makePointerArray(list, loco_getObjectType(i, 1), name);

        list = Dword(offset + i * 8 + 4);
        name = form("_%sMeta", camelize(loco_getObjectType(i, 0)));

        create_byte(list);
        make_array(list, 0x14 * loco_getObjectType(i, 1));
        MakeName(list, name);
    }

    offset = 0x004FE250;
    set_name(offset, "object_entry_group_counts");
    create_word(offset);
    make_array(offset, 34);

    offset = 0x4FE294;
    create_word(offset);
    make_array(offset, 34);

    offset = 0x4FE2D8;
    create_byte(offset);
    make_array(offset, 34);

    offset = 0x4FE2FC;
    loco_makePointerArray(offset, 34, "");
    for (i = 0; i < 34; i++) {
        list = Dword(offset + i * 4);
        set_name(list, form("_unk1_%s", loco_getObjectType(i, 0)));
        create_byte(list);
        make_array(list, loco_getObjectType(i, 1));
    }

    offset = 0x4FE384;
    create_byte(offset);
    op_bin(offset, -1);
    make_array(offset, 34);
}
