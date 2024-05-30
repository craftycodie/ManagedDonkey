#include "interface/user_interface_networking.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game_results.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_error_manager.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/network_globals.hpp"
#include "shell/shell.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x05253D88, s_user_interface_networking_globals, user_interface_networking_globals);

void __cdecl sub_69D600()
{
	//INVOKE(0x0069D600, sub_69D600);

#define UI_WAIT(_time, _set_value, _get_value, _value) \
_set_value(_value);                             \
do                                              \
{                                               \
	user_interface_update(_time);               \
	network_update();                           \
} while (_get_value() != _value);               \

	UI_WAIT(0.1f, user_interface_networking_enter_pregame_location, user_interface_squad_get_ui_game_mode, _ui_game_mode_multiplayer);
	UI_WAIT(0.1f, user_interface_squad_set_session_advertisement, user_interface_networking_get_session_advertisement, _gui_network_session_advertisement_mode_system_link);
}

bool __cdecl user_interface_join_remote_session(bool join_to_public_slots, long session_class, s_transport_secure_identifier* remote_session_id, s_transport_secure_address* remote_host_address, s_transport_secure_key* key)
{
	return INVOKE(0x00A7E3C0, user_interface_join_remote_session, join_to_public_slots, session_class, remote_session_id, remote_host_address, key);
}

bool __cdecl user_interface_networking_build_is_compatible(long executable_type, long executable_version, long compatible_version)
{
	return INVOKE(0x00A7EC20, user_interface_networking_build_is_compatible, executable_type, executable_version, compatible_version);

	//return network_build_is_compatible(executable_type, executable_version, compatible_version);
}

//.text:00A7EC30

void __cdecl user_interface_networking_clear_last_game_started()
{
	//INVOKE(0x00A7EC50, user_interface_networking_clear_last_game_started);

	generate_event(_event_level_message, "networking:ui: clearning last game started flag");

	user_interface_networking_globals.start_game_last_started = false;
}

//.text:00A7EC60 ; bool __cdecl user_interface_networking_dialogs_handle_results(c_dialog_result_message const*)
//.text:00A7EDB0 ; void __cdecl user_interface_networking_dialogs_request_join(long user_index)

void __cdecl user_interface_networking_dispose()
{
	INVOKE(0x00A7EE10, user_interface_networking_dispose);

	//network_join_squad_join_abort();
	//user_interface_networking_initialize();
}

void __cdecl user_interface_networking_dispose_from_old_map()
{
	//INVOKE(0x00A7EE60, user_interface_networking_dispose_from_old_map);
}

bool __cdecl user_interface_networking_enter_pregame_location(e_gui_game_mode game_mode)
{
	return INVOKE(0x00A7EE70, user_interface_networking_enter_pregame_location, game_mode);
}

void __cdecl user_interface_networking_game_start_when_joined(long player_count, long countdown_timer)
{
	return INVOKE(0x00A7EEC0, user_interface_networking_game_start_when_joined, player_count, countdown_timer);

	//if (player_count || countdown_timer || user_interface_squad_get_countdown_timer() <= 0)
	//{
	//	user_interface_networking_globals.start_game_when_ready = true;
	//	user_interface_networking_globals.__unknown1C = true;
	//	user_interface_networking_globals.player_count = player_count;
	//	user_interface_networking_globals.countdown_timer = countdown_timer;
	//}
	//else if (user_interface_squad_get_countdown_timer() >= 4 && user_interface_squad_start_countdown_timer(0, 0, 4))
	//{
	//	while (user_interface_squad_get_countdown_timer() > 4 && user_interface_squad_start_countdown_timer(0, 0, 4));
	//}
}

bool __cdecl user_interface_interactive_session_get_campaign_quit()
{
	return INVOKE(0x00A7EF40, user_interface_interactive_session_get_campaign_quit);

	//return network_interactive_session_get_campaign_quit();
}

bool __cdecl user_interface_networking_get_closed()
{
	return INVOKE(0x00A7EF50, user_interface_networking_get_closed);

	//return network_squad_session_get_closed();
}

bool __cdecl sub_A7EF60()
{
	return INVOKE(0x00A7EF60, sub_A7EF60);

	//return sub_438C00();
}

//e_gui_location __cdecl user_interface_networking_get_current_location()
long __cdecl user_interface_networking_get_current_location()
{
	return INVOKE(0x00A7EF70, user_interface_networking_get_current_location);

	//return 5;
}
//HOOK_DECLARE(0x00A7EF70, user_interface_networking_get_current_location);

