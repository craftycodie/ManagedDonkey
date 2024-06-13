#include "networking/network_globals.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "data_mining/data_mine_management.hpp"
#include "game/game.hpp"
#include "game/game_options.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "multithreading/threads.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/logic/logic_session_tracker.hpp"
#include "networking/logic/network_arbitration.hpp"
#include "networking/logic/network_bandwidth.hpp"
#include "networking/logic/network_banhammer.hpp"
#include "networking/logic/network_leaderboard.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/logic/network_recruiting_search.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/logic/storage/network_http_request_cache.hpp"
#include "networking/logic/storage/network_http_request_queue.hpp"
#include "networking/logic/storage/network_storage_cache.hpp"
#include "networking/logic/storage/network_storage_files.hpp"
#include "networking/logic/storage/network_storage_manifest.hpp"
#include "networking/logic/storage/network_storage_queue.hpp"
#include "networking/messages/network_messages_connect.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/messages/network_messages_session_membership.hpp"
#include "networking/messages/network_messages_session_parameters.hpp"
#include "networking/messages/network_messages_session_protocol.hpp"
#include "networking/messages/network_messages_simulation.hpp"
#include "networking/messages/network_messages_simulation_distributed.hpp"
#include "networking/messages/network_messages_simulation_synchronous.hpp"
#include "networking/messages/network_messages_test.hpp"
#include "networking/messages/network_messages_text_chat.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_memory.hpp"
#include "networking/network_time.hpp"
#include "networking/online/online_files.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/online/online_presence_pc.hpp"
#include "networking/online/online_url.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameter_types.hpp"
#include "networking/transport/transport.hpp"
#include "objects/object_placement.hpp"
#include "profiler/profiler.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "tag_files/tag_groups.hpp"
#include "xbox/xnet.hpp"

REFERENCE_DECLARE(0x0224A490, c_network_session_parameter_type_collection*, g_network_parameter_types);
REFERENCE_DECLARE(0x0224A494, c_network_link*, g_network_link);
REFERENCE_DECLARE(0x0224A498, c_network_message_type_collection*, g_network_message_types);
REFERENCE_DECLARE(0x0224A49C, c_network_message_gateway*, g_network_message_gateway);
REFERENCE_DECLARE(0x0224A4A0, c_network_message_handler*, g_network_message_handler);
REFERENCE_DECLARE(0x0224A4A4, c_network_observer*, g_network_observer);
REFERENCE_DECLARE(0x0224A4A8, c_network_session*, g_network_sessions);
REFERENCE_DECLARE(0x0224A4AC, c_network_session_parameter_type_collection*, g_network_session_parameter_types);
REFERENCE_DECLARE(0x0224A4B0, c_network_session_manager*, g_network_session_manager);
REFERENCE_DECLARE(0x0224A4B4, s_network_globals, network_globals);

HOOK_DECLARE(0x0049E050, network_dispose);
HOOK_DECLARE(0x0049E1B0, network_initialize);
HOOK_DECLARE(0x0049E7B0, network_update);
HOOK_DECLARE_CALL(0x0049E200, network_memory_base_initialize);

//bool g_network_status_memory = false;
//bool g_network_status_link = false;
//bool g_network_status_simulation = false;
//bool g_network_status_channels = false;
//bool g_network_status_connections = false;
//bool g_network_status_message_queues = false;
//bool g_network_status_observer = false;
//bool g_network_status_sessions = false;
//bool g_network_status_leaderboard = false;
//c_status_line g_network_memory_status_line[1]{};
//c_status_line g_network_link_status_line[1]{};
//c_status_line g_network_simulation_status_lines[2]{};
//c_status_line g_network_channel_status_lines[2]{};
//c_status_line g_network_connection_status_lines[4]{};
//c_status_line g_network_message_queue_status_lines[2]{};
//c_status_line g_network_observer_status_lines[5]{};
//c_status_line g_network_session_status_lines[7]{};
//c_status_line g_network_leaderboard_query_status_lines[4]{};
//c_status_line g_network_leaderboard_write_status_lines[6]{};

c_network_message_type_collection custom_message_types_override = {};
c_network_message_gateway custom_message_gateway_override = {};

