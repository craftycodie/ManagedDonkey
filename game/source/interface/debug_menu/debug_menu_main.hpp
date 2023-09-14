#pragma once

#define DEBUG_MENU_NUM_GLOBAL_CAPTIONS 8

union real_argb_color;
struct gamepad_state;
class c_debug_menu;

extern real_argb_color const* const debug_real_argb_grey;
extern real_argb_color const* const debug_real_argb_white;
extern real_argb_color const* const debug_real_argb_tv_white;
extern real_argb_color const* const debug_real_argb_tv_blue;
extern real_argb_color const* const debug_real_argb_tv_magenta;
extern real_argb_color const* const debug_real_argb_tv_orange;
extern real_argb_color const* const debug_real_argb_tv_green;

extern bool debug_menu_enabled;

extern void debug_menu_draw_rect(short, short, short, short, float, real_argb_color const*);
extern bool debug_menu_get_active();
extern void debug_menu_initialize();
extern void debug_menu_dispose();
extern void debug_menu_initialize_for_new_map();
extern void debug_menu_dispose_from_old_map();
extern void debug_menu_update();
extern void debug_menu_open();
extern void debug_menu_close();
extern void render_debug_debug_menu_game();
extern void render_debug_debug_menu();
extern gamepad_state const& debug_menu_get_gamepad_state();
extern gamepad_state const& debug_menu_get_last_gamepad_state();
extern c_debug_menu* debug_menu_get_active_menu();
extern void debug_menu_set_active_menu(c_debug_menu* menu, bool active);
extern void debug_menu_set_caption(short caption_index, char const* caption);
extern char const* debug_menu_get_caption(short caption_index);
extern long debug_menu_get_time();
extern void* debug_menu_malloc(long size);
extern void debug_menu_parse(c_debug_menu* root_menu, char const* file_name);
