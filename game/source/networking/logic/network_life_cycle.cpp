#include "networking/logic/network_life_cycle.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"
#include "text/unicode.hpp"

REFERENCE_DECLARE(0x019AB7F0, s_network_life_cycle_globals, life_cycle_globals);

void __cdecl network_life_cycle_disconnect_sessions()
{
	INVOKE(0x00454A30, network_life_cycle_disconnect_sessions);
}

void __cdecl network_life_cycle_dispose()
{
	INVOKE(0x00454A80, network_life_cycle_dispose);

	//multiplayer_game_hopper_dispose();
	//logic_qos_reply_manager_dispose();
	//
	//life_cycle_globals.handler_none.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_pre_game.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_start_game.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_game.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_game_write_stats.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_leaving.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_joining.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_start.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match_client.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_and_assemble_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_assemble_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_select_host.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_arbitration.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_prepare_map.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_match.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_match_write_stats.dispose(&life_cycle_globals.manager);
	//life_cycle_globals.handler_post_match.dispose(&life_cycle_globals.manager);
	//
	//life_cycle_globals.initialized = false;
}

void __cdecl network_life_cycle_end()
{
	//INVOKE(0x00454B40, network_life_cycle_end);

	generate_event(_event_level_message, "networking:lifecycle: calling life cycle end");
	
	network_life_cycle_disconnect_sessions();
	life_cycle_globals.manager.terminate();
}

void __cdecl network_life_cycle_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	INVOKE(0x00454C20, network_life_cycle_get_matchmaking_progress, progress_out);

	//csmemset(progress_out, 0, sizeof(s_life_cycle_matchmaking_progress));
	//progress_out->progress_type = 0;
	//
	//if (life_cycle_globals.manager.current_state_ready_for_state_transition_or_query())
	//{
	//	e_life_cycle_state state = network_life_cycle_get_state();
	//	switch (state)
	//	{
	//	case _life_cycle_state_matchmaking_start:
	//		life_cycle_globals.handler_matchmaking_start.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_find_match_client:
	//		life_cycle_globals.handler_matchmaking_find_match_client.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_find_match:
	//		life_cycle_globals.handler_matchmaking_find_match.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_find_and_assemble_match:
	//		life_cycle_globals.handler_matchmaking_find_and_assemble_match.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_assemble_match:
	//		life_cycle_globals.handler_matchmaking_assemble_match.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_arbitration:
	//		life_cycle_globals.handler_matchmaking_arbitration.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_select_host:
	//		life_cycle_globals.handler_matchmaking_select_host.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_matchmaking_prepare_map:
	//		life_cycle_globals.handler_matchmaking_prepare_map.get_progress(progress_out);
	//		break;
	//	case _life_cycle_state_post_match:
	//		life_cycle_globals.handler_post_match.get_progress(progress_out);
	//		break;
	//	}
	//}
}

e_life_cycle_state __cdecl network_life_cycle_get_state()
{
	//return INVOKE(0x00454DB0, network_life_cycle_get_state);

	if (life_cycle_globals.initialized)
		return life_cycle_globals.manager.m_current_state;

	return _life_cycle_state_none;
}

bool __cdecl network_life_cycle_in_squad_session(c_network_session** out_active_squad_session)
{
	//return INVOKE(0x00454F20, network_life_cycle_in_squad_session, out_active_squad_session);

	if (life_cycle_globals.initialized)
	{
		c_network_session* active_squad_session = life_cycle_globals.manager.get_active_squad_session();
		if (!active_squad_session->disconnected())
		{
			if (out_active_squad_session)
				*out_active_squad_session = active_squad_session;

			return true;
		}
	}

	return false;
}

bool __cdecl network_life_cycle_in_system_link_advertisable_session(c_network_session** out_active_system_link_advertisable_session)
{
	//return INVOKE(0x00454F50, network_life_cycle_in_system_link_advertisable_session, out_active_system_link_advertisable_session);

	if (life_cycle_globals.initialized)
	{
		c_network_session* active_system_link_advertisable_session = life_cycle_globals.manager.get_active_squad_session();
		if (active_system_link_advertisable_session->is_host() &&
			active_system_link_advertisable_session->session_class() == _network_session_class_system_link &&
			active_system_link_advertisable_session->has_managed_session_connection())
		{
			if (out_active_system_link_advertisable_session)
				*out_active_system_link_advertisable_session = active_system_link_advertisable_session;

			return true;
		}
	}

	return false;
}

bool __cdecl network_life_cycle_initialize(c_network_observer* observer, c_network_session_manager* session_manager, c_network_session* squad_session_one, c_network_session* squad_session_two, c_network_session* group_session)
{
	return INVOKE(0x00454FD0, network_life_cycle_initialize, observer, session_manager, squad_session_one, squad_session_two, group_session);

	//life_cycle_globals.manager.initialize(observer, session_manager, squad_session_one, squad_session_two, group_session);
	//
	//life_cycle_globals.handler_none.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_pre_game.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_start_game.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_game.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_game_write_stats.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_leaving.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_joining.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_start.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match_client.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_find_and_assemble_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_assemble_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_select_host.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_arbitration.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_matchmaking_prepare_map.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_in_match.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_end_match_write_stats.initialize(&life_cycle_globals.manager);
	//life_cycle_globals.handler_post_match.initialize(&life_cycle_globals.manager);
	//
	//multiplayer_game_hopper_initialize();
	//logic_qos_reply_manager_initialize();
	//
	//life_cycle_globals.initialized = true;
	//return true;
}

void __cdecl network_life_cycle_request_leave(bool disconnect)
{
	//return INVOKE(0x00455260, network_life_cycle_request_leave, disconnect);

	life_cycle_globals.manager.request_leave_sessions(disconnect);
}

bool __cdecl network_life_cycle_set_pre_game_state()
{
	//return INVOKE(0x00455280, network_life_cycle_set_pre_game_state);

	ASSERT(life_cycle_globals.initialized);

	if (life_cycle_globals.manager.get_current_state())
	{
		if (life_cycle_globals.manager.get_current_state() == _life_cycle_state_pre_game)
			return true;

		c_network_session* active_squad_session = life_cycle_globals.manager.get_active_squad_session();
		if (active_squad_session->is_leader())
		{
			c_network_session_parameters* session_parameters = active_squad_session->get_session_parameters();
			if (session_parameters->session_mode.request_change(_network_session_mode_idle))
				return true;
		}
	}

	return false;
}

void __cdecl network_life_cycle_update()
{
	INVOKE(0x00455390, network_life_cycle_update);

	//if (life_cycle_globals.initialized)
	//{
	//	multiplayer_game_hopper_update();
	//	life_cycle_globals.manager.update();
	//	logic_qos_reply_manager_update();
	//}
}