#define UI_WAIT(_time, _set_value, _get_value, _value) \
_set_value(_value);                                    \
do                                                     \
{                                                      \
    user_interface_update(_time);                      \
    network_update();                                  \
} while (_get_value() != _value);

// comment out until `network_initialize` is implemented
#define NETWORK_ENTER_AND_LOCK_TIME \
ASSERT(network_globals.entered == false); \
ASSERT(network_globals.thread_id == system_get_current_thread_id()); \
network_globals.entered = true; \
network_time_lock(true)

#define NETWORK_EXIT_AND_UNLOCK_TIME \
network_time_lock(false); \
ASSERT(network_globals.entered == true); \
ASSERT(network_globals.thread_id == system_get_current_thread_id()); \
network_globals.entered = false

//#define NETWORK_ENTER_AND_LOCK_TIME \
//ASSERT(network_globals.entered == false); \
//network_globals.entered = true; \
//network_time_lock(true)
//
//#define NETWORK_EXIT_AND_UNLOCK_TIME \
//network_time_lock(false); \
//ASSERT(network_globals.entered == true); \
//network_globals.entered = false

void __cdecl network_memory_base_dispose()
{
	INVOKE(0x00462350, network_memory_base_dispose);
}

bool __cdecl network_memory_base_initialize(
	c_network_link** link,
	c_network_message_type_collection** message_types,
	c_network_message_gateway** message_gateway,
	c_network_message_handler** message_handler,
	c_network_observer** observer,
	c_network_session** sessions,
	c_network_session_manager** session_manager,
	c_network_session_parameter_type_collection** session_parameter_types)
{
	ASSERT(link);
	ASSERT(message_types);
	ASSERT(message_gateway);
	ASSERT(message_handler);
	ASSERT(observer);
	ASSERT(sessions);
	ASSERT(session_parameter_types);

	bool result = INVOKE(0x004623F0, network_memory_base_initialize, link, message_types, message_gateway, message_handler, observer, sessions, session_manager, session_parameter_types);

	//*link = &network_base_memory_globals.link;
	//*message_types = &network_base_memory_globals.message_types;
	//*message_gateway = &network_base_memory_globals.message_gateway;
	//*message_handler = &network_base_memory_globals.message_handler;
	//*observer = &network_base_memory_globals.observer;
	//*sessions = network_base_memory_globals.sessions;
	//*session_manager = &network_base_memory_globals.session_manager;
	//*session_parameter_types = &network_base_memory_globals.sesssion_parameter_types;

	*message_types = &custom_message_types_override;
	*message_gateway = &custom_message_gateway_override;

	return result;
}

void __cdecl network_dispose()
{
	//INVOKE(0x0049E050, network_dispose);

	if (network_globals.initialized)
	{
		network_storage_cache_dispose();
		network_storage_manifest_dispose();
		network_storage_dispose();
		network_storage_files_dispose();
		network_http_request_queue_dispose();
		online_files_dispose();
		online_rich_presence_dispose();
		online_guide_dispose();
		online_lsp_dispose();
		online_url_dispose();
		online_dispose();
		network_bandwidth_dispose();
		network_leaderboard_destory();
		network_arbitration_destory();
		network_session_interface_dispose();
		network_life_cycle_dispose();
		network_search_dispose();
		network_recruiting_search_dispose();
		network_broadcast_search_dispose();
		network_session_tracker_dispose();
	
		for (long session_index = 0; session_index < 3; session_index++)
			g_network_sessions[session_index].destroy_session();
	
		g_network_observer->destroy_observer();
		g_network_session_manager->destroy_session_manager();
		g_network_session_parameter_types->clear_session_parameter_types();
	
		online_session_manager_dispose();
		network_memory_shared_dispose();
	
		g_network_message_handler->destroy_handler();
		g_network_message_gateway->destroy_gateway();
		g_network_message_types->clear_message_types();
		g_network_link->destroy_link();
	
		network_globals.initialized = false;
		g_network_link = NULL;
		g_network_message_types = NULL;
		g_network_message_gateway = NULL;
		g_network_message_handler = NULL;
		g_network_observer = NULL;
		g_network_sessions = NULL;
	
		network_memory_base_dispose();
		network_configuration_dispose();
	}
}

