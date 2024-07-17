#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_display.hpp"
#include "game/game_engine_team.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_mapping.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "simulation/simulation.hpp"
#include "tag_files/string_ids.hpp"

#include <cstdlib> // qsort

REFERENCE_DECLARE_ARRAY(0x05269788, real, c_gui_screen_scoreboard::m_scoreboard_alpha, 4);
REFERENCE_DECLARE(0x05269798, real, c_gui_screen_scoreboard::m_console_scoreboard_alpha);

HOOK_DECLARE_CLASS_MEMBER(0x00AB42F0, c_gui_scoreboard_data, _update_for_scoreboard_mode);
HOOK_DECLARE_CLASS_MEMBER(0x00AB4920, c_gui_screen_scoreboard, _update_render_state);

void c_scoreboard_load_screen_message::set_is_interactive(bool is_interactive)
{
	m_is_interactive = is_interactive;
}

void c_gui_screen_scoreboard::set_is_interactive(bool is_interactive)
{
	m_is_interactive = is_interactive;
}

void __cdecl c_gui_screen_scoreboard::translate_widget_recursive(c_gui_widget* widget, long a2, long a3)
{
	INVOKE(0x00AB2870, c_gui_screen_scoreboard::translate_widget_recursive, widget, a2, a3);
}

// c_scoreboard_load_screen_message::c_scoreboard_load_screen_message
c_scoreboard_load_screen_message* scoreboard_load_screen_message_ctor(c_scoreboard_load_screen_message* message,
	long screen_name,
	e_controller_index controller,
	e_window_index window,
	long layered_position,
	bool is_interactive)
{
	return DECLFUNC(0x00AB2AD0, c_scoreboard_load_screen_message*, __thiscall, c_scoreboard_load_screen_message*, long, e_controller_index, e_window_index, long, bool)(message, screen_name, controller, window, layered_position, is_interactive);
}

real __cdecl c_gui_screen_scoreboard::get_scoreboard_alpha(e_controller_index controller_index)
{
	return INVOKE(0x00AB30F0, c_gui_screen_scoreboard::get_scoreboard_alpha, controller_index);
}

c_gui_screen_scoreboard* __cdecl c_gui_screen_scoreboard::get_scoreboard_screen(e_controller_index controller_index)
{
	return INVOKE(0x00AB3120, c_gui_screen_scoreboard::get_scoreboard_screen, controller_index);
}

