#include "common.idc"
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

    offset = 0x4FE2FC;
    loco_makePointerArray(offset, 34, "");
    for (i = 0; i < 34; i++) {
        list = Dword(offset + i * 4);
        set_name(list, form("_unk1_%s", loco_getObjectType(i, 0)));
        create_byte(list);
        make_array(list, loco_getObjectType(i, 1));
    }

    initObject_interfaceSkin();
    initObject_sound();
    initObject_currency();
    initObject_water();
    initObject_land();
    initObject_townNames();
    initObject_cargo();
    initObject_wall();
    initObject_trackSignal();
    initObject_bridge();
    initObject_trackStation();
    initObject_trackExtra();
    initObject_track();
    initObject_roadStation();
    initObject_roadExtra();
    initObject_road();
    initObject_airport();
    initObject_dock();
    initObject_tree();
    initObject_building();
    initObject_industry();
    initObject_competitors();
    initObject_scenarioText();
}

static initObject_interfaceSkin(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x00, 0) + "_t", 0x18);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U32, "img");
    loco_setStructFld(id, 0x06, U8, "colour_06");
    loco_setStructFld(id, 0x07, U8, "colour_07");
    loco_setStructFld(id, 0x08, U8, "colour_08");
    loco_setStructFld(id, 0x09, U8, "colour_09");
    loco_setStructFld(id, 0x0A, U8, "colour_0A");
    loco_setStructFld(id, 0x0B, U8, "colour_0B");
    loco_setStructFld(id, 0x0C, U8, "colour_0C");
    loco_setStructFld(id, 0x10, U8, "colour_10");
    loco_setStructFld(id, 0x12, U8, "colour_12");
    loco_setStructFld(id, 0x13, U8, "colour_13");
    loco_setStructFld(id, 0x14, U8, "colour_14");
    loco_setStructFld(id, 0x15, U8, "colour_15");
    loco_setStructFld(id, 0x16, U8, "colour_16");
    loco_setStructFld(id, 0x17, U8, "colour_17");

    // sort on
    op_stroff(0x004291E9, 1, id, 0);
    op_stroff(0x0042A53B, 1, id, 0);
    op_stroff(0x0042ABB8, 1, id, 0);
    op_stroff(0x0042ABEB, 1, id, 0);
    op_stroff(0x00431C43, 1, id, 0);
    op_stroff(0x00432833, 1, id, 0);
    op_stroff(0x00434435, 1, id, 0);
    op_stroff(0x00434467, 1, id, 0);
    op_stroff(0x004344A2, 1, id, 0);
    op_stroff(0x004344D5, 1, id, 0);
    op_stroff(0x00434505, 1, id, 0);
    op_stroff(0x00434541, 1, id, 0);
    op_stroff(0x00434829, 1, id, 0);
    op_stroff(0x00434F95, 1, id, 0);
    op_stroff(0x00435CEB, 1, id, 0);
    op_stroff(0x00435CF4, 1, id, 0);
    op_stroff(0x0043765C, 1, id, 0);
    op_stroff(0x0043768F, 1, id, 0);
    op_stroff(0x004376CE, 1, id, 0);
    op_stroff(0x0043770D, 1, id, 0);
    op_stroff(0x0043774C, 1, id, 0);
    op_stroff(0x004377F6, 1, id, 0);
    op_stroff(0x00438BA3, 1, id, 0);
    op_stroff(0x00438BAC, 1, id, 0);
    op_stroff(0x00438BB5, 1, id, 0);
    op_stroff(0x00438BBE, 1, id, 0);
    op_stroff(0x00438C14, 1, id, 0);
    op_stroff(0x00438C94, 1, id, 0);
    op_stroff(0x00438CF6, 1, id, 0);
    op_stroff(0x00438D01, 1, id, 0);
    op_stroff(0x00438EB7, 1, id, 0);
    op_stroff(0x004398E5, 1, id, 0);
    op_stroff(0x00439BE0, 1, id, 0);
    op_stroff(0x00439C26, 1, id, 0);
    op_stroff(0x00439C33, 1, id, 0);
    op_stroff(0x00439C40, 1, id, 0);
    op_stroff(0x00439C4D, 1, id, 0);
    op_stroff(0x00439C5A, 1, id, 0);
    op_stroff(0x00439C67, 1, id, 0);
    op_stroff(0x00439C88, 1, id, 0);
    op_stroff(0x00439C95, 1, id, 0);
    op_stroff(0x00439CA6, 1, id, 0);
    op_stroff(0x00439CB3, 1, id, 0);
    op_stroff(0x00439CC4, 1, id, 0);
    op_stroff(0x00439E5A, 1, id, 0);
    op_stroff(0x00439F17, 1, id, 0);
    op_stroff(0x00439F8E, 1, id, 0);
    op_stroff(0x00439FB5, 1, id, 0);
    op_stroff(0x0043A027, 1, id, 0);
    op_stroff(0x0043B2D1, 1, id, 0);
    op_stroff(0x0043B2DA, 1, id, 0);
    op_stroff(0x0043B50D, 1, id, 0);
    op_stroff(0x0043B516, 1, id, 0);
    op_stroff(0x0043C82D, 1, id, 0);
    op_stroff(0x0043C837, 0, id, 0);
    op_stroff(0x0043C83F, 0, id, 0);
    op_stroff(0x0043C853, 0, id, 0);
    op_stroff(0x0043C858, 0, id, 0);
    op_stroff(0x0043C86D, 1, id, 0);
    op_stroff(0x0043D3D6, 1, id, 0);
    op_stroff(0x0043D3FA, 1, id, 0);
    op_stroff(0x004BE8CA, 1, id, 0);
    op_stroff(0x004BE8D3, 1, id, 0);
    op_stroff(0x004C2A93, 1, id, 0);
    op_stroff(0x004C2C22, 1, id, 0);
    op_stroff(0x004C2C64, 1, id, 0);
    op_stroff(0x004C2CA6, 1, id, 0);
    op_stroff(0x004C2CE8, 1, id, 0);
    op_stroff(0x004C2D2A, 1, id, 0);
    op_stroff(0x004C2D6C, 1, id, 0);
    op_stroff(0x004C93D5, 1, id, 0);
    op_stroff(0x004CE66E, 1, id, 0);
    op_stroff(0x004CE677, 1, id, 0);
    op_stroff(0x004CE696, 1, id, 0);
    op_stroff(0x004CEFD5, 1, id, 0);
    op_stroff(0x004CEFDE, 1, id, 0);
    // sort off
}