c_network_session_manager* __cdecl network_get_session_manager()
{
	//return INVOKE(0x0049E1A0, network_get_session_manager);

	return g_network_session_manager;
}

void __cdecl network_initialize()
{
	//INVOKE(0x0049E1B0, network_initialize);

	if (shell_application_type() == _shell_application_type_client && !network_globals.initialized)
	{
		network_globals.thread_id = system_get_current_thread_id();
		network_configuration_initialize(false);
	
		if (network_memory_base_initialize(
			&g_network_link,
			&g_network_message_types,
			&g_network_message_gateway,
			&g_network_message_handler,
			&g_network_observer,
			&g_network_sessions,
			&g_network_session_manager,
			&g_network_session_parameter_types))
		{
			ASSERT(g_network_link != NULL);
			ASSERT(g_network_message_types != NULL);
			ASSERT(g_network_message_gateway != NULL);
			ASSERT(g_network_message_handler != NULL);
			ASSERT(g_network_observer != NULL);
			ASSERT(g_network_sessions != NULL);
			ASSERT(g_network_session_manager != NULL);
			ASSERT(g_network_session_parameter_types);
	
			bool success = g_network_link->initialize_link();
			g_network_message_types->clear_message_types();
			network_message_types_register_out_of_band(g_network_message_types);
			network_message_types_register_connect(g_network_message_types);
			network_message_types_register_session_protocol(g_network_message_types);
			network_message_types_register_session_membership(g_network_message_types);
			network_message_types_register_session_parameters(g_network_message_types);
			network_message_types_register_simulation(g_network_message_types);
			network_message_types_register_simulation_synchronous(g_network_message_types);
			network_message_types_register_simulation_distributed(g_network_message_types);
			network_message_types_register_text_chat(g_network_message_types);
			network_message_types_register_test(g_network_message_types);
	
			success |=
				g_network_message_gateway->initialize_gateway(g_network_link, g_network_message_types) &&
				g_network_message_handler->initialize_handler(g_network_link, g_network_message_types, g_network_message_gateway) &&
				g_network_observer->initialize_observer(g_network_link, g_network_message_types, g_network_message_gateway, g_network_message_handler, &g_network_configuration.observer_configuration);
	
			g_network_message_handler->register_observer(g_network_observer);

			g_network_session_parameter_types->clear_session_parameter_types();
			network_session_parameter_types_register(g_network_session_parameter_types);
			g_network_session_parameter_types->check_session_parameter_types();
			network_session_parameters_register_parameter_type_collection(g_network_session_parameter_types);
	
			success |= g_network_session_manager->initialize_session_manager();
			network_session_time_register_session_manager(g_network_session_manager);
			g_network_message_handler->register_session_manager(g_network_session_manager);
	
			success |= 
				g_network_sessions[0].initialize_session(0, _network_session_type_squad, g_network_message_gateway, g_network_observer, g_network_session_manager) &&
				g_network_sessions[1].initialize_session(1, _network_session_type_squad, g_network_message_gateway, g_network_observer, g_network_session_manager) &&
				g_network_sessions[2].initialize_session(2, _network_session_type_group, g_network_message_gateway, g_network_observer, g_network_session_manager);
	
			online_session_manager_initialize();
	
			if (success
				&& network_banhammer_initialize()
				&& network_broadcast_search_initialize(g_network_link, g_network_message_gateway)
				&& network_recruiting_search_initialize()
				&& network_search_initialize()
				&& network_life_cycle_initialize(g_network_observer, g_network_session_manager, &g_network_sessions[0], &g_network_sessions[1], &g_network_sessions[2])
				&& network_session_interface_initialize(g_network_session_manager)
				&& network_leaderboard_initialize()
				&& network_arbitration_initialize()
				&& network_bandwidth_initialize(g_network_observer, &g_network_configuration.bandwidth_configuration)
				&& network_session_tracker_initialize())
			{
				transport_register_transition_functions(network_startup_transport, network_shutdown_transport, nullptr, nullptr);
				online_initialize();
				online_url_initialize();
				network_storage_initialize();
				network_storage_manifest_initialize();
				network_storage_cache_initialize();
				online_lsp_initialize();
				online_guide_initialize();
				online_rich_presence_initialize();
				online_files_initialize();
				network_http_request_queue_initialize();
				network_storage_files_initialize();
	
				//status_lines_initialize(g_network_memory_status_line, &g_network_status_memory, 1);
				//status_lines_initialize(g_network_link_status_line, &g_network_status_link, 1);
				//status_lines_initialize(g_network_simulation_status_lines, &g_network_status_simulation, 2);
				//status_lines_initialize(g_network_channel_status_lines, &g_network_status_channels, 32);
				//status_lines_initialize(g_network_connection_status_lines, &g_network_status_connections, 64);
				//status_lines_initialize(g_network_message_queue_status_lines, &g_network_status_message_queues, 32);
				//status_lines_initialize(g_network_observer_status_lines, &g_network_status_observer, 65);
				//status_lines_initialize(g_network_session_status_lines, &g_network_status_sessions, 7);
				//status_lines_initialize(g_network_leaderboard_query_status_lines, &g_network_status_leaderboard, 4);
				//status_lines_initialize(g_network_leaderboard_write_status_lines, &g_network_status_leaderboard, 16);
			}
	
			if (success)
			{
				network_set_online_environment(false);
				network_globals.initialized = true;

				get_external_ip();
			}
			else
			{
				generate_event(_event_level_warning, "network_globals_initialize(): failed to initialize networking");
				network_dispose();
			}
		}
		else
		{
			generate_event(_event_level_warning, "network_globals_initialize(): failed to initialize base networking memory layer");
		}
	}
}