void __cdecl c_gui_screen_scoreboard::hide_scoreboard(e_controller_index controller_index)
{
	return INVOKE(0x00AB3560, c_gui_screen_scoreboard::hide_scoreboard, controller_index);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_displayed(e_controller_index controller_index)
{
	return INVOKE(0x00AB3690, c_gui_screen_scoreboard::is_scoreboard_displayed, controller_index);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_interactive(e_controller_index controller_index)
{
	return INVOKE(0x00AB3740, c_gui_screen_scoreboard::is_scoreboard_interactive, controller_index);
}

void __cdecl c_gui_screen_scoreboard::show_scoreboard(e_controller_index controller_index, bool is_interactive)
{
	INVOKE(0x00AB3C60, c_gui_screen_scoreboard::show_scoreboard, controller_index, is_interactive);

	//if (is_scoreboard_displayed(controller_index))
	//{
	//	if (c_gui_screen_scoreboard* scoreboard_screen = get_scoreboard_screen(controller_index))
	//	{
	//		scoreboard_screen->set_is_interactive(is_interactive);
	//	}
	//}
	//else
	//{
	//	bool half_screen = false;
	//	if (controller_index == k_any_controller)
	//	{
	//		half_screen = true;
	//	}
	//	else
	//	{
	//		long user_index = controller_get(controller_index)->get_user_index();
	//		if (user_index != NONE)
	//		{
	//			long v1 = 0;
	//			long v2 = 0;
	//			user_interface_get_number_of_render_windows(user_index, &v1, &v2);
	//			half_screen = v2 == 1;
	//		}
	//	}
	//
	//	c_scoreboard_load_screen_message* message = (c_scoreboard_load_screen_message*)user_interface_malloc_tracked(sizeof(c_scoreboard_load_screen_message), __FILE__, __LINE__);
	//	if (scoreboard_load_screen_message_ctor(
	//		message,
	//		!half_screen + STRING_ID(gui, scoreboard),
	//		controller_index,
	//		user_interface_get_window_for_controller(controller_index),
	//		STRING_ID(gui, top_most),
	//		is_interactive))
	//	{
	//		user_interface_messaging_post(message);
	//	}
	//}
}

void __thiscall c_gui_screen_scoreboard::_update_render_state(dword a1)
{
	// 0x00AB4920
	//HOOK_INVOKE_CLASS(, c_gui_screen_scoreboard, _update_render_state, void(__thiscall*)(c_gui_screen_scoreboard*, dword), _this, a2);

	c_gui_list_widget* child_list_widget = get_child_list_widget(STRING_ID(gui, scoreboard));
	c_gui_data* data = get_data(STRING_ID(gui, scoreboard), 0);

	//c_gui_widget::update_render_state
	DECLFUNC(0x00ABB1A0, void, __thiscall, c_gui_widget*, dword)(this, a1);

	if (child_list_widget)
	{
		if (data)
		{
			bool has_teams = game_engine_has_teams();

			for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(child_list_widget->get_first_child_widget_by_type(_gui_widget_type_list_item));
				list_item_widget;
				list_item_widget = list_item_widget->get_next_list_item_widget(true))
			{
				c_gui_bitmap_widget* base_color_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color));
				c_gui_text_widget* name_text_widget = list_item_widget->get_child_text_widget(STRING_ID(global, name));
				c_gui_text_widget* team_bar_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, team_bar));
				c_gui_text_widget* service_tag_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag));
				c_gui_text_widget* service_tag_hilite_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag_hilite));
				c_gui_text_widget* place_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, place));
				c_gui_text_widget* score_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, score));
				c_gui_text_widget* round_score_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, round_score));
				c_gui_bitmap_widget* team_bar_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, team_bar));
				c_gui_bitmap_widget* observer_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, observer));

				long element_handle = list_item_widget->get_element_handle();

				long player_row_type = 0;
				long base_color = 0;
				long team_color = NONE;

				if (data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type)
					&& data->get_integer_value(element_handle, STRING_ID(gui, base_color), &base_color)
					&& data->get_integer_value(element_handle, STRING_ID(gui, team_color), &team_color))
				{
					long color_list_index = base_color;
					if (has_teams)
					{
						color_list_index = team_color;
						tint_widget_to_change_color(base_color_bitmap_widget, color_list_index, has_teams);
					}
					else
					{
						base_color |= (255 << 24);
						tint_widget_to_change_argb_color(base_color_bitmap_widget, { .value = static_cast<dword>(base_color) });
					}

					tint_widget_to_change_color((c_gui_widget*)name_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)team_bar_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)service_tag_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)service_tag_hilite_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)place_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)score_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)round_score_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color(team_bar_bitmap_widget, color_list_index, true);
					tint_widget_to_change_color(observer_bitmap_widget, 16, true);
				}
			}
		}

		c_gui_widget* button_key_child_list_widget = get_first_child_widget_by_type(_gui_widget_type_button_key);
		if (button_key_child_list_widget)
		{
			long v18 = 0;
			long y19 = 0;
			for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(child_list_widget->get_first_child_widget_by_type(_gui_widget_type_list_item));
				list_item_widget;
				list_item_widget = list_item_widget->get_next_list_item_widget(false))
			{
				real_rectangle2d current_bounds{};
				list_item_widget->get_current_bounds(&current_bounds);

				if (list_item_widget->get_element_handle() != NONE)
					y19 = (long)current_bounds.y.lower;

				v18 = (long)current_bounds.y.lower;
			}

			if (y19 != v18)
				translate_widget_recursive(button_key_child_list_widget, 0, y19 - v18);
		}
	}
}

void __cdecl c_gui_screen_scoreboard::update_scoreboard_alpha(e_controller_index controller_index)
{
	INVOKE(0x00AB4C50, c_gui_screen_scoreboard::update_scoreboard_alpha, controller_index);
}