static initObject_sound(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x01, 0) + "_t", 0x0C);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructVar(id, 0x02, U32);
    loco_setStructVar(id, 0x06, U8);
    loco_setStructVar(id, 0x08, U32);

    // sort on
    op_stroff(0x00489E57, 1, id, 0);
    op_stroff(0x0048A0C0, 1, id, 0);
    op_stroff(0x0048A554, 1, id, 0);
    op_stroff(0x0048AFAF, 1, id, 0);
    op_stroff(0x0048AFB9, 0, id, 0);
    op_stroff(0x0048AFCB, 0, id, 0);
    op_stroff(0x0048AFE1, 0, id, 0);
    op_stroff(0x0048AFE6, 0, id, 0);
    op_stroff(0x0048B00E, 1, id, 0);
    // sort off
}

static initObject_currency(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x02, 0) + "_t", 0x0C);
    loco_setStructFld(id, 0x00, U16, "name");
    loco_setStructFld(id, 0x02, U16, "prefix_symbol");
    loco_setStructFld(id, 0x04, U16, "suffix_symbol");
    loco_setStructFld(id, 0x06, U32, "img");
    loco_setStructFld(id, 0x0A, U8, "separator");
    loco_setStructFld(id, 0x0B, U8, "factor");

    // sort on
    op_stroff(0x0046DF56, 1, id, 0);
    op_stroff(0x0046DF60, 0, id, 0);
    op_stroff(0x0046DF69, 0, id, 0);
    op_stroff(0x0046DF73, 0, id, 0);
    op_stroff(0x0046DF7C, 0, id, 0);
    op_stroff(0x0046DF90, 0, id, 0);
    op_stroff(0x0046DF95, 0, id, 0);
    op_stroff(0x0046DF9B, 0, id, 0);
    op_stroff(0x0046DFA1, 0, id, 0);
    op_stroff(0x0046DFA9, 0, id, 0);
    op_stroff(0x0046DFAF, 0, id, 0);
    op_stroff(0x0046DFC3, 1, id, 0);
    // sort off
}