bool __cdecl network_initialized()
{
	//return INVOKE(0x0049E5A0, network_initialized);

	return network_globals.initialized;
}

void __cdecl network_idle()
{
	//INVOKE(0x0049E5B0, network_idle);

	ASSERT(is_main_thread());

	PROFILER(networking_idle)
	{
		if (!network_globals.entered)
		{
			network_receive();
			network_update();
			network_send();
		}
	}
}

void __cdecl network_receive()
{
	//INVOKE(0x0049E600, network_receive);

	ASSERT(is_main_thread());

	PROFILER(networking_receive)
	{
		if (network_initialized())
		{
			NETWORK_ENTER_AND_LOCK_TIME;

			g_network_link->process_incoming_packets();

			NETWORK_EXIT_AND_UNLOCK_TIME;
		}
	}
}

void __cdecl network_send()
{
	//INVOKE(0x0049E640, network_send);

	ASSERT(is_main_thread());

	PROFILER(networking_send)
	{
		if (network_initialized())
		{
			NETWORK_ENTER_AND_LOCK_TIME;

			for (long i = 0; i < k_network_session_type_count; i++)
				g_network_sessions[i].idle();

			g_network_observer->monitor();
			simulation_prepare_to_send();
			g_network_link->process_all_channels();
			g_network_message_gateway->send_all_pending_messages();

			NETWORK_EXIT_AND_UNLOCK_TIME;
		}
	}
}

void __cdecl network_set_online_environment(bool online_environment)
{
	INVOKE(0x0049E6C0, network_set_online_environment, online_environment);
}

void __cdecl network_shutdown_transport(void* userdata)
{
	//INVOKE(0x0049E6E0, network_shutdown_transport, userdata);

	if (network_initialized())
	{
		generate_event(_event_level_error, "networking:global: network terminating due to transport shutdown");
	
		if (g_network_link)
			g_network_link->destroy_endpoints();
	
		if (g_network_observer)
			g_network_observer->set_online_network_environment(false);
	}
}

void __cdecl network_startup_transport(void* userdata)
{
	//INVOKE(0x0049E770, network_startup_transport, userdata);

	if (network_initialized())
	{
		if (g_network_link)
			g_network_link->create_endpoints();
	}
}

void __cdecl network_update()
{
	//INVOKE(0x0049E7B0, network_update);

	ASSERT(is_main_thread());
	
	PROFILER(networking)
	{
		transport_global_update();
	
		if (network_initialized())
		{
			NETWORK_ENTER_AND_LOCK_TIME;
	
			network_configuration_update();
			network_bandwidth_update();
			network_broadcast_search_update();
			network_recruiting_search_update();
			network_session_tracker_update();
			network_session_interface_update();
			network_join_update();
			network_life_cycle_update();
			network_banhammer_update();
			online_session_manager_update();
			online_update();
			network_leaderboard_update();
			network_arbitration_update();
			network_storage_queue_update();
			network_storage_manifest_update();
			network_storage_cache_update();
			data_mine_update();
			network_webstats_update();
			online_guide_update();
			online_rich_presence_update();
			online_files_update();
			network_http_request_cache_update();
			network_http_request_queue_update();
			network_storage_files_update();
			c_online_lsp_manager::get()->update();
	
			NETWORK_EXIT_AND_UNLOCK_TIME;
		}
	}
}