long __cdecl user_interface_networking_get_current_party_size()
{
	return INVOKE(0x00A7F050, user_interface_networking_get_current_party_size);

	//long player_count = 1;
	//if (network_squad_session_get_membership(NULL, NULL, NULL, NULL, NULL, NULL, NULL, &player_count, NULL, NULL))
	//	return player_count;
	//
	//return player_count;
}

c_game_results const* __cdecl user_interface_networking_get_final_game_results()
{
	return INVOKE(0x00A7F090, user_interface_networking_get_final_game_results);

	//c_game_results const* final_game_results = game_results_get_final_results();
	//if (final_game_results && final_game_results->game_description.finished)
	//	return final_game_results;
	//
	//return NULL;
}

long __cdecl user_interface_networking_get_maximum_party_size()
{
	return INVOKE(0x00A7F0B0, user_interface_networking_get_maximum_party_size);

	//return network_squad_session_get_maximum_player_count();
}

long __cdecl user_interface_networking_get_name_from_gui_game_mode(e_gui_game_mode gui_game_mode)
{
	return INVOKE(0x00A7F0C0, user_interface_networking_get_name_from_gui_game_mode, gui_game_mode);

	//switch (gui_game_mode)
	//{
	//case _ui_game_mode_campaign:    return STRING_ID(gui, campaign);
	//case _ui_game_mode_matchmaking: return STRING_ID(gui, matchmaking);
	//case _ui_game_mode_multiplayer: return STRING_ID(global, multiplayer);
	//case _ui_game_mode_map_editor:  return STRING_ID(gui, mapeditor);
	//case _ui_game_mode_theater:     return STRING_ID(gui, theater);
	//case _ui_game_mode_survival:    return STRING_ID(gui, survival);
	//}
	//
	//return _string_id_invalid; // ?
}

//.text:00A7F110

e_gui_network_session_advertisement_mode __cdecl user_interface_networking_get_session_advertisement()
{
	return INVOKE(0x00A7F160, user_interface_networking_get_session_advertisement);
}

bool __cdecl user_interface_networking_get_start_game_when_ready()
{
	//return INVOKE(0x00A7F1B0, user_interface_networking_get_start_game_when_ready);

	return user_interface_networking_globals.start_game_when_ready;
}

bool __cdecl user_interface_networking_handle_dialog_result(c_dialog_result_message const* message)
{
	return INVOKE(0x00A7F1C0, user_interface_networking_handle_dialog_result, message);
}

void __cdecl user_interface_networking_in_progress_update()
{
	INVOKE(0x00A7F2B0, user_interface_networking_in_progress_update);
}

void __cdecl user_interface_networking_initialize()
{
	INVOKE(0x00A7F380, user_interface_networking_initialize);

	//csmemset(&user_interface_networking_globals, 0, sizeof(user_interface_networking_globals));
	//user_interface_networking_globals.current_screen_controller_index = k_no_controller;
	//user_interface_networking_globals.active_alert = _string_id_invalid;
	//user_interface_networking_globals.active_dialog = _string_id_invalid;
	//user_interface_networking_globals.join_failed_alert_name = _string_id_invalid;
	//user_interface_session_initialize();
}

void __cdecl user_interface_networking_initialize_for_new_map()
{
	INVOKE(0x00A7F3D0, user_interface_networking_initialize_for_new_map);

	//network_banhammer_update_for_new_map();
	//user_interface_networking_globals.start_game_when_ready = false;
}

//.text:00A7F3E0

void __cdecl user_interface_networking_join_update()
{
	INVOKE(0x00A7F3F0, user_interface_networking_join_update);
}

void __cdecl user_interface_networking_memory_dispose()
{
	INVOKE(0x00A7F560, user_interface_networking_memory_dispose);

	//user_interface_session_memory_dispose();
}

//void __cdecl user_interface_networking_memory_initialize(e_map_memory_configuration configuration)
void __cdecl user_interface_networking_memory_initialize(long configuration)
{
	//INVOKE(0x00A7F570, user_interface_networking_memory_initialize, configuration);

}

