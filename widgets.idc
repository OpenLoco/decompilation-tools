#include <idc.idc>

static LocoWidgetList(enumWidget, addr, name)
{
    auto i, type;

    i = 0;
    while (1) {
        type = Byte(addr + i * 0x10);
        if (type == 0x1e) {
            MakeByte(addr + i * 0x10);
            OpEnumEx(addr + i * 0x10, 0, enumWidget, 0);
            SetColor(addr + i * 0x10, CIC_ITEM, 0xFFFFFF);
            break;
        }

        i++;
    }

    MakeUnknown(addr, i * 0x10, 1 | 2);
    MakeStruct(addr, "widget_t");
    MakeName(addr, name);
    MakeArray(addr, i);
    SetColor(addr, CIC_ITEM, 0xFFFFFF);
}

static LocoWidgetSet(enumWidget, layoutAddr, eventAddr, name)
{
    LocoWidgetList(enumWidget, layoutAddr, form("%s::_widgets", name));
    LocoEvents(eventAddr, name, "", form("%s::_events", name));
}

static getEventName(event)
{
    if (event == 0) {
        return "on_close";
    }

    if (event == 1) {
        return "on_mouse_up";
    }

    if (event == 2) {
        return "on_resize";
    }

    if (event == 4) {
        return "on_mouse_down";
    }

    if (event == 5) {
        return "on_dropdown";
    }

    if (event == 7) {
        return "on_update";
    }

    if (event == 14) {
        return "tool_abort";
    }

    if (event == 16) {
        return "get_scroll_size";
    }

    if (event == 17) {
        return "scroll_mouse_down";
    }

    if (event == 20) {
        return "text_input";
    }

    if (event == 23) {
        return "tooltip";
    }

    if (event == 24) {
        return "cursor";
    }

    if (event == 26) {
        return "invalidate";
    }

    if (event == 27) {
        return "draw";
    }

    if (event == 28) {
        return "scroll_paint";
    }

    return form("%d", event);
}

static IsNullSub(addr)
{
    auto chr;

    chr = Byte(addr);
    if (chr == 0xC3 || chr == 0xCB) {
        return 1;
    }

    return 0;
}

static LocoEvents(addr, prefix, suffix, basename)
{
    auto i, offset;

    MakeUnknown(addr, 29 * 4, 1 | 2);
    MakeStruct(addr, "window_events_t");
    SetColor(addr, CIC_ITEM, 0xFFFFFF);
    MakeName(addr, basename);

    for (i = 0; i <= 28; i++) {
        offset = Dword(addr + i * 4);
        if (IsNullSub(offset)) {
            MakeName(offset, form("nullsub_%x", offset)); // SN_AUTO | SN_NOWARN
            continue;
        }
        MakeName(offset, form("%s::event%s_%s", prefix, suffix, getEventName(i)));
    }
}

static LocoWidgetArray(enumWidget, addr, addr2, name, count)
{
    auto i, widgets, events;

    OpOff(addr, 0, 0);
    MakeArray(addr, count);
    SetColor(addr, CIC_ITEM, 0xFFFFFF);

    OpOff(addr2, 0, 0);
    MakeArray(addr2, count);
    SetColor(addr2, CIC_ITEM, 0xFFFFFF);

    for (i = 0; i < count; i++) {
        widgets = Dword(addr + i * 4);
        LocoWidgetList(enumWidget, widgets, form("%s::_widgets_%d", name, i + 1));

        events = Dword(addr2 + i * 4);
        LocoEvents(events, name, form("_%d", i + 1), form("%s::_events_%d", name, i + 1));
    }
}

static loco_StructOffsetOp(id, address)
{
    MakeCode(address);
    OpStroffEx(address, 0, id, 0);
}

static loco_initEnumMember(id, prefix, name, val, mask)
{
    auto res;

    name = form("%s%s", prefix, name);

    res = GetConstByName(name);
    if (res != -1) {
        return;
    }

    res = AddConstEx(id, name, val, mask);
    Message("%d\n", res);
}

static loco_initEnum(id, prefix)
{
    loco_initEnumMember(id, prefix, "main", 0, 0b1111111);
    loco_initEnumMember(id, prefix, "toolbar_top", 1, 0b1111111);
    loco_initEnumMember(id, prefix, "toolbar_player", 2, 0b1111111);
    loco_initEnumMember(id, prefix, "toolbar_time", 3, 0b1111111);
    loco_initEnumMember(id, prefix, "toolbar_bottom", 4, 0b1111111);
    loco_initEnumMember(id, prefix, "tooltip", 6, 0b1111111);
    loco_initEnumMember(id, prefix, "dropdown", 7, 0b1111111);
    loco_initEnumMember(id, prefix, "about", 9, 0b1111111);
    loco_initEnumMember(id, prefix, "about_atari", 10, 0b1111111);
    loco_initEnumMember(id, prefix, "about_music", 11, 0b1111111);
    loco_initEnumMember(id, prefix, "construction", 13, 0b1111111);
    loco_initEnumMember(id, prefix, "prompt_savegame", 14, 0b1111111);
    loco_initEnumMember(id, prefix, "terraform", 15, 0b1111111);
    loco_initEnumMember(id, prefix, "title_menu", 16, 0b1111111);
    loco_initEnumMember(id, prefix, "title_exit", 17, 0b1111111);
    loco_initEnumMember(id, prefix, "scenario_select", 18, 0b1111111);
    loco_initEnumMember(id, prefix, "keyboard_shortcuts", 19, 0b1111111);
    loco_initEnumMember(id, prefix, "keyboard_shortcut_edit", 20, 0b1111111);
    loco_initEnumMember(id, prefix, "map", 21, 0b1111111);
    loco_initEnumMember(id, prefix, "title_logo", 22, 0b1111111);
    loco_initEnumMember(id, prefix, "vehicle", 23, 0b1111111);
    loco_initEnumMember(id, prefix, "station", 24, 0b1111111);
    loco_initEnumMember(id, prefix, "drag_vehicle_part", 25, 0b1111111);
    loco_initEnumMember(id, prefix, "company", 26, 0b1111111);
    loco_initEnumMember(id, prefix, "vehicle_list", 27, 0b1111111);
    loco_initEnumMember(id, prefix, "build_vehicle", 28, 0b1111111);
    loco_initEnumMember(id, prefix, "station_list", 29, 0b1111111);
    loco_initEnumMember(id, prefix, "object_selection", 31, 0b1111111);
    loco_initEnumMember(id, prefix, "town_list", 32, 0b1111111);
    loco_initEnumMember(id, prefix, "town", 33, 0b1111111);
    loco_initEnumMember(id, prefix, "industry", 34, 0b1111111);
    loco_initEnumMember(id, prefix, "industry_list", 35, 0b1111111);
    loco_initEnumMember(id, prefix, "messages", 37, 0b1111111);
    loco_initEnumMember(id, prefix, "multiplayer", 39, 0b1111111);
    loco_initEnumMember(id, prefix, "options", 40, 0b1111111);
    loco_initEnumMember(id, prefix, "music_selection", 41, 0b1111111);
    loco_initEnumMember(id, prefix, "company_face_selection", 42, 0b1111111);
    loco_initEnumMember(id, prefix, "landscape_generation", 43, 0b1111111);
    loco_initEnumMember(id, prefix, "scenario_options", 45, 0b1111111);
    loco_initEnumMember(id, prefix, "company_list", 48, 0b1111111);
    loco_initEnumMember(id, prefix, "tutorial", 49, 0b1111111);
    loco_initEnumMember(id, prefix, "prompt_confirm_displaymode", 50, 0b1111111);
    loco_initEnumMember(id, prefix, "textinput", 51, 0b1111111);
    loco_initEnumMember(id, prefix, "prompt_browse", 52, 0b1111111);
    loco_initEnumMember(id, prefix, "prompt_okcancel", 54, 0b1111111);
}