void __cdecl network_test_set_map_name(char const* scenario_path)
{
	if (network_squad_session_set_map(-1, -2, scenario_path))
	{
		console_printf("set %s successfully", scenario_path);

		static c_static_wchar_string<256> map_path;
		static c_map_variant map_variant;

		map_path.print(L"%hs", tag_name_strip_path(scenario_path));
		long multiplayer_map = levels_get_multiplayer_map_by_display_name(map_path.get_string());
		map_variant.create_default(multiplayer_map);

		network_test_set_map_variant(&map_variant);

		//BUILD_DEFAULT_GAME_VARIANT(game_variant, _game_engine_type_slayer);
		//network_test_set_game_variant(&game_variant);
	}
	else
	{
		generate_event(_event_level_warning, "unable to set map %s", scenario_path);
	}
}

void __cdecl network_test_set_map_variant(c_map_variant const* map_variant)
{
	if (!user_interface_squad_set_map_variant(map_variant))
	{
		static c_map_variant default_map_variant;
		long multiplayer_map = levels_get_default_multiplayer_map_id();
		default_map_variant.create_default(multiplayer_map);
		user_interface_squad_set_map_variant(&default_map_variant);
	}
}

void __cdecl network_test_set_game_variant(e_game_engine_type game_engine_index)
{
	static c_game_variant game_variant;
	BUILD_DEFAULT_GAME_VARIANT(game_variant, game_engine_index);

	if (!user_interface_squad_set_game_variant(&game_variant))
	{
		static c_game_variant default_game_variant;
		BUILD_DEFAULT_GAME_VARIANT(default_game_variant, _game_engine_type_slayer);
		user_interface_squad_set_game_variant(&default_game_variant);
	}
}

void __cdecl network_test_set_game_variant(char const* game_engine_name)
{
	e_game_engine_type game_engine_index = _game_engine_type_none;

	for (long i = _game_engine_type_none; i < k_game_engine_type_count; i++)
	{
		if (csstricmp(game_engine_name, game_engine_type_get_string(i)) != 0)
			continue;

		game_engine_index = e_game_engine_type(i);
	}

	network_test_set_game_variant(game_engine_index);
}

void __cdecl network_test_set_session_mode(char const* session_mode_name)
{
	e_network_session_mode session_mode = _network_session_mode_none;

	for (long i = _network_session_mode_none; i < k_network_session_mode_count; i++)
	{
		if (csstricmp(session_mode_name, network_session_mode_get_name(i)) != 0)
			continue;

		session_mode = e_network_session_mode(i);
	}

	network_squad_session_set_session_mode(session_mode);
}

void __cdecl network_test_set_ui_game_mode(char const* ui_game_mode_name)
{
	e_gui_game_mode ui_game_mode = _ui_game_mode_none;

	for (long i = _ui_game_mode_campaign; i < k_ui_game_mode_count; i++)
	{
		if (csstricmp(ui_game_mode_name, ui_game_mode_get_name(i)) != 0)
			continue;

		ui_game_mode = e_gui_game_mode(i);
	}

	UI_WAIT(0.1f, user_interface_networking_enter_pregame_location, user_interface_squad_get_ui_game_mode, ui_game_mode);
}

void __cdecl network_test_set_advertisement_mode(char const* advertisement_mode_name)
{
	e_gui_network_session_advertisement_mode advertisement_mode = _gui_network_session_advertisement_mode_invalid;

	for (long i = _gui_network_session_advertisement_mode_open_to_public; i < k_gui_network_session_advertisement_mode_count; i++)
	{
		if (csstricmp(advertisement_mode_name, gui_network_session_advertisement_mode_get_name(i)) != 0)
			continue;

		advertisement_mode = e_gui_network_session_advertisement_mode(i);
	}

	UI_WAIT(0.1f, user_interface_squad_set_session_advertisement, user_interface_networking_get_session_advertisement, advertisement_mode);
}

