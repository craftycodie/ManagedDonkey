#include "main/main_time.hpp"

#include "interface/interface_constants.hpp"
#include "game/game.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "text/draw_string.hpp"
#include "rasterizer/rasterizer_globals.hpp"

REFERENCE_DECLARE(0x022B47FC, bool, display_framerate);

bool debug_disable_frame_rate_throttle = false;
bool display_frame_deltas = false;

void __cdecl main_time_frame_rate_debug()
{
	INVOKE(0x00507B30, main_time_frame_rate_debug);
}

void rasterizer_render_debug_frame_deltas()
{
	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;

	short_rectangle2d bounds{};
	interface_get_current_display_settings(nullptr, nullptr, &bounds, nullptr);

	bounds.x0 = short(bounds.x1 - (50.0f * draw_string_get_glyph_scaling_for_display_settings()));
	bounds.y0 = short(bounds.y1 - (75.0f * draw_string_get_glyph_scaling_for_display_settings()));

	char string[8]{};
	for (long i = (rasterizer_globals.frame_delta_index + 14) % 15; i != rasterizer_globals.frame_delta_index; i = (i + 14) % 15)
	{
		bounds.y0 -= 20;
		bounds.y1 -= 20;

		csnzprintf(string, 4, "%d", rasterizer_globals.frame_deltas[i]);

		real_argb_color const* color = global_real_argb_green;
		if (rasterizer_globals.frame_deltas[i] > 2)
			color = global_real_argb_red;

		draw_string.set_justification(1);
		draw_string.set_color(color);
		draw_string.set_bounds(&bounds);
		draw_string.draw(&font_cache, string);
	}
}

void __cdecl main_time_frame_rate_display()
{
	INVOKE(0x00507B40, main_time_frame_rate_display);

	if (display_frame_deltas)
		rasterizer_render_debug_frame_deltas();
}

//.text:00507D70 ; __int64 sub_507D70() // return g_main_time_globals->last_milliseconds;

__int64 __cdecl main_time_get_absolute_milliseconds()
{
	return INVOKE(0x00507D90, main_time_get_absolute_milliseconds);
}

//.text:00507DD0
//.text:00507DE0

__int64 __cdecl main_time_get_input_collection_time()
{
	return INVOKE(0x00507DF0, main_time_get_input_collection_time);
}

long __cdecl main_time_get_native_tick_rate()
{
	return INVOKE(0x00507E10, main_time_get_native_tick_rate);
}

__int64 __cdecl main_time_get_publishing_end_time()
{
	return INVOKE(0x00507E40, main_time_get_publishing_end_time);
}

__int64 __cdecl main_time_get_publishing_start_time()
{
	return INVOKE(0x00507E60, main_time_get_publishing_start_time);
}

__int64 __cdecl main_time_get_target_display_vblank_index()
{
	return INVOKE(0x00507E80, main_time_get_target_display_vblank_index);
}

long __cdecl main_time_get_time_since_input_collection(__int64 a1)
{
	return INVOKE(0x00507EA0, main_time_get_time_since_input_collection, a1);
}

long __cdecl main_time_get_vblank_rate()
{
	return INVOKE(0x00507EC0, main_time_get_vblank_rate);
}

void __cdecl main_time_globals_post_copy_update(void* userdata)
{
	INVOKE(0x00507EF0, main_time_globals_post_copy_update, userdata);
}

void __cdecl main_time_initialize()
{
	INVOKE(0x00507F10, main_time_initialize);
}

bool __cdecl main_time_is_throttled()
{
	return INVOKE(0x00507FF0, main_time_is_throttled);

	//if (!game_in_progress())
	//	return false;
	//
	//if (debug_disable_frame_rate_throttle)
	//	return false;
	//
	//TLS_DATA_GET_VALUE_REFERENCE(g_main_time_globals);
	//
	//if (g_main_time_globals->__unknown0)
	//	return g_main_time_globals->__unknown1;
	//
	//return true;
}

void __cdecl main_time_mark_input_collection_time()
{
	INVOKE(0x00508030, main_time_mark_input_collection_time);
}

void __cdecl main_time_mark_publishing_end_time()
{
	INVOKE(0x00508070, main_time_mark_publishing_end_time);
}

void __cdecl main_time_mark_publishing_start_time()
{
	INVOKE(0x005080B0, main_time_mark_publishing_start_time);
}

//.text:005080F0 ; void __cdecl sub_5080F0()

void __cdecl main_time_reset()
{
	INVOKE(0x00508110, main_time_reset);
}

void __cdecl main_time_restore(long a1)
{
	INVOKE(0x00508120, main_time_restore, a1);
}

void __cdecl main_time_set_temporary_throttle(bool temporary_throttle)
{
	INVOKE(0x00508130, main_time_set_temporary_throttle, temporary_throttle);
}

void __cdecl main_time_throttle(__int64 a1)
{
	INVOKE(0x00508160, main_time_throttle, a1);
}

real __cdecl main_time_update()
{
	return INVOKE(0x00508170, main_time_update);
}