static initObject_water(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x05, 0) + "_t", 0x07);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U8, "cost_index");
    loco_setStructFld(id, 0x04, U8, "cost_factor");
}

static initObject_land(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x06, 0) + "_t", 0x1A);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U8, "cost_index");
    loco_setStructFld(id, 0x08, U8, "cost_factor");
}

static initObject_townNames(void)
{
    auto id, i;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x07, 0) + "_t", 0x1A);
    loco_setStructFld(id, 0x00, U16, "str");
    for (i = 0; i < 6; i++) {
        loco_setStructFld(id, i * 4 + 0x02, U8, sprintf("g%d_count", i));
        loco_setStructFld(id, i * 4 + 0x03, U8, sprintf("g%d_fill", i));
        loco_setStructFld(id, i * 4 + 0x04, U16, sprintf("g%d_offset", i));
    }

    // sort on
    op_stroff(0x00497A7D, 1, id, 0);
    op_stroff(0x00497A87, 1, id, 0);
    op_stroff(0x00497A9B, 1, id, 0);
    op_stroff(0x00497AAC, 1, id, 0);
    op_stroff(0x00497AC0, 1, id, 0);
    op_stroff(0x00497ACA, 1, id, 0);
    op_stroff(0x00497ADE, 1, id, 0);
    op_stroff(0x00497AEF, 1, id, 0);
    op_stroff(0x00497B02, 1, id, 0);
    op_stroff(0x00497B0C, 1, id, 0);
    op_stroff(0x00497B20, 1, id, 0);
    op_stroff(0x00497B31, 1, id, 0);
    op_stroff(0x00497B44, 1, id, 0);
    op_stroff(0x00497B4E, 1, id, 0);
    op_stroff(0x00497B62, 1, id, 0);
    op_stroff(0x00497B73, 1, id, 0);
    op_stroff(0x00497B86, 1, id, 0);
    op_stroff(0x00497B90, 1, id, 0);
    op_stroff(0x00497BA4, 1, id, 0);
    op_stroff(0x00497BB5, 1, id, 0);
    op_stroff(0x00497BC8, 1, id, 0);
    op_stroff(0x00497BD2, 1, id, 0);
    op_stroff(0x00497BE6, 1, id, 0);
    op_stroff(0x00497BF7, 1, id, 0);
    op_stroff(0x00498E1D, 1, id, 0);
    op_stroff(0x00498E27, 0, id, 0);
    op_stroff(0x00498E3B, 0, id, 0);
    op_stroff(0x00498E46, 1, id, 0);
    op_stroff(0x00498E51, 1, id, 0);
    op_stroff(0x00498E5C, 1, id, 0);
    op_stroff(0x00498E67, 1, id, 0);
    op_stroff(0x00498E72, 1, id, 0);
    op_stroff(0x00498E7D, 1, id, 0);
    // sort off
}

static initObject_cargo(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x08, 0) + "_t", 0x10);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x06, U16, "units_and_cargo_name");
    loco_setStructFld(id, 0x08, U16, "unit_name_singular");
    loco_setStructFld(id, 0x0A, U16, "unit_name_plural");
    loco_setStructFld(id, 0x0C, U32, "sprite");
}

static initObject_wall(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x09, 0) + "_t", 0x0A);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U32, "sprite");
    loco_setStructFld(id, 0x07, U8, "flags");
}

static initObject_trackSignal(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x0A, 0) + "_t", 0x1D);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "track_side");
    loco_setStructFld(id, 0x05, U8, "num_frames");
    loco_setStructFld(id, 0x06, U16, "cost_factor");
    loco_setStructFld(id, 0x08, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x0A, U8, "cost_index");
    loco_setStructFld(id, 0x12, U8, "num_compatible");
    loco_setStructFldArray(id, 0x13, U8, "mods", 7);
    loco_setStructFld(id, 0x1A, U16, "designed_year");
    loco_setStructFld(id, 0x1C, U16, "obsolete_year");
}