bool __cdecl c_gui_scoreboard_data::add_player_internal(
	e_player_row_type row_type,
	long player_index,
	long network_player_index,
	s_player_appearance const* appearance,
	wchar_t const* player_name,
	wchar_t const* service_tag,
	long base_color,
	long multiplayer_team,
	bool team_game,
	e_controller_index controller_index,
	long voice_talking_state,
	long connectivity_rating,
	wchar_t const* place,
	wchar_t const* score,
	wchar_t const* round_score,
	bool dead,
	bool left_game
)
{
	bool result = m_player_row_count < 25;
	if (result)
	{
		m_player_rows[m_player_row_count].player_row_type = row_type;
		m_player_rows[m_player_row_count].player_index = player_index;
		m_player_rows[m_player_row_count].network_player_index = network_player_index;
		csmemcpy(&m_player_rows[m_player_row_count].player_appearance, appearance, sizeof(s_player_appearance));
		m_player_rows[m_player_row_count].player_name.set(player_name);
		m_player_rows[m_player_row_count].service_tag.set(service_tag);
		m_player_rows[m_player_row_count].base_color = base_color;
		m_player_rows[m_player_row_count].multiplayer_team = multiplayer_team;
		m_player_rows[m_player_row_count].team_game = team_game;
		m_player_rows[m_player_row_count].controller_index = controller_index;
		m_player_rows[m_player_row_count].voice_talking_state = voice_talking_state;
		m_player_rows[m_player_row_count].connectivity_rating = connectivity_rating;
		m_player_rows[m_player_row_count].place.set(place);
		m_player_rows[m_player_row_count].score.set(score);
		m_player_rows[m_player_row_count].round_score.set(round_score);
		m_player_rows[m_player_row_count].dead = dead;
		m_player_rows[m_player_row_count].left_game = left_game;

		m_player_row_count++;
	}

	return result;
}

