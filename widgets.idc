#include <idc.idc>

static LocoWidgetList(enumWidget, addr, name)
{
    auto i = 0;
    while (1) {
        auto type = Byte(addr + i * 0x10);
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
    LocoWidgetList(enumWidget, layoutAddr, sprintf("%s::_widgets", name));
    LocoEvents(eventAddr, name, "", sprintf("%s::_events", name));
}

static getEventName(event)
{
    if (event == 28) {
        return "scroll_paint";
    }
    if (event == 27) {
        return "onpaint";
    }
    if (event == 16) {
        return "onscrollgetheight";
    }

    return sprintf("%d", event);
}

static IsNullSub(addr)
{
    auto chr = Byte(addr);
    if (chr == 0xC3 || chr == 0xCB) {
        return 1;
    }

    return 0;
}

static LocoEvents(addr, prefix, suffix, basename)
{
    auto i;
    MakeUnknown(addr, 29 * 4, 1 | 2);
    MakeStruct(addr, "window_events_t");
    SetColor(addr, CIC_ITEM, 0xFFFFFF);
    MakeName(addr, basename);

    for (i = 0; i <= 28; i++) {
        auto offset = Dword(addr + i * 4);
        if (IsNullSub(offset)) {
            MakeName(offset, sprintf("nullsub_%x", offset)); // SN_AUTO | SN_NOWARN
            continue;
        }
        MakeName(offset, sprintf("%s::event%s_%s", prefix, suffix, getEventName(i)));
    }
}

static LocoWidgetArray(enumWidget, addr, addr2, name, count)
{
    auto i = 0;

    OpOff(addr, 0, 0);
    MakeArray(addr, count);
    SetColor(addr, CIC_ITEM, 0xFFFFFF);

    OpOff(addr2, 0, 0);
    MakeArray(addr2, count);
    SetColor(addr2, CIC_ITEM, 0xFFFFFF);

    for (i = 0; i < count; i++) {
        auto widgets = Dword(addr + i * 4);
        LocoWidgetList(enumWidget, widgets, sprintf("%s::_widgets_%d", name, i + 1));

        auto events = Dword(addr2 + i * 4);
        LocoEvents(events, name, sprintf("_%d", i + 1), sprintf("%s::_events_%d", name, i + 1));
    }
}

static initWidgets(void)
{
    auto id, i;

    id = GetStrucIdByName("window_events_t");
    if (id == -1) {
        id = AddStruc(-1, "window_events_t");
        for (i = 0; i <= 28; i++) {
            AddStrucMember(id, sprintf("event_%s", getEventName(i)), i * 4, FF_DWRD | FF_DATA | FF_0OFF, 0, 4);
        }
    } else {

        for (i = 0; i <= 28; i++) {
            SetMemberName(id, i * 4, sprintf("event_%s", getEventName(i)));
            SetMemberType(id, i * 4, FF_DWRD | FF_DATA | FF_0OFF, 0, 1);
        }
    }

    auto enumWidget = GetEnum("widget_type");
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