static initObject_bridge(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x0E, 0) + "_t", 0x2B);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U8, "no_roof");
    loco_setStructFld(id, 0x08, U8, "span_length");
    loco_setStructFld(id, 0x09, U8, "pillar_spacing");
    loco_setStructFld(id, 0x0A, U16, "max_speed");
    loco_setStructFld(id, 0x0C, U8, "max_height");
    loco_setStructFld(id, 0x0D, U8, "cost_index");
    loco_setStructFld(id, 0x0E, U16, "base_cost_factor");
    loco_setStructFld(id, 0x10, U16, "height_cost_factor");
    loco_setStructFld(id, 0x12, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x14, U16, "disabled_track_cfg");
    loco_setStructFld(id, 0x1A, U8, "track_num_compatible");
    loco_setStructFldArray(id, 0x1B, U8, "track_mods", 7);
    loco_setStructFld(id, 0x22, U8, "road_num_compatible");
    loco_setStructFldArray(id, 0x23, U8, "road_mods", 7);
    loco_setStructFld(id, 0x2A, U16, "designed_year");
}

static initObject_trackStation(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x0F, 0) + "_t", 0xAC);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x04, U16, "track_pieces");
    loco_setStructFld(id, 0x06, U16, "build_cost_factor");
    loco_setStructFld(id, 0x08, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x0A, U8, "cost_index");
    loco_setStructFld(id, 0x0C, U8, "flags");
    loco_setStructFld(id, 0x22, U8, "num_compatible");
    loco_setStructFldArray(id, 0x23, U8, "mods", 7);
    loco_setStructFld(id, 0x2A, U16, "designed_year");
    loco_setStructFld(id, 0x2C, U16, "obsolete_year");
}

static initObject_trackExtra(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x10, 0) + "_t", 0x12);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "track_pieces");
    loco_setStructFld(id, 0x04, U8, "is_overhead");
    loco_setStructFld(id, 0x05, U8, "cost_index");
    loco_setStructFld(id, 0x06, U16, "build_cost_factor");
    loco_setStructFld(id, 0x08, U16, "sell_cost_factor");
}

static initObject_track(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x11, 0) + "_t", 0x35);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "track_pieces");
    loco_setStructFld(id, 0x04, U16, "station_track_pieces");
    loco_setStructFld(id, 0x07, U8, "num_compatible");
    loco_setStructFld(id, 0x08, U8, "num_mods");
    loco_setStructFld(id, 0x09, U8, "num_signals");
    loco_setStructFldArray(id, 0x0A, U8, "mods", 4);
    loco_setStructFld(id, 0x14, U16, "build_cost_factor");
    loco_setStructFld(id, 0x16, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x18, U16, "tunel_cost_factor");
    loco_setStructFld(id, 0x1A, U8, "cost_index");
    loco_setStructFld(id, 0x1C, U16, "curve_speed");
    loco_setStructFld(id, 0x22, U16, "flags");
    loco_setStructFld(id, 0x24, U8, "num_bridges");
    loco_setStructFldArray(id, 0x25, U8, "briges", 7);
    loco_setStructFld(id, 0x2C, U8, "num_stations");
    loco_setStructFldArray(id, 0x2D, U8, "stations", 7);
    loco_setStructFld(id, 0x34, U8, "display_offset");
}

static initObject_roadStation(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x12, 0) + "_t", 0x6E);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x04, U16, "road_pieces");
    loco_setStructFld(id, 0x06, U16, "build_cost_factor");
    loco_setStructFld(id, 0x08, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x0A, U8, "cost_index");
    loco_setStructFld(id, 0x0B, U8, "flags");
    loco_setStructFld(id, 0x20, U8, "num_compatible");
    loco_setStructFldArray(id, 0x21, U8, "mods", 7);
    loco_setStructFld(id, 0x28, U16, "designed_year");
    loco_setStructFld(id, 0x2A, U16, "obsolete_year");
}

static initObject_roadExtra(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x13, 0) + "_t", 0x12);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "road_pieces");
    loco_setStructFld(id, 0x04, U8, "is_overhead");
    loco_setStructFld(id, 0x05, U8, "cost_index");
    loco_setStructFld(id, 0x06, U16, "build_cost_factor");
    loco_setStructFld(id, 0x08, U16, "sell_cost_factor");
}

