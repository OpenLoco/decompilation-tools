#include "common.idc"
#include <idc.idc>

static loco_makeArrayMaybe(offset, count)
{
    if (count == 1)
        return;

    make_array(offset, count);
}

static loco_setType(offset, type, count)
{
    if (type == "u8" || type == "s8" || type == "b8") {
        MakeUnknown(offset, count * 1, 1 | 2);
        create_byte(offset);
        if (type == "b8") {
            op_bin(offset, 0);
        }
        loco_makeArrayMaybe(offset, count);
        return;
    } else if (type == "u16" || type == "s16") {
        MakeUnknown(offset, count * 2, 1 | 2);
        create_word(offset);
        loco_makeArrayMaybe(offset, count);
        return;
    } else if (type == "u32" || type == "s32" || type == "ptr32") {
        MakeUnknown(offset, count * 4, 1 | 2);
        create_dword(offset);
        if (type == "ptr32") {
            op_plain_offset(offset, -1, 0);
        }
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
}

static loco_var(offset, type, name)
{
    if (type != "") {
        auto count;

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
    // sort on
    loco_var(0x004D72EC, "u32", "lpClassName");
    loco_var(0x004FAE28, "u32", "file_paths");
    loco_var(0x004FD120, "u32[4]", "AddToPlotList1");
    loco_var(0x004FD130, "u32[4]", "AddToPlotList2");
    loco_var(0x004FD140, "u32[4]", "AddToPlotList3");
    loco_var(0x004FD150, "u32[4]", "AddToPlotList4");
    loco_var(0x004FD2E8, "u32", "map_animation_event_handlers");
    loco_var(0x004FE250, "u16[34]", "object_entry_group_counts");
    loco_var(0x004FE294, "u16[34]", "");
    loco_var(0x004FE2D8, "u8[34]", "");
    loco_var(0x004FE384, "b8[34]", "");
    loco_var(0x004FE3A6, "u8", "window_editor_object_selection_default_objects_flags");
    loco_var(0x004FEF98, "u16", "month_table");
    loco_var(0x00503DD0, "u32", "keyboard_shortcut_actions");
    loco_var(0x005046FA, "u8", "appropriate_image_dropdown_items_per_row");
    loco_var(0x00504C78, "u16", "SrcStr");
    loco_var(0x005060D0, "u8", "Data");
    loco_var(0x00508F08, "u8", "game_command_nest_level");
    loco_var(0x00508F12, "u16", "screen_age");
    loco_var(0x00508F14, "u16", "screen_flags");
    loco_var(0x00508F16, "u8", "screenshot_countdown");
    loco_var(0x00508F17, "u8", "is_paused");
    loco_var(0x00508F18, "u8", "place_object_modifier");
    loco_var(0x00508F19, "u8", "on_tutorial");
    loco_var(0x00508F1A, "u8", "game_speed");
    loco_var(0x0050AE8C, "u32", "scenario_list");
    loco_var(0x0050AEA0, "u32", "num_scenarios");
    loco_var(0x0050AEB4, "u32", "config_data_start");
    loco_var(0x0050AEB8, "u16", "saved_resolution_width");
    loco_var(0x0050AEBA, "u16", "saved_resolution_height");
    loco_var(0x0050AEDE, "u16", "keyboard_shortcuts");
    loco_var(0x0050B0CE, "u8[257]", "path_install");
    loco_var(0x0050B1CF, "u8[257]", "path_1p_saves");
    loco_var(0x0050B2EC, "u8[257]", "path_2p_saves");
    loco_var(0x0050B406, "u8[257]", "path_scenarios");
    loco_var(0x0050B518, "u8[257]", "path_landscapes");
    loco_var(0x0050B635, "u8[257]", "path_objects");
    loco_var(0x0050B85C, "u32", "g1_buffer");
    loco_var(0x0050B884, "drawpixelinfo_t", "screen_dpi");
    loco_var(0x0050B894, "u16", "ui_width");
    loco_var(0x0050B896, "u16", "ui_height");
    loco_var(0x0050B8A0, "u16", "dirty_blocks_width");
    loco_var(0x0050B8A2, "u16", "dirty_blocks_height");
    loco_var(0x0050B8A4, "u32", "dirty_blocks_width_2");
    loco_var(0x0050B8A8, "u32", "dirty_blocks_height_2");
    loco_var(0x0050B8AC, "u8", "dirty_blocks_columns");
    loco_var(0x0050B8AD, "u8", "dirty_blocks_rows");
    loco_var(0x0050B8AE, "u8", "dirty_blocks_initialised");
    loco_var(0x0050BC60, "u32", "draw_rain_function");
    loco_var(0x0050C194, "u8", "game_initialized");
    loco_var(0x0050C195, "u8", "intro_state");
    loco_var(0x0050C19C, "u16", "ticks_since_last_update");
    loco_var(0x0050C19E, "u32", "last_tick_count");
    loco_var(0x0050C1A6, "u32", "game_loop_iteration_esp");
    loco_var(0x0050D13C, "u32", "installed_object_list");
    loco_var(0x0050D154, "u32", "total_num_images");
    loco_var(0x0050D1E8, "u32", "current_sound_device");
    loco_var(0x0050D555, "u8", "enable_sound");
    loco_var(0x0050D5B4, "u8", "playing_title_music");
    loco_var(0x005183FC, "u32[8192]", "strings");
    loco_var(0x005230C8, "u32", "map_elements");
    loco_var(0x00523338, "u32", "cursor_x_2");
    loco_var(0x0052333C, "u32", "cursor_y_2");
    loco_var(0x00523360, "u16", "textinput::_callingWidget");
    loco_var(0x00523362, "u16", "textinput::_windowNumber");
    loco_var(0x00523364, "u8", "textinput::_windowType");
    loco_var(0x00523368, "u32", "input_flags");
    loco_var(0x0052336D, "u8", "input_state");
    loco_var(0x0052336F, "u8", "_pressedWindowType");
    loco_var(0x00523370, "u16", "_pressedWindowNumber");
    loco_var(0x00523372, "u32", "_pressedWidgetIndex");
    loco_var(0x00523376, "u16", "_clickRepeatTicks");
    loco_var(0x00523378, "u16", "_dragLastX");
    loco_var(0x0052337A, "u16", "_dragLastY");
    loco_var(0x0052337C, "u16", "_dragWindowNumber");
    loco_var(0x0052337E, "u8", "_dragWindowType");
    loco_var(0x0052337F, "u8", "_dragWidgetIndex");
    loco_var(0x00523380, "u8", "_dragScrollIndex");
    loco_var(0x00523381, "u8", "_tooltipWindowType");
    loco_var(0x00523382, "u16", "_tooltipWindowNumber");
    loco_var(0x00523384, "u16", "_tooltipWidgetIndex");
    loco_var(0x00523386, "u16", "_tooltipCursorX");
    loco_var(0x00523388, "u16", "_tooltipCursorY");
    loco_var(0x0052338A, "u16", "_tooltipTimeout");
    loco_var(0x0052338C, "u16", "tooltip_not_shown_ticks");
    loco_var(0x0052338E, "u16", "_ticksSinceDragStart");
    loco_var(0x00523390, "u16", "tool_window_number");
    loco_var(0x00523392, "u8", "tool_window_class");
    loco_var(0x00523393, "u8", "current_tool");
    loco_var(0x00523394, "u16", "tool_widget_index");
    loco_var(0x00523398, "u32", "_currentScrollOffset");
    loco_var(0x005233A4, "u16", "last_scroll_cursor_x");
    loco_var(0x005233A6, "u16", "last_scroll_cursor_y");
    loco_var(0x005233A8, "u8", "hover_type");
    loco_var(0x005233AA, "u16", "hover_number");
    loco_var(0x005233AC, "u16", "hover_widget_index");
    loco_var(0x005233B6, "u8", "modal_window_type");
    loco_var(0x005233B8, "drawpixelinfo_t", "window_dpi");
    loco_var(0x00523814, "u32", "cchMultiByte");
    loco_var(0x00524120, "u32", "uTimerID");
    loco_var(0x0052412C, "u32", "Target");
    loco_var(0x00524144, "u32", "ddraw_psurface");
    loco_var(0x00524168, "u32", "ddraw_num_display_modes");
    loco_var(0x005251DC, "u32", "dinput");
    loco_var(0x005251E0, "u32", "dinput_device_mouse");
    loco_var(0x005251E4, "u32", "dinput_device_keyboard");
    loco_var(0x005251F0, "u32", "num_dsound_devices");
    loco_var(0x005251F4, "u32", "dsound_devices");
    loco_var(0x0052524C, "u32", "video_interface_update_palette");
    loco_var(0x00525314, "u32", "ppv");
    loco_var(0x00525320, "u32", "hWnd");
    loco_var(0x00525330, "u32", "cursor_wheel");
    loco_var(0x00525374, "u32", "_cursor_drag_state");
    loco_var(0x00525380, "u32", "key_queue_write_index");
    loco_var(0x00525384, "u32", "key_queue_read_index");
    loco_var(0x00525394, "u32", "input_queue_index");
    loco_var(0x00525E18, "u32", "srand_0");
    loco_var(0x00525E1C, "u32", "srand_1");
    loco_var(0x00525E2C, "u32", "current_day");
    loco_var(0x00525E30, "u16", "day_counter");
    loco_var(0x00525E32, "u16", "current_year");
    loco_var(0x00525E34, "u8", "current_month");
    loco_var(0x00525E35, "u8", "current_day_of_month");
    loco_var(0x00525E36, "u16", "saved_view_x");
    loco_var(0x00525E38, "u16", "saved_view_y");
    loco_var(0x00525E3A, "u16", "saved_view_zoom_and_rotation");
    loco_var(0x00525E3E, "u16", "sprites_next_index");
    loco_var(0x00525E40, "u16", "things_head_null");
    loco_var(0x00525E42, "u16", "things_head_vehicle");
    loco_var(0x00525E46, "u16", "things_head_misc");
    loco_var(0x00525F5E, "u32", "scenario_ticks");
    loco_var(0x00525F64, "u32", "scenario_ticks_2");
    loco_var(0x00525F6C, "u16", "map_animations_count");
    loco_var(0x00531780, "u8", "current_rain_level");
    loco_var(0x00531784, "company_t[15]", "_companies");
    loco_var(0x005B825C, "town_t[80]", "towns");
    loco_var(0x005C455C, "industry_t[128]", "_industries");
    loco_var(0x005E6EDC, "station_t[1024]", "stations");
    loco_var(0x006DB6DC, "entity_t[20000]", "things");
    loco_var(0x0094C6DC, "map_animation_t[0x2000]", "_mapAnimations");
    loco_var(0x0095885C, "str_32[0x800]", "_userStrings");
    loco_var(0x009C68E0, "u16", "game_command_map_x");
    loco_var(0x009C68E2, "u16", "game_command_map_y");
    loco_var(0x009C68E4, "u16", "game_command_map_z");
    loco_var(0x009C68E6, "u16", "game_command_error_reason_stringid");
    loco_var(0x009CCA34, "u8", "scenario_header_chunk");
    loco_var(0x009D0E73, "u8[257]", "FileName");
    loco_var(0x009D118E, "u8[260]", "path_buffer");
    loco_var(0x009D1C9C, "u32", "save_scenario_or_game_flags");
    loco_var(0x009D1CBC, "u32", "common_hfile");
    loco_var(0x009D9D4D, "u8", "write_chunk_encoding_type");
    loco_var(0x009D9D52, "u32", "decoded_chunk_size");
    loco_var(0x009D9D63, "u8", "prompt_browse_type");
    loco_var(0x009D9D64, "u8[256]", "prompt_browse_title");
    loco_var(0x009D9E64, "u8[32]", "prompt_browse_filter");
    loco_var(0x009D9E84, "u8[512]", "prompt_browse_directory");
    loco_var(0x009E2424, "g1_element_t[0x4201A]", "_g1Data");
    loco_var(0x00E025C4, "u8", "dirty_blocks");
    loco_var(0x00E04320, "u32", "screenshot_buffer");
    loco_var(0x00E04328, "u32", "screenshot_hfile");
    loco_var(0x00E0C408, "u32", "paint_entry_2");
    loco_var(0x00E0C40C, "u32", "paint_entry");
    loco_var(0x00E3F0B8, "u32", "current_rotation");
    loco_var(0x00E400C0, "u32", "_paintQuadrantBackIndex");
    loco_var(0x00E400C4, "u32", "_paintQuadrantFrontIndex");
    loco_var(0x00E400C8, "u32", "g_saved_ps_cur");
    loco_var(0x00E400CC, "u32", "g_saved_ps_cur3");
    loco_var(0x00E40134, "ptr32[0x30000]", "tile_map_element_pointers");
    loco_var(0x00F24484, "u16", "map_selection_flags");
    loco_var(0x00F24486, "u16", "map_selection_a_x");
    loco_var(0x00F24488, "u16", "map_selection_b_x");
    loco_var(0x00F2448A, "u16", "map_selection_a_y");
    loco_var(0x00F2448C, "u16", "map_selection_b_y");
    loco_var(0x00F24490, "u32", "map_selected_tiles");
    loco_var(0x00F2533F, "u8", "gridlines_state");
    loco_var(0x00F253A2, "u16", "num_logic_updates");
    loco_var(0x0112A110, "u32", "installed_object_list_count");
    loco_var(0x0112A128, "u32", "loaded_object_entry");
    loco_var(0x0112C810, "u32", "current_day_in_olympiad");
    loco_var(0x0112C824, "u16", "current_font_flags");
    loco_var(0x0112C826, "u32", "common_format_args");
    loco_var(0x0112C876, "u16", "current_font_sprite_base");
    loco_var(0x0112C884, "u32", "font_character_widths");
    loco_var(0x0112CC04, "u8[256]", "buffer_string_format");
    loco_var(0x0112D408, "u8", "character_bitmap_array");
    loco_var(0x0113653C, "u8[16]", "formatargs");
    loco_var(0x011365A0, "u16", "textinput::_title");
    loco_var(0x011366A0, "u16", "textinput::_message");
    loco_var(0x011369A0, "u8[512]", "textinput_buffer");
    loco_var(0x01136BA0, "colourmap_a[31]", "ColourMapA");
    loco_var(0x01136C98, "colourmap_b[31]", "ColourMapB");
    loco_var(0x01136D90, "u8[512]", "");
    loco_var(0x01136F94, "u16", "window_dropdown_onpaint_cell_x");
    loco_var(0x01136F96, "u16", "window_dropdown_onpaint_cell_y");
    loco_var(0x01136FA4, "s16", "txtutils::word_1136FA4");
    loco_var(0x011370AC, "window_t", "_windows");
    loco_var(0x0113D754, "u32", "new_window_pointer");
    loco_var(0x0113D758, "viewport_t[10]", "_viewports");
    loco_var(0x0113D820, "u32", "new_viewport_pointer");
    loco_var(0x0113D84C, "u16", "dropdown_num_items");
    loco_var(0x0113D84E, "u16", "dropdown_highlighted_index");
    loco_var(0x0113D850, "u16[40]", "_dropdownItemFormats");
    loco_var(0x0113D8A0, "dropdown_format_args_t[40]", "_dropdownItemFormatArgs1");
    loco_var(0x0113D9E0, "dropdown_format_args_t[40]", "_dropdownItemFormatArgs2");
    loco_var(0x0113DC60, "u32", "dropdown_items_disabled");
    loco_var(0x0113DC64, "u32", "dropdown_checked");
    loco_var(0x0113DC68, "u32", "dropdown_item_height");
    loco_var(0x0113DC6C, "u32", "dropdown_item_width");
    loco_var(0x0113DC70, "u32", "dropdown_num_columns");
    loco_var(0x0113DC74, "u32", "dropdown_num_rows");
    loco_var(0x0113DE00, "u32", "cursor_pointer");
    loco_var(0x0113DE04, "u32", "hCursor");
    loco_var(0x0113DF44, "u32", "CodePage");
    loco_var(0x0113DF54, "u8[260]", "Filename");
    loco_var(0x0113E0AC, "u32", "lpString");
    loco_var(0x0113E0B0, "u16", "os_time_month");
    loco_var(0x0113E0B4, "u32", "hinst");
    loco_var(0x0113E0B8, "u32", "min_window_height");
    loco_var(0x0113E1E0, "u16", "os_time_dayofweek");
    loco_var(0x0113E228, "u8[80]", "Buffer");
    loco_var(0x0113E300, "u32", "key_queue");
    loco_var(0x0113E724, "u32", "min_window_width");
    loco_var(0x0113E72C, "u32", "cursor_x");
    loco_var(0x0113E730, "u32", "cursor_y");
    loco_var(0x0113E844, "u32", "screen_height");
    loco_var(0x0113E87C, "u32", "exception_code_os");
    loco_var(0x0113E8C0, "u8[260]", "WindowName");
    loco_var(0x0113E9C4, "u16", "os_time_day");
    loco_var(0x0113E9D4, "u32", "queue_cursor_x");
    loco_var(0x0113E9D8, "u32", "queue_cursor_y");
    loco_var(0x0113E9DC, "u16", "os_time_year");
    loco_var(0x0113ECE0, "u32", "screen_width");
    loco_var(0x0113ED00, "u32", "nPriority");
    loco_var(0x0113F128, "u32", "dwPriorityClass");
    loco_var(0x0113FFE8, "u32", "current_dddisplaymode_index");
    loco_var(0x01140020, "u16", "dddisplaymodes_width");
    loco_var(0x01140022, "u16", "dddisplaymodes_height");
    loco_var(0x01140024, "u16", "dddisplaymodes_depth");
    loco_var(0x01140624, "", "X");
    loco_var(0x01140628, "u32", "Y");
    loco_var(0x01140740, "", "keyboard_state");
    loco_var(0x01140840, "u32", "mouse_state_rely");
    loco_var(0x0114084C, "u32", "mouse_state_relx");
    loco_var(0x011427E0, "u32", "uNumber");
    loco_var(0x011427F8, "u32", "lpMem");
    loco_var(0x011427FC, "u32", "hHeap");
    // sort off
}