void __thiscall c_gui_scoreboard_data::_update_for_scoreboard_mode(bool a1, bool include_score)
{
	static c_static_wchar_string<256> place;
	static c_static_wchar_string<256> score;
	static c_static_wchar_string<256> round_score;

	place.set(L" ");
	score.set(L" ");
	round_score.set(L" ");

	m_player_row_count = 0;
	m_player_rows.clear();

	bool team_game = game_engine_has_teams();
	bool include_team_score = include_score && team_game;
	long team_round_score = 0;
	dword_flags team_flags = 0;

	if (a1)
	{
		for (long session_player_index = 0; session_player_index < 16; ++session_player_index)
		{
			if (user_interface_squad_is_player_valid(session_player_index))
			{
				s_player_configuration* player_data = user_interface_session_get_player_data(session_player_index);
				if (player_data->host.team_index != NONE)
				{
					long player_rating = NONE;
					e_controller_index controller_index = k_no_controller;
					if (user_interface_session_is_local_player(session_player_index))
						controller_index = user_interface_session_get_controller_index(session_player_index);

					long base_color = player_data->host.armor.loadouts[player_data->host.armor.loadout_index].colors[0].value;
					long voice_for_player = 0;

					add_player_internal(
						/* row_type             */ _player_row_type_player,
						/* player_index         */ NONE,
						/* network_player_index */ session_player_index,
						/* appearance           */ &player_data->host.appearance,
						/* player_name          */ player_data->host.name.get_string(),
						/* service_tag          */ player_data->host.appearance.service_tag.get_string(),
						/* base_color           */ base_color,
						/* multiplayer_team     */ NONE,
						/* team_game            */ false,
						/* controller_index     */ controller_index,
						/* voice_talking_state  */ voice_for_player,
						/* connectivity_rating  */ player_rating,
						/* place                */ place.get_string(),
						/* score                */ score.get_string(),
						/* round_score          */ round_score.get_string(),
						/* dead                 */ false,
						/* left_game            */ false
					);
				}
			}
		}
	}
	else
	{
		c_player_in_game_iterator player_iterator;
		player_iterator.begin();
		while (player_iterator.next())
		{
			player_datum* player = player_iterator.get_datum();
			long player_iterator_index = player_iterator.get_index();
			long network_player_index = user_interface_squad_get_player_index(&player->player_identifier);

			if (player->configuration.host.team_index != NONE)
			{
				bool dead = !TEST_BIT(player->flags, _player_initial_spawn_bit) && player->unit_index == NONE && game_in_progress() && !game_is_finished() && !simulation_starting_up() && game_engine_in_round();

				if (include_score)
				{
					long finalized_place = game_engine_get_player_place(player_iterator_index);
					if (include_team_score)
						finalized_place = game_engine_get_team_place(player->configuration.host.team_index);

					game_engine_get_place_string(finalized_place, &place);
					long player_score = game_engine_get_player_score_for_display(player_iterator_index, true);
					game_engine_get_score_string(player_score, &score);
					long player_round_score = game_engine_get_player_score_for_display(player_iterator_index, false);
					game_engine_get_score_string(player_round_score, &round_score);
				}

				if (include_team_score)
					team_flags |= FLAG(player->configuration.host.team_index);

				if (network_player_index != NONE)
				{
					long player_rating = NONE;

					e_controller_index controller_index = k_no_controller;
					if (user_interface_session_is_local_player(network_player_index))
						controller_index = user_interface_session_get_controller_index(network_player_index);

					long base_color = player->configuration.host.armor.loadouts[player->armor_loadout_index].colors[0].value;
					long voice_for_player = 0;

					add_player_internal(
						/* row_type             */ _player_row_type_player,
						/* player_index         */ NONE,
						/* network_player_index */ network_player_index,
						/* appearance           */ &player->configuration.host.appearance,
						/* player_name          */ player->configuration.host.name.get_string(),
						/* service_tag          */ player->configuration.host.appearance.service_tag.get_string(),
						/* base_color           */ base_color,
						/* multiplayer_team     */ player->configuration.host.team_index,
						/* team_game            */ team_game,
						/* controller_index     */ controller_index,
						/* voice_talking_state  */ voice_for_player,
						/* connectivity_rating  */ player_rating,
						/* place                */ place.get_string(),
						/* score                */ score.get_string(),
						/* round_score          */ round_score.get_string(),
						/* dead                 */ dead,
						/* left_game            */ false
					);
					team_round_score = 0;
				}
				else
				{
					long base_color = player->configuration.host.armor.loadouts[player->armor_loadout_index].colors[0].value;
					long player_index = player_iterator_index;
					bool left_game = TEST_BIT(player->flags, _player_left_game_bit);
					team_round_score = 0;

					c_gui_scoreboard_data::add_player_internal(
						/* row_type             */ _player_row_type_player,
						/* player_index         */ player_index,
						/* network_player_index */ NONE,
						/* appearance           */ &player->configuration.host.appearance,
						/* player_name          */ player->configuration.host.name.get_string(),
						/* service_tag          */ player->configuration.host.appearance.service_tag.get_string(),
						/* base_color           */ base_color,
						/* multiplayer_team     */ player->configuration.host.team_index,
						/* team_game            */ team_game,
						/* controller_index     */ k_no_controller,
						/* voice_talking_state  */ 0,
						/* connectivity_rating  */ NONE,
						/* place                */ place.get_string(),
						/* score                */ score.get_string(),
						/* round_score          */ round_score.get_string(),
						/* dead                 */ dead,
						/* left_game            */ left_game
					);
				}
			}
		}

		if (include_team_score)
		{
			static s_player_appearance player_appearance; // for emblem?
			static c_static_wchar_string<256> team_name;

			csmemset(&player_appearance, 0, sizeof(s_player_appearance));
			team_name.clear();

			for (long team_index = 0; team_index < 8; team_index++)
			{
				if (game_engine_is_team_ever_active(team_index) || TEST_BIT(team_flags, team_index))
				{
					if (include_score)
					{
						long team_place = 0;
						long team_score = 0;

						if (game_engine_is_team_ever_active(team_index))
						{
							team_place = game_engine_get_team_place(team_index);
							team_score = game_engine_get_team_score_for_display(team_index, true);
							team_round_score = game_engine_get_team_score_for_display(team_index, false);
						}

						game_engine_get_place_string(team_place, &place);
						game_engine_get_score_string(team_score, &score);
						game_engine_get_score_string(team_round_score, &round_score);
					}
					game_engine_get_team_name(team_index, &team_name);

					add_player_internal(
						/* row_type             */ _player_row_type_team_bar,
						/* player_index         */ NONE,
						/* network_player_index */ NONE,
						/* appearance           */ &player_appearance,
						/* player_name          */ team_name.get_string(),
						/* service_tag          */ L" ",
						/* base_color           */ NONE,
						/* multiplayer_team     */ team_index,
						/* team_game            */ true,
						/* controller_index     */ k_no_controller,
						/* voice_talking_state  */ 0,
						/* connectivity_rating  */ NONE,
						/* place                */ place.get_string(),
						/* score                */ score.get_string(),
						/* round_score          */ round_score.get_string(),
						/* dead                 */ false,
						/* left                 */ false
					);
				}
			}
		}
	}

	int(__cdecl * scoreboard_sort_proc)(void const*, void const*) = reinterpret_cast<decltype(scoreboard_sort_proc)>(a1 ? 0x00AB3A00 : 0x00AB38A0);
	qsort(m_player_rows.get_elements(), m_player_row_count, sizeof(s_player_row), scoreboard_sort_proc);
}