static initObject_road(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x14, 0) + "_t", 0x30);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "road_pieces");
    loco_setStructFld(id, 0x04, U16, "build_cost_factor");
    loco_setStructFld(id, 0x06, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x08, U16, "tunnel_cost_factor");
    loco_setStructFld(id, 0x0A, U8, "cost_index");
    loco_setStructFld(id, 0x12, U16, "flags");
    loco_setStructFld(id, 0x14, U8, "num_bridges");
    loco_setStructFldArray(id, 0x15, U8, "bridges", 7);
    loco_setStructFld(id, 0x1C, U8, "num_stations");
    loco_setStructFldArray(id, 0x1D, U8, "stations", 7);
    loco_setStructFld(id, 0x25, U8, "num_mods");
    loco_setStructFldArray(id, 0x26, U8, "mods", 2);
    loco_setStructFld(id, 0x28, U8, "num_compatible");
}

static initObject_airport(void)
{
    auto id;

    id = loco_makeStubStruct("airport_var_AE_object", 0x7);
    loco_setStructFld(id, 0x00, U16, "x");
    loco_setStructFld(id, 0x02, U16, "y");
    loco_setStructFld(id, 0x04, U16, "z");
    loco_setStructFld(id, 0x06, U16, "flags");

    id = loco_makeStubStruct("airport_var_B2_object", 0xC);

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x15, 0) + "_t", 0xBA);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "build_cost_factor");
    loco_setStructFld(id, 0x04, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x06, U8, "cost_index");
    loco_setStructFld(id, 0x10, U16, "allowed_plane_types");
    loco_setStructFld(id, 0x12, U8, "num_sprite_sets");
    loco_setStructFld(id, 0x13, U8, "num_tiles");
    loco_setStructFld(id, 0xA0, U32, "large_tiles");
    loco_setStructFld(id, 0xA4, U8, "min_x");
    loco_setStructFld(id, 0xA5, U8, "min_y");
    loco_setStructFld(id, 0xA6, U8, "max_x");
    loco_setStructFld(id, 0xA7, U8, "max_y");
    loco_setStructFld(id, 0xA8, U16, "designed_year");
    loco_setStructFld(id, 0xAA, U16, "obsolete_year");
    loco_setStructFld(id, 0xAC, U8, "num_nodes");
    loco_setStructFld(id, 0xAD, U8, "num_edges");
    loco_setStructFld(id, 0xAE, U32, "struct_AE_ptr");
    loco_setStructFld(id, 0xB2, U32, "struct_B2_ptr");
}

static initObject_dock(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x16, 0) + "_t", 0x28);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "build_cost_factor");
    loco_setStructFld(id, 0x04, U16, "sell_cost_factor");
    loco_setStructFld(id, 0x06, U8, "cost_index");
    loco_setStructFld(id, 0x12, U8, "num_aux_01");
    loco_setStructFld(id, 0x13, U8, "num_aux_02_ent");
    loco_setStructFld(id, 0x20, U16, "designed_year");
    loco_setStructFld(id, 0x22, U16, "obsolete_year");
}

static initObject_tree(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x18, 0) + "_t", 0x4C);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x03, U8, "height");
    loco_setStructFld(id, 0x06, U8, "num_rotations");
    loco_setStructFld(id, 0x07, U8, "growth");
    loco_setStructFldArray(id, 0x0A, U32, "sprites", 12);
    loco_setStructFld(id, 0x3D, U8, "season_state");
    loco_setStructFld(id, 0x3F, U8, "cost_index");
    loco_setStructFld(id, 0x40, U16, "build_cost_factor");
    loco_setStructFld(id, 0x42, U16, "clear_cost_factor");
    loco_setStructFld(id, 0x44, U32, "colours");
}

static initObject_building(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x1C, 0) + "_t", 0xA9);
    loco_setStructFld(id, 0x00, U16, "name");
    loco_setStructFld(id, 0x90, U32, "colours");
    loco_setStructFld(id, 0x98, U8, "flags");
    loco_setStructFldArray(id, 0xA2, U8, "producedCargoType", 2);
}

