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

static initWidgets(void)
{
    auto id, i, enumWidget;

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