void __cdecl user_interface_networking_notify_booted_from_session(e_network_session_type type, e_network_session_boot_reason boot_reason)
{
	INVOKE(0x00A7F580, user_interface_networking_notify_booted_from_session, type, boot_reason);

	generate_event(_event_level_message, "networking:ui: notified that we have been booted from a session [type %d / reason %d]", type, boot_reason);

	if (boot_reason == _network_session_boot_reason_unknown4)
	{
		generate_event(_event_level_message, "networking:ui: posting alert and taking us back to the pre-game lobby (from-ui)");
		user_interface_error_manager_get()->post_error(STRING_ID(gui_alert, booted_from_session), k_any_controller, false);
	}
	else
	{
		generate_event(_event_level_message, "networking:ui: posting alert and taking us back to the pre-game lobby (in-game)");
		user_interface_error_manager_get()->post_error(STRING_ID(gui_alert, booted_from_game), k_any_controller, false);
	}

	user_interface_leave_sessions(_user_interface_session_leave_type_leave_to_pre_game_lobby, _user_interface_session_leave_reason_booted);
}

//.text:00A7F5D0
//.text:00A7F660
//.text:00A7F680
//.text:00A7F710
//.text:00A7F730
//.text:00A7F7C0
//.text:00A7F850
//.text:00A7F870
//.text:00A7F890
//.text:00A7F8B0 ; bool __cdecl user_interface_interactive_session_request_campaign_quit(e_network_session_request_campaign_quit_reason campaign_quit_reason)
//.text:00A7F8D0 ; void __cdecl user_interface_networking_reset()
//.text:00A7F920 ; bool __cdecl user_interface_squad_set_closed_by_user(bool closed_by_user)
//.text:00A7F930 ; void __cdecl user_interface_networking_set_maximum_party_size(long maximum_party_size)
//.text:00A7F950 ; bool __cdecl user_interface_squad_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode)

void __cdecl user_interface_networking_set_start_game_when_ready(bool a1, long controller_index)
{
	INVOKE(0x00A7FAC0, user_interface_networking_set_start_game_when_ready, a1, controller_index);
}

void __cdecl user_interface_networking_set_ui_upload_quota(long ui_upload_quota)
{
	INVOKE(0x00A7FB00, user_interface_networking_set_ui_upload_quota, ui_upload_quota);
}

bool __cdecl user_interface_networking_should_allow_team_switching()
{
	return INVOKE(0x00A7FB20, user_interface_networking_should_allow_team_switching);
}

bool __cdecl sub_A7FB70()
{
	return INVOKE(0x00A7FB70, sub_A7FB70);
}

bool __cdecl user_interface_networking_should_show_teams()
{
	return INVOKE(0x00A7FBF0, user_interface_networking_should_show_teams);
}

void __cdecl user_interface_networking_show_signed_out_while_in_live_alert()
{
	INVOKE(0x00A7FCB0, user_interface_networking_show_signed_out_while_in_live_alert);
}

void __cdecl user_interface_networking_start_game_when_ready_update()
{
	INVOKE(0x00A7FCD0, user_interface_networking_start_game_when_ready_update);
}

void __cdecl user_interface_networking_update()
{
	INVOKE(0x00A7FE50, user_interface_networking_update);
}

bool __cdecl user_interface_networking_xbox_guide_was_active_recently()
{
	return INVOKE(0x00A7FE70, user_interface_networking_xbox_guide_was_active_recently);
}

bool __cdecl user_interface_reset_networking_to_pregame()
{
	//return INVOKE(0x00A7FEC0, user_interface_reset_networking_to_pregame);

	// halo online inplementation
	//return network_squad_session_local_peer_is_leader() && network_life_cycle_set_pre_game_state();

	if (!network_squad_session_local_peer_is_leader())
	{
		generate_event(_event_level_error, "ui:networking:user_interface_reset_networking_to_pregame: we are not the leader of a custom game");
		return false;
	}

	if (!network_life_cycle_set_pre_game_state())
	{
		generate_event(_event_level_error, "ui:networking:user_interface_reset_networking_to_pregame: failed to return networking to pregame mode");
		return false;
	}

	return true;
}

void __cdecl user_interface_sanitize_game_setup_preferences_for_remote_squad_join()
{
	INVOKE(0x00A7FEF0, user_interface_sanitize_game_setup_preferences_for_remote_squad_join);
}

// game_status->connection_quality
long __cdecl sub_A7FFA0(long player_index)
{
	return INVOKE(0x00A7FFA0, sub_A7FFA0, player_index);
}

void __cdecl user_interface_set_desired_multiplayer_mode(e_desired_multiplayer_mode desired_multiplayer_mode)
{
	INVOKE(0x00A7FFB0, user_interface_set_desired_multiplayer_mode, desired_multiplayer_mode);
}

//.text:00A80060 ; void __cdecl user_interface_simulation_describe_status(string_id* status_string, real* progress)