static initObject_industry(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x1E, 0) + "_t", 0xF1);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x0A, U16, "nameSingular");
    loco_setStructFld(id, 0x0C, U16, "namePlural");
    loco_setStructFld(id, 0xCA, U16, "designed");
    loco_setStructFld(id, 0xCC, U16, "obsolete");
    loco_setStructFld(id, 0xCF, U8, "cost_index");
    loco_setStructFld(id, 0xD0, U16, "cost_factor");
    loco_setStructFldArray(id, 0xDE, U8, "produced_cargo_type", 2);
    loco_setStructFldArray(id, 0xE0, U8, "received_cargo_type", 3);
    loco_setStructFld(id, 0xE4, U32, "flags");
}

static initObject_competitors(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x20, 0) + "_t", 0x38);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "str2");
    loco_setStructVar(id, 0x04, U32);
    loco_setStructVar(id, 0x08, U32);
    loco_setStructFld(id, 0x0C, U32, "emotions");
    loco_setStructFldArray(id, 0x10, U32, "imgs", 9);
    loco_setStructFld(id, 0x34, U8, "intelligence");
    loco_setStructFld(id, 0x35, U8, "agressiveness");
    loco_setStructFld(id, 0x36, U8, "competitiveness");

    // sort on
    op_stroff(0x0042A175, 1, id, 0);
    op_stroff(0x0042A2D9, 1, id, 0);
    op_stroff(0x0042FE7C, 1, id, 0);
    op_stroff(0x0042FEBB, 1, id, 0);
    op_stroff(0x0042FED0, 1, id, 0);
    op_stroff(0x0042FEDF, 1, id, 0);
    op_stroff(0x0042FEF4, 1, id, 0);
    op_stroff(0x00431DA8, 1, id, 0);
    op_stroff(0x0043207F, 1, id, 0);
    op_stroff(0x00432123, 1, id, 0);
    op_stroff(0x00432943, 1, id, 0);
    op_stroff(0x00434CA0, 1, id, 0);
    op_stroff(0x00434CAA, 0, id, 0);
    op_stroff(0x00434CB3, 0, id, 0);
    op_stroff(0x00434CBF, 0, id, 0);
    op_stroff(0x00434CCB, 0, id, 0);
    op_stroff(0x00434CD1, 0, id, 0);
    op_stroff(0x00434CEF, 1, id, 0);
    op_stroff(0x00434CF7, 1, id, 0);
    op_stroff(0x00434CFF, 1, id, 0);
    op_stroff(0x00434D08, 0, id, 0);
    op_stroff(0x00434D0D, 0, id, 0);
    op_stroff(0x00434D15, 0, id, 0);
    op_stroff(0x00434D2D, 0, id, 0);
    op_stroff(0x00434D33, 0, id, 0);
    op_stroff(0x00434D39, 0, id, 0);
    op_stroff(0x00434D3F, 0, id, 0);
    op_stroff(0x00434D45, 0, id, 0);
    op_stroff(0x00434D4B, 0, id, 0);
    op_stroff(0x00434D8C, 1, id, 0);
    op_stroff(0x00434DAA, 1, id, 0);
    op_stroff(0x00434DDC, 1, id, 0);
    op_stroff(0x00434E0E, 1, id, 0);
    op_stroff(0x004356B1, 1, id, 0);
    op_stroff(0x00435F61, 1, id, 0);
    op_stroff(0x0043AAD2, 1, id, 0);
    op_stroff(0x0048136A, 1, id, 0);
    op_stroff(0x0048254E, 1, id, 0);
    op_stroff(0x004869E5, 1, id, 0);
    op_stroff(0x004CF335, 1, id, 0);
    // sort off
}

static initObject_scenarioText(void)
{
    auto id;

    id = loco_makeStubStruct("obj_" + loco_getObjectType(0x21, 0) + "_t", 0x06);
    loco_setStructFld(id, 0x00, U16, "str");
    loco_setStructFld(id, 0x02, U16, "str2");

    // sort on
    op_stroff(0x0043EDE3, 1, id, 0);
    op_stroff(0x0043EDED, 0, id, 0);
    op_stroff(0x0043EDF6, 0, id, 0);
    op_stroff(0x0043EE0B, 0, id, 0);
    op_stroff(0x0043EE10, 0, id, 0);
    op_stroff(0x0043F038, 1, id, 0);
    op_stroff(0x0043F0E1, 1, id, 0);
    op_stroff(0x00440178, 1, id, 0);
    op_stroff(0x004441B0, 1, id, 0);
    // sort off
}