void __cdecl network_test_set_game_variant_parameter(char const* parameter_name, long value, long* old_value)
{
	e_game_variant_parameter parameter = k_game_variant_parameter_none;

	for (long i = _game_variant_parameter_game_misc_teams; i < k_game_variant_parameter_count; i++)
	{
		if (csstricmp(parameter_name, game_variant_parameter_get_name(i)) != 0)
			continue;

		parameter = e_game_variant_parameter(i);
	}

	c_game_variant* game_variant = new c_game_variant();
	game_variant->copy_from_and_validate(network_life_cycle_session_get_game_variant());

	if (*old_value)
		game_variant->get_integer_game_engine_setting(parameter, old_value);

	game_variant->set_integer_game_engine_setting(parameter, value);
	user_interface_squad_set_game_variant(game_variant);
	delete game_variant;
}

void __cdecl network_test_ping()
{
	static word id = 0;
	if (network_initialized())
	{
		s_network_message_ping ping =
		{
			.id = id++,
			.timestamp = network_time_get(),
			.request_qos = false
		};

		generate_event(_event_level_message, "networking:test:ping: ping #%d sent at local %dms", id, network_time_get_exact());
		for (word broadcast_port = k_broadcast_port; broadcast_port < k_broadcast_port + k_broadcast_port_alt_ammount; broadcast_port++)
			g_network_message_gateway->send_message_broadcast(_network_message_ping, sizeof(s_network_message_ping), &ping, broadcast_port);
	}
	else
	{
		generate_event(_event_level_error, "networking:test: networking is not initialized");
	}
}

void __cdecl network_test_reset_objects()
{
	c_object_iterator<object_datum> object_iterator;
	object_iterator.begin(NONE, 0);
	while (object_iterator.next())
	{
		if (!simulation_query_object_is_predicted(object_iterator.get_index()))
			object_delete_immediately(object_iterator.get_index());
	}

	object_placement_create_global_objects(game_mode_get(), false);
	object_placement_create_active_zone_set_objects(_object_placement_zone_set_create_mode_unknown0);
}

void __cdecl network_test_ping_directed(transport_address const* address)
{
	static word id = 0;
	if (network_initialized())
	{
		s_network_message_ping ping =
		{
			.id = id++,
			.timestamp = network_time_get(),
			.request_qos = false
		};

		generate_event(_event_level_message, "networking:test:ping: ping #%d sent at local %dms", id, network_time_get_exact());
		g_network_message_gateway->send_message_directed(address, _network_message_ping, sizeof(s_network_message_ping), &ping);
	}
	else
	{
		generate_event(_event_level_error, "networking:test: networking is not initialized");
	}
}

void __cdecl network_test_text_chat(char const* text)
{
	if (network_initialized())
	{
		static s_network_message_text_chat text_chat{};
		csmemset(&text_chat, 0, sizeof(s_network_message_text_chat));

		text_chat.payload.destination_player_count = 16;
		text_chat.payload.text.print(L"%hs", text);

		for (word broadcast_port = k_broadcast_port; broadcast_port < k_broadcast_port + k_broadcast_port_alt_ammount; broadcast_port++)
			g_network_message_gateway->send_message_broadcast(_custom_network_message_text_chat, sizeof(s_network_message_text_chat), &text_chat, broadcast_port);
	}
	else
	{
		generate_event(_event_level_error, "networking:test: networking is not initialized");
	}
}

void __cdecl network_test_text_chat_directed(transport_address const* address, char const* text)
{
	if (network_initialized())
	{
		static s_network_message_text_chat text_chat{};
		csmemset(&text_chat, 0xFF, sizeof(s_network_message_text_chat));
		csmemset(text_chat.payload.text_buffer, 0, sizeof(text_chat.payload.text_buffer));

		text_chat.payload.destination_player_count = 16;
		text_chat.payload.text.print(L"%hs", text);

		g_network_message_gateway->send_message_directed(address, _custom_network_message_text_chat, sizeof(s_network_message_text_chat), &text_chat);
	}
	else
	{
		generate_event(_event_level_error, "networking:test: networking is not initialized");
	}
}