static initWidgets(void)
{
    auto id, i, enumWidget;

    id = GetEnum("windowtype");
    if (id == -1) {
        id = AddEnum(-1, "windowtype", 0);
        set_enum_bf(id, 1);
    }
    loco_initEnum(id, "wt_");

    id = GetEnum("windowtype32");
    if (id == -1) {
        id = AddEnum(-1, "windowtype32", 0x1100000);
        set_enum_bf(id, 1);
    }
    loco_initEnum(id, "wtf_");

    id = GetStrucIdByName("window_events_t");
    if (id == -1) {
        id = AddStruc(-1, "window_events_t");
        for (i = 0; i <= 28; i++) {
            AddStrucMember(id, form("event_%s", getEventName(i)), i * 4, FF_DWRD | FF_DATA | FF_0OFF, 0, 4);
        }
    } else {
        for (i = 0; i <= 28; i++) {
            SetMemberName(id, i * 4, form("event_%s", getEventName(i)));
            SetMemberType(id, i * 4, FF_DWRD | FF_DATA | FF_0OFF, 0, 1);
        }
    }

    loco_StructOffsetOp(id, 0x42A495);
    loco_StructOffsetOp(id, 0x42A49A);
    loco_StructOffsetOp(id, 0x42A80E);
    loco_StructOffsetOp(id, 0x42A813);
    loco_StructOffsetOp(id, 0x42A82F);
    loco_StructOffsetOp(id, 0x42AB73);
    loco_StructOffsetOp(id, 0x42ABFE);
    loco_StructOffsetOp(id, 0x4324AB);
    loco_StructOffsetOp(id, 0x4324B0);
    loco_StructOffsetOp(id, 0x432713);
    loco_StructOffsetOp(id, 0x432D8E);
    loco_StructOffsetOp(id, 0x433268);
    loco_StructOffsetOp(id, 0x4339A6);
    loco_StructOffsetOp(id, 0x433C86);
    loco_StructOffsetOp(id, 0x434037);
    loco_StructOffsetOp(id, 0x4340C3);
    loco_StructOffsetOp(id, 0x43426D);
    loco_StructOffsetOp(id, 0x435AEB);
    loco_StructOffsetOp(id, 0x435C75);
    loco_StructOffsetOp(id, 0x43629E);
    loco_StructOffsetOp(id, 0x4362A3);
    loco_StructOffsetOp(id, 0x4362C9);
    loco_StructOffsetOp(id, 0x43669F);
    loco_StructOffsetOp(id, 0x4369C3);
    loco_StructOffsetOp(id, 0x436CE7);
    loco_StructOffsetOp(id, 0x43700B);
    loco_StructOffsetOp(id, 0x437345);
    loco_StructOffsetOp(id, 0x437579);
    loco_StructOffsetOp(id, 0x43A5C0);
    loco_StructOffsetOp(id, 0x43DADC);
    loco_StructOffsetOp(id, 0x43DAE1);
    loco_StructOffsetOp(id, 0x43DE11);
    loco_StructOffsetOp(id, 0x43DE16);
    loco_StructOffsetOp(id, 0x43DEA6);
    loco_StructOffsetOp(id, 0x43E3E2);
    loco_StructOffsetOp(id, 0x43E8F4);
    loco_StructOffsetOp(id, 0x43EAD2);
    loco_StructOffsetOp(id, 0x43EC8B);
    loco_StructOffsetOp(id, 0x43EEF1);
    loco_StructOffsetOp(id, 0x43EEF6);
    loco_StructOffsetOp(id, 0x43F2BC);
    loco_StructOffsetOp(id, 0x43F2C1);
    loco_StructOffsetOp(id, 0x43F3F3);
    loco_StructOffsetOp(id, 0x43F8B6);
    loco_StructOffsetOp(id, 0x43FAF3);
    loco_StructOffsetOp(id, 0x44008B);
    loco_StructOffsetOp(id, 0x443EDB);
    loco_StructOffsetOp(id, 0x443EE2);
    loco_StructOffsetOp(id, 0x4468C4);
    loco_StructOffsetOp(id, 0x4468CF);
    loco_StructOffsetOp(id, 0x44713E);
    loco_StructOffsetOp(id, 0x455D69);
    loco_StructOffsetOp(id, 0x455D6E);
    loco_StructOffsetOp(id, 0x455F09);
    loco_StructOffsetOp(id, 0x45653E);
    loco_StructOffsetOp(id, 0x4565EE);
    loco_StructOffsetOp(id, 0x4569AA);
    loco_StructOffsetOp(id, 0x456C46);
    loco_StructOffsetOp(id, 0x457812);
    loco_StructOffsetOp(id, 0x45786A);
    loco_StructOffsetOp(id, 0x45786F);
    loco_StructOffsetOp(id, 0x458096);
    loco_StructOffsetOp(id, 0x45809B);
    loco_StructOffsetOp(id, 0x4580B7);
    loco_StructOffsetOp(id, 0x4586A1);
    loco_StructOffsetOp(id, 0x458B56);
    loco_StructOffsetOp(id, 0x45F1A1);
    loco_StructOffsetOp(id, 0x46B9EA);
    loco_StructOffsetOp(id, 0x46BA64);
    loco_StructOffsetOp(id, 0x46E345);
    loco_StructOffsetOp(id, 0x46E6D4);
    loco_StructOffsetOp(id, 0x46E6D9);
    loco_StructOffsetOp(id, 0x46EB77);
    loco_StructOffsetOp(id, 0x46EB7C);
    loco_StructOffsetOp(id, 0x46EC70);
    loco_StructOffsetOp(id, 0x46F2E9);
    loco_StructOffsetOp(id, 0x48E5C7);
    loco_StructOffsetOp(id, 0x48E5CC);
    loco_StructOffsetOp(id, 0x48E6FA);
    loco_StructOffsetOp(id, 0x48EBA6);
    loco_StructOffsetOp(id, 0x48EE86);
    loco_StructOffsetOp(id, 0x48F12B);
    loco_StructOffsetOp(id, 0x491002);
    loco_StructOffsetOp(id, 0x491007);
    loco_StructOffsetOp(id, 0x49182F);
    loco_StructOffsetOp(id, 0x491834);
    loco_StructOffsetOp(id, 0x491927);
    loco_StructOffsetOp(id, 0x49192C);
    loco_StructOffsetOp(id, 0x491948);
    loco_StructOffsetOp(id, 0x49926F);
    loco_StructOffsetOp(id, 0x499274);
    loco_StructOffsetOp(id, 0x499394);
    loco_StructOffsetOp(id, 0x4996E5);
    loco_StructOffsetOp(id, 0x499920);
    loco_StructOffsetOp(id, 0x499A97);
    loco_StructOffsetOp(id, 0x499C96);
    loco_StructOffsetOp(id, 0x499CEE);
    loco_StructOffsetOp(id, 0x499CF3);
    loco_StructOffsetOp(id, 0x49A488);
    loco_StructOffsetOp(id, 0x49A48D);
    loco_StructOffsetOp(id, 0x49A4A9);
    loco_StructOffsetOp(id, 0x49A7FB);
    loco_StructOffsetOp(id, 0x49AE3A);
    loco_StructOffsetOp(id, 0x49B2BA);
    loco_StructOffsetOp(id, 0x49DA29);
    loco_StructOffsetOp(id, 0x49DA2E);
    loco_StructOffsetOp(id, 0x49DA63);
    loco_StructOffsetOp(id, 0x49DA76);
    loco_StructOffsetOp(id, 0x49DA89);
    loco_StructOffsetOp(id, 0x49DA9C);
    loco_StructOffsetOp(id, 0x49DCAB);
    loco_StructOffsetOp(id, 0x49E440);
    loco_StructOffsetOp(id, 0x49E778);
    loco_StructOffsetOp(id, 0x49ECDA);
    loco_StructOffsetOp(id, 0x4A22C8);
    loco_StructOffsetOp(id, 0x4A230F);
    loco_StructOffsetOp(id, 0x4A2588);
    loco_StructOffsetOp(id, 0x4A25CF);
    loco_StructOffsetOp(id, 0x4A3D03);
    loco_StructOffsetOp(id, 0x4A3DD4);
    loco_StructOffsetOp(id, 0x4A3E39);
    loco_StructOffsetOp(id, 0x4A3E9E);
    loco_StructOffsetOp(id, 0x4A5D6B);
    loco_StructOffsetOp(id, 0x4A5D8E);
    loco_StructOffsetOp(id, 0x4A6FD0);
    loco_StructOffsetOp(id, 0x4B261F);
    loco_StructOffsetOp(id, 0x4B2624);
    loco_StructOffsetOp(id, 0x4B29BC);
    loco_StructOffsetOp(id, 0x4B2D86);
    loco_StructOffsetOp(id, 0x4B30FC);
    loco_StructOffsetOp(id, 0x4B3C81);
    loco_StructOffsetOp(id, 0x4B4610);
    loco_StructOffsetOp(id, 0x4B55DA);
    loco_StructOffsetOp(id, 0x4B599E);
    loco_StructOffsetOp(id, 0x4B5D98);
    loco_StructOffsetOp(id, 0x4B60D8);
    loco_StructOffsetOp(id, 0x4B9488);
    loco_StructOffsetOp(id, 0x4B9494);
    loco_StructOffsetOp(id, 0x4BB4B6);
    loco_StructOffsetOp(id, 0x4BB50E);
    loco_StructOffsetOp(id, 0x4BB513);
    loco_StructOffsetOp(id, 0x4BB552);
    loco_StructOffsetOp(id, 0x4BB562);
    loco_StructOffsetOp(id, 0x4BB572);
    loco_StructOffsetOp(id, 0x4BB582);
    loco_StructOffsetOp(id, 0x4BBCF7);
    loco_StructOffsetOp(id, 0x4BBCFC);
    loco_StructOffsetOp(id, 0x4BBEA9);
    loco_StructOffsetOp(id, 0x4BC341);
    loco_StructOffsetOp(id, 0x4BC4BC);
    loco_StructOffsetOp(id, 0x4BC50B);
    loco_StructOffsetOp(id, 0x4BC7AE);
    loco_StructOffsetOp(id, 0x4BCB2F);
    loco_StructOffsetOp(id, 0x4BCE9C);
    loco_StructOffsetOp(id, 0x4BF180);
    loco_StructOffsetOp(id, 0x4BF3D2);
    loco_StructOffsetOp(id, 0x4BF815);
    loco_StructOffsetOp(id, 0x4BF81A);
    loco_StructOffsetOp(id, 0x4BF82F);
    loco_StructOffsetOp(id, 0x4BFDCC);
    loco_StructOffsetOp(id, 0x4BFDD1);
    loco_StructOffsetOp(id, 0x4C01FE);
    loco_StructOffsetOp(id, 0x4C04E9);
    loco_StructOffsetOp(id, 0x4C0A40);
    loco_StructOffsetOp(id, 0x4C1093);
    loco_StructOffsetOp(id, 0x4C119E);
    loco_StructOffsetOp(id, 0x4C13A5);
    loco_StructOffsetOp(id, 0x4C19FD);
    loco_StructOffsetOp(id, 0x4C1A94);
    loco_StructOffsetOp(id, 0x4C1A99);
    loco_StructOffsetOp(id, 0x4C1B2C);
    loco_StructOffsetOp(id, 0x4C1C08);
    loco_StructOffsetOp(id, 0x4C1C0D);
    loco_StructOffsetOp(id, 0x4C1C60);
    loco_StructOffsetOp(id, 0x4C24B8);
    loco_StructOffsetOp(id, 0x4C24BD);
    loco_StructOffsetOp(id, 0x4C25EE);
    loco_StructOffsetOp(id, 0x4C25F3);
    loco_StructOffsetOp(id, 0x4C25F8);
    loco_StructOffsetOp(id, 0x4C2614);
    loco_StructOffsetOp(id, 0x4C281B);
    loco_StructOffsetOp(id, 0x4C368A);
    loco_StructOffsetOp(id, 0x4C368F);
    loco_StructOffsetOp(id, 0x4C3694);
    loco_StructOffsetOp(id, 0x4C36F4);
    loco_StructOffsetOp(id, 0x4C36F9);
    loco_StructOffsetOp(id, 0x4C36FE);
    loco_StructOffsetOp(id, 0x4C3784);
    loco_StructOffsetOp(id, 0x4C60D2);
    loco_StructOffsetOp(id, 0x4C60DC);
    loco_StructOffsetOp(id, 0x4C61C3);
    loco_StructOffsetOp(id, 0x4C72B0);
    loco_StructOffsetOp(id, 0x4C73BD);
    loco_StructOffsetOp(id, 0x4C7417);
    loco_StructOffsetOp(id, 0x4C78C2);
    loco_StructOffsetOp(id, 0x4C78C7);
    loco_StructOffsetOp(id, 0x4C7A48);
    loco_StructOffsetOp(id, 0x4C7A5D);
    loco_StructOffsetOp(id, 0x4C7D9D);
    loco_StructOffsetOp(id, 0x4C7E66);
    loco_StructOffsetOp(id, 0x4C7EE5);
    loco_StructOffsetOp(id, 0x4C80EE);
    loco_StructOffsetOp(id, 0x4C8155);
    loco_StructOffsetOp(id, 0x4C81AC);
    loco_StructOffsetOp(id, 0x4C81ED);
    loco_StructOffsetOp(id, 0x4C85CC);
    loco_StructOffsetOp(id, 0x4C86AC);
    loco_StructOffsetOp(id, 0x4C8714);
    loco_StructOffsetOp(id, 0x4C876A);
    loco_StructOffsetOp(id, 0x4C908D);
    loco_StructOffsetOp(id, 0x4C90C4);
    loco_StructOffsetOp(id, 0x4C924C);
    loco_StructOffsetOp(id, 0x4C9519);
    loco_StructOffsetOp(id, 0x4C97FB);
    loco_StructOffsetOp(id, 0x4C98C9);
    loco_StructOffsetOp(id, 0x4C98E5);
    loco_StructOffsetOp(id, 0x4C997E);
    loco_StructOffsetOp(id, 0x4C99B3);
    loco_StructOffsetOp(id, 0x4C9AEE);
    loco_StructOffsetOp(id, 0x4C9B4A);
    loco_StructOffsetOp(id, 0x4CA12B);
    loco_StructOffsetOp(id, 0x4CA19D);
    loco_StructOffsetOp(id, 0x4CB744);
    loco_StructOffsetOp(id, 0x4CC6FD);
    loco_StructOffsetOp(id, 0x4CD3F6);
    loco_StructOffsetOp(id, 0x4CD450);
    loco_StructOffsetOp(id, 0x4CD52E);
    loco_StructOffsetOp(id, 0x4CD5F8);
    loco_StructOffsetOp(id, 0x4CE433);
    loco_StructOffsetOp(id, 0x4CE8F1);
    loco_StructOffsetOp(id, 0x4CE978);
    loco_StructOffsetOp(id, 0x4CE983);

    set_cmt(0X4C1B2C, "wtf_build_vehicle", 1);
    set_cmt(0X4C1C08, "wtf_build_vehicle", 1);
    set_cmt(0X4C1C0D, "wtf_build_vehicle", 1);
    set_cmt(0X4C1C60, "wtf_build_vehicle", 1);
    set_cmt(0X4C368A, "wtf_build_vehicle", 1);
    set_cmt(0X4C368F, "wtf_build_vehicle", 1);
    set_cmt(0X4C3694, "wtf_build_vehicle", 1);
    set_cmt(0X4C36F4, "wtf_build_vehicle", 1);
    set_cmt(0X4C36F9, "wtf_build_vehicle", 1);
    set_cmt(0X4C36FE, "wtf_build_vehicle", 1);
    set_cmt(0X4C3784, "wtf_build_vehicle", 1);
    set_cmt(0X435C75, "wtf_company_list", 1);
    set_cmt(0X43629E, "wtf_company_list", 1);
    set_cmt(0X4362A3, "wtf_company_list", 1);
    set_cmt(0X4362C9, "wtf_company_list", 1);
    set_cmt(0X43669F, "wtf_company_list", 1);
    set_cmt(0X4369C3, "wtf_company_list", 1);
    set_cmt(0X436CE7, "wtf_company_list", 1);
    set_cmt(0X43700B, "wtf_company_list", 1);
    set_cmt(0X437345, "wtf_company_list", 1);
    set_cmt(0X437579, "wtf_company_list", 1);
    set_cmt(0X4324AB, "wtf_company", 1);
    set_cmt(0X4324B0, "wtf_company", 1);
    set_cmt(0X432713, "wtf_company", 1);
    set_cmt(0X432D8E, "wtf_company", 1);
    set_cmt(0X433268, "wtf_company", 1);
    set_cmt(0X4339A6, "wtf_company", 1);
    set_cmt(0X433C86, "wtf_company", 1);
    set_cmt(0X434037, "wtf_company", 1);
    set_cmt(0X4340C3, "wtf_company", 1);
    set_cmt(0X43426D, "wtf_company", 1);
    set_cmt(0X435AEB, "wtf_company", 1);
    set_cmt(0X49DA29, "wtf_construction", 1);
    set_cmt(0X49DA2E, "wtf_construction", 1);
    set_cmt(0X49DA63, "wtf_construction", 1);
    set_cmt(0X49DCAB, "wtf_construction", 1);
    set_cmt(0X49E440, "wtf_construction", 1);
    set_cmt(0X49E778, "wtf_construction", 1);
    set_cmt(0X49ECDA, "wtf_construction", 1);
    set_cmt(0X4A22C8, "wtf_construction", 1);
    set_cmt(0X4A230F, "wtf_construction", 1);
    set_cmt(0X4A2588, "wtf_construction", 1);
    set_cmt(0X4A25CF, "wtf_construction", 1);
    set_cmt(0X4A3D03, "wtf_construction", 1);
    set_cmt(0X4A3DD4, "wtf_construction", 1);
    set_cmt(0X4A3E39, "wtf_construction", 1);
    set_cmt(0X4A3E9E, "wtf_construction", 1);
    set_cmt(0X4A5D6B, "wtf_construction", 1);
    set_cmt(0X4A5D8E, "wtf_construction", 1);
    set_cmt(0X4A6FD0, "wtf_construction", 1);
    set_cmt(0X457812, "wtf_industry_list", 1);
    set_cmt(0X45786A, "wtf_industry_list", 1);
    set_cmt(0X45786F, "wtf_industry_list", 1);
    set_cmt(0X458096, "wtf_industry_list", 1);
    set_cmt(0X45809B, "wtf_industry_list", 1);
    set_cmt(0X4580B7, "wtf_industry_list", 1);
    set_cmt(0X4586A1, "wtf_industry_list", 1);
    set_cmt(0X458B56, "wtf_industry_list", 1);
    set_cmt(0X455D69, "wtf_industry", 1);
    set_cmt(0X455D6E, "wtf_industry", 1);
    set_cmt(0X455F09, "wtf_industry", 1);
    set_cmt(0X45653E, "wtf_industry", 1);
    set_cmt(0X4565EE, "wtf_industry", 1);
    set_cmt(0X4569AA, "wtf_industry", 1);
    set_cmt(0X456C46, "wtf_industry", 1);
    set_cmt(0X43DADC, "wtf_landscape_generation", 1);
    set_cmt(0X43DAE1, "wtf_landscape_generation", 1);
    set_cmt(0X43DE11, "wtf_landscape_generation", 1);
    set_cmt(0X43DE16, "wtf_landscape_generation", 1);
    set_cmt(0X43DEA6, "wtf_landscape_generation", 1);
    set_cmt(0X43E3E2, "wtf_landscape_generation", 1);
    set_cmt(0X43E8F4, "wtf_landscape_generation", 1);
    set_cmt(0X43EAD2, "wtf_landscape_generation", 1);
    set_cmt(0X43EC8B, "wtf_landscape_generation", 1);
    set_cmt(0X46B9EA, "wtf_map", 1);
    set_cmt(0X46BA64, "wtf_map", 1);
    set_cmt(0X42A495, "wtf_messages", 1);
    set_cmt(0X42A49A, "wtf_messages", 1);
    set_cmt(0X42A80E, "wtf_messages", 1);
    set_cmt(0X42A813, "wtf_messages", 1);
    set_cmt(0X42A82F, "wtf_messages", 1);
    set_cmt(0X42AB73, "wtf_messages", 1);
    set_cmt(0X42ABFE, "wtf_messages", 1);
    set_cmt(0X46E345, "wtf_multiplayer", 1);
    set_cmt(0X46E6D4, "wtf_multiplayer", 1);
    set_cmt(0X46E6D9, "wtf_multiplayer", 1);
    set_cmt(0X46EB77, "wtf_multiplayer", 1);
    set_cmt(0X46EB7C, "wtf_multiplayer", 1);
    set_cmt(0X46EC70, "wtf_multiplayer", 1);
    set_cmt(0X46F2E9, "wtf_multiplayer", 1);
    set_cmt(0X4BF815, "wtf_options", 1);
    set_cmt(0X4BF81A, "wtf_options", 1);
    set_cmt(0X4BF82F, "wtf_options", 1);
    set_cmt(0X4BFDCC, "wtf_options", 1);
    set_cmt(0X4BFDD1, "wtf_options", 1);
    set_cmt(0X4C01FE, "wtf_options", 1);
    set_cmt(0X4C04E9, "wtf_options", 1);
    set_cmt(0X4C0A40, "wtf_options", 1);
    set_cmt(0X4C1093, "wtf_options", 1);
    set_cmt(0X4C119E, "wtf_options", 1);
    set_cmt(0X4C13A5, "wtf_options", 1);
    set_cmt(0X4468C4, "wtf_prompt_browse", 1);
    set_cmt(0X4468CF, "wtf_prompt_browse", 1);
    set_cmt(0X44713E, "wtf_prompt_okcancel", 1);
    set_cmt(0X43EEF1, "wtf_scenario_options", 1);
    set_cmt(0X43EEF6, "wtf_scenario_options", 1);
    set_cmt(0X43F2BC, "wtf_scenario_options", 1);
    set_cmt(0X43F2C1, "wtf_scenario_options", 1);
    set_cmt(0X43F3F3, "wtf_scenario_options", 1);
    set_cmt(0X43F8B6, "wtf_scenario_options", 1);
    set_cmt(0X43FAF3, "wtf_scenario_options", 1);
    set_cmt(0X44008B, "wtf_scenario_options", 1);
    set_cmt(0X443EDB, "wtf_scenario_select", 1);
    set_cmt(0X443EE2, "wtf_scenario_select", 1);
    set_cmt(0X43A5C0, "wtf_station_list", 1);
    set_cmt(0X491002, "wtf_station_list", 1);
    set_cmt(0X491007, "wtf_station_list", 1);
    set_cmt(0X49182F, "wtf_station_list", 1);
    set_cmt(0X491834, "wtf_station_list", 1);
    set_cmt(0X491927, "wtf_station_list", 1);
    set_cmt(0X49192C, "wtf_station_list", 1);
    set_cmt(0X491948, "wtf_station_list", 1);
    set_cmt(0X48E5C7, "wtf_station", 1);
    set_cmt(0X48E5CC, "wtf_station", 1);
    set_cmt(0X48E6FA, "wtf_station", 1);
    set_cmt(0X48EBA6, "wtf_station", 1);
    set_cmt(0X48EE86, "wtf_station", 1);
    set_cmt(0X48F12B, "wtf_station", 1);
    set_cmt(0X4BB4B6, "wtf_terraform", 1);
    set_cmt(0X4BB50E, "wtf_terraform", 1);
    set_cmt(0X4BB513, "wtf_terraform", 1);
    set_cmt(0X4BB552, "wtf_terraform", 1);
    set_cmt(0X4BB562, "wtf_terraform", 1);
    set_cmt(0X4BB572, "wtf_terraform", 1);
    set_cmt(0X4BB582, "wtf_terraform", 1);
    set_cmt(0X4BBCF7, "wtf_terraform", 1);
    set_cmt(0X4BBCFC, "wtf_terraform", 1);
    set_cmt(0X4BBEA9, "wtf_terraform", 1);
    set_cmt(0X4BC341, "wtf_terraform", 1);
    set_cmt(0X4BC4BC, "wtf_terraform", 1);
    set_cmt(0X4BC50B, "wtf_terraform", 1);
    set_cmt(0X4BC7AE, "wtf_terraform", 1);
    set_cmt(0X4BCB2F, "wtf_terraform", 1);
    set_cmt(0X4BCE9C, "wtf_terraform", 1);
    set_cmt(0X4BF180, "wtf_terraform", 1);
    set_cmt(0X4CE978, "wtf_textinput", 1);
    set_cmt(0X4CE983, "wtf_textinput", 1);
    set_cmt(0X499C96, "wtf_town_list", 1);
    set_cmt(0X499CEE, "wtf_town_list", 1);
    set_cmt(0X499CF3, "wtf_town_list", 1);
    set_cmt(0X49A488, "wtf_town_list", 1);
    set_cmt(0X49A48D, "wtf_town_list", 1);
    set_cmt(0X49A4A9, "wtf_town_list", 1);
    set_cmt(0X49A7FB, "wtf_town_list", 1);
    set_cmt(0X49AE3A, "wtf_town_list", 1);
    set_cmt(0X49B2BA, "wtf_town_list", 1);
    set_cmt(0X49926F, "wtf_town", 1);
    set_cmt(0X499274, "wtf_town", 1);
    set_cmt(0X499394, "wtf_town", 1);
    set_cmt(0X4996E5, "wtf_town", 1);
    set_cmt(0X499920, "wtf_town", 1);
    set_cmt(0X499A97, "wtf_town", 1);
    set_cmt(0X4C19FD, "wtf_vehicle_list", 1);
    set_cmt(0X4C1A94, "wtf_vehicle_list", 1);
    set_cmt(0X4C1A99, "wtf_vehicle_list", 1);
    set_cmt(0X4C24B8, "wtf_vehicle_list", 1);
    set_cmt(0X4C24BD, "wtf_vehicle_list", 1);
    set_cmt(0X4C25EE, "wtf_vehicle_list", 1);
    set_cmt(0X4C25F3, "wtf_vehicle_list", 1);
    set_cmt(0X4C25F8, "wtf_vehicle_list", 1);
    set_cmt(0X4C2614, "wtf_vehicle_list", 1);
    set_cmt(0X4B261F, "wtf_vehicle", 1);
    set_cmt(0X4B2624, "wtf_vehicle", 1);
    set_cmt(0X4B29BC, "wtf_vehicle", 1);
    set_cmt(0X4B2D86, "wtf_vehicle", 1);
    set_cmt(0X4B30FC, "wtf_vehicle", 1);
    set_cmt(0X4B3C81, "wtf_vehicle", 1);
    set_cmt(0X4B4610, "wtf_vehicle", 1);
    set_cmt(0X4B55DA, "wtf_vehicle", 1);
    set_cmt(0X4B599E, "wtf_vehicle", 1);
    set_cmt(0X4B5D98, "wtf_vehicle", 1);
    set_cmt(0X4B60D8, "wtf_vehicle", 1);
    set_cmt(0X4B9488, "wtf_vehicle", 1);
    set_cmt(0X4B9494, "wtf_vehicle", 1);
    set_cmt(0X4C281B, "wtf_vehicle", 1);
    set_cmt(0X49DA76, "ui::construction", 1);
    set_cmt(0X49DA89, "ui::construction", 1);
    set_cmt(0X49DA9C, "ui::construction", 1);

    enumWidget = GetEnum("widget_type");
    if (enumWidget == -1) {
        enumWidget = AddEnum(-1, "widget_type", 0);
    }
    AddConstEx(enumWidget, "widget_type_scroll", 0x1A, -1);
    AddConstEx(enumWidget, "widget_end", 0x1E, -1);

    id = GetStrucIdByName("widget_t");
    if (id == -1) {
        id = AddStruc(-1, "widget_t");
    }
    AddStrucMember(id, "type", 0, 0x000400, -1, 1);
    AddStrucMember(id, "colour", 0X1, 0x000400, -1, 1);
    AddStrucMember(id, "left", 0X2, 0x10000400, -1, 2);
    AddStrucMember(id, "right", 0X4, 0x10000400, -1, 2);
    AddStrucMember(id, "top", 0X6, 0x10000400, -1, 2);
    AddStrucMember(id, "bottom", 0X8, 0x10000400, -1, 2);
    AddStrucMember(id, "image", 0XA, 0x20000400, -1, 4);
    AddStrucMember(id, "tooltip", 0XE, 0x10000400, -1, 2);

    op_stroff(0x0043913A, 1, id, 0);
    op_stroff(0x0043913E, 1, id, 0);
    op_stroff(0x00439147, 1, id, 0);
    op_stroff(0x0043914B, 1, id, 0);
    op_stroff(0x004C7A1E, 1, id, 0);
    op_stroff(0x004C7A22, 1, id, 0);
    op_stroff(0x004C7B95, 1, id, 0);
    op_stroff(0x004C7B99, 1, id, 0);
    op_stroff(0x004C90FB, 1, id, 0);
    op_stroff(0x004CA1BF, 0, id, 0);
    op_stroff(0x004CA1CE, 0, id, 0);
    op_stroff(0x004CA606, 1, id, 0);
    op_stroff(0x004CA614, 1, id, 0);
    op_stroff(0x004CA626, 1, id, 0);
    op_stroff(0x004CA634, 1, id, 0);
    op_stroff(0x004CA67B, 1, id, 0);
    op_stroff(0x004CA67F, 1, id, 0);
    op_stroff(0x004CA683, 1, id, 0);
    op_stroff(0x004CA687, 1, id, 0);
    op_stroff(0x004CA6B0, 1, id, 0);
    op_stroff(0x004CA6B4, 1, id, 0);
    op_stroff(0x004CA6B8, 1, id, 0);
    op_stroff(0x004CA6BC, 1, id, 0);
    op_stroff(0x004CA704, 1, id, 0);
    op_stroff(0x004CA708, 1, id, 0);
    op_stroff(0x004CA71C, 1, id, 0);
    op_stroff(0x004CA720, 1, id, 0);
    op_stroff(0x004CA735, 1, id, 0);
    op_stroff(0x004CA75A, 1, id, 0);
    op_stroff(0x004CA76F, 1, id, 0);
    op_stroff(0x004CA77B, 1, id, 0);
    op_stroff(0x004CA77F, 1, id, 0);
    op_stroff(0x004CA7B9, 1, id, 0);
    op_stroff(0x004CA800, 1, id, 0);
    op_stroff(0x004CA815, 1, id, 0);
    op_stroff(0x004CA821, 1, id, 0);
    op_stroff(0x004CA825, 1, id, 0);
    op_stroff(0x004CA85F, 1, id, 0);
    op_stroff(0x004CA895, 1, id, 0);
    op_stroff(0x004CA8AA, 1, id, 0);
    op_stroff(0x004CA8B6, 1, id, 0);
    op_stroff(0x004CA8BA, 1, id, 0);
    op_stroff(0x004CA8F4, 1, id, 0);
    op_stroff(0x004CA964, 1, id, 0);
    op_stroff(0x004CA968, 1, id, 0);
    op_stroff(0x004CA96C, 1, id, 0);
    op_stroff(0x004CA970, 1, id, 0);
    op_stroff(0x004CAABB, 1, id, 0);
    op_stroff(0x004CAABF, 1, id, 0);
    op_stroff(0x004CAAC3, 1, id, 0);
    op_stroff(0x004CAB0A, 1, id, 0);
    op_stroff(0x004CAB0E, 1, id, 0);
    op_stroff(0x004CAB5A, 1, id, 0);
    op_stroff(0x004CAB5E, 1, id, 0);
    op_stroff(0x004CAB62, 1, id, 0);
    op_stroff(0x004CAB66, 1, id, 0);
    op_stroff(0x004CABB5, 1, id, 0);
    op_stroff(0x004CABB9, 1, id, 0);
    op_stroff(0x004CABFE, 1, id, 0);
    op_stroff(0x004CAC1A, 0, id, 0);
    op_stroff(0x004CAC24, 0, id, 0);
    op_stroff(0x004CAC43, 1, id, 0);
    op_stroff(0x004CAC47, 1, id, 0);
    op_stroff(0x004CAC5F, 1, id, 0);
    op_stroff(0x004CACA6, 1, id, 0);
    op_stroff(0x004CACAA, 1, id, 0);
    op_stroff(0x004CACEC, 1, id, 0);
    op_stroff(0x004CACF0, 1, id, 0);
    op_stroff(0x004CACF4, 1, id, 0);
    op_stroff(0x004CACF8, 1, id, 0);
    op_stroff(0x004CAD37, 0, id, 0);
    op_stroff(0x004CAD4E, 1, id, 0);
    op_stroff(0x004CADAC, 0, id, 0);
    op_stroff(0x004CADE8, 1, id, 0);
    op_stroff(0x004CADEC, 1, id, 0);
    op_stroff(0x004CADF8, 0, id, 0);
    op_stroff(0x004CADFE, 0, id, 0);
    op_stroff(0x004CAE04, 0, id, 0);
    op_stroff(0x004CAE0A, 0, id, 0);
    op_stroff(0x004CB011, 1, id, 0);
    op_stroff(0x004CB01D, 1, id, 0);
    op_stroff(0x004CB08E, 1, id, 0);
    op_stroff(0x004CB166, 1, id, 0);
    op_stroff(0x004CB16A, 1, id, 0);
    op_stroff(0x004CB16E, 1, id, 0);
    op_stroff(0x004CB172, 1, id, 0);
    op_stroff(0x004CB1BE, 1, id, 0);
    op_stroff(0x004CB1CA, 1, id, 0);
    op_stroff(0x004CB1CE, 1, id, 0);
    op_stroff(0x004CB1D2, 1, id, 0);
    op_stroff(0x004CB1DD, 1, id, 0);
    op_stroff(0x004CB1E3, 1, id, 0);
    op_stroff(0x004CB1EF, 0, id, 0);
    op_stroff(0x004CB217, 0, id, 0);
    op_stroff(0x004CB21D, 1, id, 0);
    op_stroff(0x004CB22C, 1, id, 0);
    op_stroff(0x004CB230, 1, id, 0);
    op_stroff(0x004CB267, 1, id, 0);
    op_stroff(0x004CB26B, 1, id, 0);
    op_stroff(0x004CB29E, 1, id, 0);
    op_stroff(0x004CB2A2, 1, id, 0);
    op_stroff(0x004CB2A6, 1, id, 0);
    op_stroff(0x004CB2AA, 1, id, 0);
    op_stroff(0x004CB2D6, 1, id, 0);
    op_stroff(0x004CB2E2, 1, id, 0);
    op_stroff(0x004CB2E6, 1, id, 0);

    LocoWidgetSet(enumWidget, 0x508C8C, 0x4F8D64, "ui::window_36::_widgets_1");
    LocoWidgetSet(enumWidget, 0x508CF0, 0x4F8D64, "ui::window_36::_widgets_0");
    LocoWidgetSet(enumWidget, 0x508D54, 0x4F8DD8, "ui::window_36::_widgets_2");
    LocoWidgetSet(enumWidget, 0x508F1C, 0x4F96D8, "ui::window_12::_widgets_0");
    LocoWidgetSet(enumWidget, 0x508F30, 0x4F96D8, "ui::window_12::_widgets_1");
    LocoWidgetSet(enumWidget, 0x508F54, 0x4F9A90, "ui::window_46");
    LocoWidgetSet(enumWidget, 0x509680, 0x4F9A1C, "ui::company_face_selection");
    LocoWidgetSet(enumWidget, 0x509C20, 0x4FA1F4, "ui::window_0::_widgets");
    LocoWidgetSet(enumWidget, 0x509C34, 0x4FA180, "ui::toolbar_top::_widgets");
    LocoWidgetSet(enumWidget, 0x509D08, 0x4FA024, "ui::toolbar_player_info::_widgets");
    LocoWidgetSet(enumWidget, 0x509D5C, 0x4FA098, "ui::toolbar_time::_widgets");
    LocoWidgetSet(enumWidget, 0x509DE0, 0x4FA10C, "ui::tutorial::_widgets");
    LocoWidgetSet(enumWidget, 0x509DF4, 0x4F9EC8, "ui::title_menu::_widgets");
    LocoWidgetSet(enumWidget, 0x509E58, 0x4F9F3C, "ui::title_exit::_widgets");
    LocoWidgetSet(enumWidget, 0x509E6C, 0x4F9FB0, "ui::title_logo::_widgets");
    LocoWidgetSet(enumWidget, 0x509E80, 0x4FA268, "ui::about::_widgets");
    LocoWidgetSet(enumWidget, 0x509EE4, 0x4FA2DC, "ui::about_atari::_widgets");
    LocoWidgetSet(enumWidget, 0x509F38, 0x4FA350, "ui::about_music::_widgets");
    LocoWidgetSet(enumWidget, 0x509F8C, 0x4FA3C4, "ui::prompt_save_game::_widgets");
    LocoWidgetSet(enumWidget, 0x50A044, 0x4FA5F8, "ui::window_0_alt::_widgets");
    LocoWidgetSet(enumWidget, 0x50A058, 0x4FA584, "ui::toolbar_top_alt::_widgets");
    LocoWidgetSet(enumWidget, 0x50A12C, 0x4FA510, "ui::window_4::_widgets");
    LocoWidgetSet(enumWidget, 0x50ACB4, 0x4FB294, "ui::scenario_select::_widgets");
    LocoWidgetSet(enumWidget, 0x50AD58, 0x4FB308, "ui::prompt_browse::_widgets");
    LocoWidgetSet(enumWidget, 0x50ADDC, 0x4FB3F0, "ui::window_53::_widgets");
    LocoWidgetSet(enumWidget, 0x50AE00, 0x4FB37C, "ui::prompt_ok_cancel::_widgets");
    LocoWidgetSet(enumWidget, 0x50C1B4, 0x4FDDC8, "ui::map");
    LocoWidgetSet(enumWidget, 0x50D164, 0x4FE3C8, "ui::object_selection");
    LocoWidgetSet(enumWidget, 0x522504, 0x50063C, "ui::window_25::_widgets"); //  (created on vehicle event)
    LocoWidgetSet(enumWidget, 0x5228FC, 0x503CDC, "ui::keyboard_shortcuts");
    LocoWidgetSet(enumWidget, 0x522960, 0x503D50, "ui::keyboard_shortcuts_edit");
    LocoWidgetSet(enumWidget, 0x5229A4, 0x5041F8, "ui::music_selection");
    LocoWidgetSet(enumWidget, 0x523040, 0x504274, "ui::window_44");
    LocoWidgetSet(enumWidget, 0x523094, 0x5042E8, "ui::prompt_confirm_display_mode");
    LocoWidgetSet(enumWidget, 0x5234CC, 0x504774, "ui::tooltip::_widgets"); // type: 6
    LocoWidgetSet(enumWidget, 0x5234E0, 0x5048D0, "ui::dropdown::_widgets"); // type: 7
    LocoWidgetSet(enumWidget, 0x5234F4, 0x5049B8, "ui::window_30::_widgets");
    LocoWidgetSet(enumWidget, 0x52350C, 0x504A2C, "ui::window_47::_widgets");
    LocoWidgetSet(enumWidget, 0x523540, 0x504944, "ui::text_input::_widgets");

    LocoWidgetArray(enumWidget, 0x4FCD2C, 0x4FCD34, "ui::industry_list", 2);
    LocoWidgetArray(enumWidget, 0x4F8C60, 0x4F8C68, "ui::messages", 2);
    LocoWidgetArray(enumWidget, 0x4F92C0, 0x4F92D8, "ui::company", 6);
    LocoWidgetArray(enumWidget, 0x4F9338, 0x4F9354, "ui::company_list", 7);
    LocoWidgetArray(enumWidget, 0x4FA438, 0x4FA44C, "ui::landscape_generation", 5);
    LocoWidgetArray(enumWidget, 0x4FA4B0, 0x4FA4C0, "ui::scenario_options", 4);
    LocoWidgetArray(enumWidget, 0x4FCCEC, 0x4FCCFC, "ui::industry", 4);
    LocoWidgetArray(enumWidget, 0x4FDF40, 0x4FDF48, "ui::multiplayer", 2);
    LocoWidgetArray(enumWidget, 0x4FEBF4, 0x4FEC00, "ui::station", 3);
    LocoWidgetArray(enumWidget, 0x4FEC24, 0x4FEC38, "ui::station_list", 5);
    LocoWidgetArray(enumWidget, 0x4FF734, 0x4FF740, "ui::town", 3);
    LocoWidgetArray(enumWidget, 0x4FF764, 0x4FF774, "ui::town_list", 4);
    LocoWidgetArray(enumWidget, 0x4FFAB8, 0x4FFAC8, "ui::construction", 4);
    LocoWidgetArray(enumWidget, 0x5002BC, 0x5002D0, "ui::vehicle", 5);
    LocoWidgetArray(enumWidget, 0x5006E8, 0x5006FC, "ui::terraform", 5);
    LocoWidgetArray(enumWidget, 0x500AD8, 0x500AF0, "ui::options", 6);
    LocoWidgetArray(enumWidget, 0x50435C, 0x504374, "ui::vehicle_list", 6);
    LocoWidgetArray(enumWidget, 0x5043E0, 0x5043F8, "ui::build_vehicle", 6);
}