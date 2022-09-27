#include "main/main.hpp"

#include "cache/cache_files_windows.hpp"
#include "camera/director.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries_windows.hpp"
#include "cseries/symbols_reader.hpp"
#include "game/player_control.hpp"
#include "interface/gui_location_manager.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "main/global_preferences.hpp"
#include "main/main_game_launch.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "simulation/simulation.hpp"
#include "tag_files/string_ids.hpp"

#include <assert.h>

HOOK_DECLARE_CALL(0x00505C2B, main_loop_body_begin);
HOOK_DECLARE_CALL(0x0050605C, main_loop_body_end);

void __cdecl main_loop_body_begin()
{
    FUNCTION_BEGIN(false);

    // right control for tests
    if (GetKeyState(VK_RCONTROL) & 0x8000)
    {
        cache_file_table_of_contents;
        cache_file_copy_globals;
        simulation_globals;
        g_message_globals;

        printf("");
    }
}

void __cdecl main_loop_body_end()
{
    FUNCTION_BEGIN(false);

    // home cluster keys
    if (GetKeyState(VK_INSERT) & 0x8000)
    {
        global_preferences_set_shadow_quality(_quality_setting_high);
        global_preferences_set_texture_resolution_quality(_quality_setting_high);
        global_preferences_set_texture_filtering_quality(_quality_setting_high);
        global_preferences_set_lighting_quality(_quality_setting_high);
        global_preferences_set_effects_quality(_quality_setting_high);
        global_preferences_set_details_quality(_quality_setting_high);
        global_preferences_set_postprocessing_quality(_quality_setting_high);
        global_preferences_set_vsync(true);
        global_preferences_set_motion_blur(false);
        global_preferences_set_antialiasing(true);
        global_preferences_set_brightness(50);
        global_preferences_set_contrast(50);

        global_preferences_set_fullscreen(true);
        sub_79BA30(1920, 1080);
    }
    else if (GetKeyState(VK_DELETE) & 0x8000)
    {
        dword address = symbols_reader.get_rva_blocking(L"?main_loop_body_main_part@@YAXXZ");
        assert(address == 0 || global_address_get(address) == 0x00505C10);

        wchar_t const* name = symbols_reader.get_name_blocking(address);
        display_debug_string("%ls", name);
    }
    else if (GetKeyState(VK_HOME) & 0x8000)
    {
        //main_game_launch_set_multiplayer_splitscreen_count(1);
        main_game_launch_set_coop_player_count(1);
        main_game_launch("maps\\riverworld");
    }
    else if (GetKeyState(VK_END) & 0x8000)
    {
        director_toggle(main_game_launch_get_last_player(), _director_mode_debug);
        Sleep(75);
    }
    else if (GetKeyState(VK_PRIOR) & 0x8000)
    {
        player_control_toggle_machinima_camera_enabled();
        player_control_toggle_machinima_camera_debug();
        Sleep(75);
    }
    else if (GetKeyState(VK_NEXT) & 0x8000)
    {
        player_control_toggle_machinima_camera_use_old_controls();
        Sleep(75);
    }

    // right control for tests
    if (GetKeyState(VK_ESCAPE) & 0x8000)
    {
        //window_manager_load_screen_hs(STRING_ID(gui, start_menu));
        c_load_screen_message* message = (c_load_screen_message*)user_interface_malloc_tracked(sizeof(c_load_screen_message), __FILE__, __LINE__);
        if (load_screen_message_ctor(message, STRING_ID(gui, start_menu), k_any_controller, _window_index4, STRING_ID(gui, top_most)))
            user_interface_messaging_post(message);

        Sleep(25);
    }
}
