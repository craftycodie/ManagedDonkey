#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"

#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "life_cycle_manager.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/session/network_session.hpp"
#include <simulation/simulation_watcher.hpp>
#include <game/game_engine_util.hpp>
#include <memory/thread_local.hpp>
#include "simulation/simulation.hpp"
#include <editor/editor_stubs.hpp>
#include <math/random_math.hpp>
#include "objects/objects.hpp"
#include <game/game_engine_team.hpp>
#include <simulation/game_interface/simulation_game_action.hpp>
#include <game/game_engine_spawning.hpp>





HOOK_DECLARE_CLASS_MEMBER(0x00494A10, c_life_cycle_state_handler_matchmaking_arbitration, mark_arbitration_complete);
void __thiscall c_life_cycle_state_handler_matchmaking_arbitration::mark_arbitration_complete() {
    HOOK_INVOKE_CLASS_MEMBER(, c_life_cycle_state_handler_matchmaking_arbitration, mark_arbitration_complete);
}

HOOK_DECLARE_CLASS_MEMBER(0x00494EF0, c_life_cycle_state_handler_matchmaking_arbitration, update);
void __thiscall c_life_cycle_state_handler_matchmaking_arbitration::update()
{
    c_console::write_line("donkey:matchmaking c_life_cycle_state_handler_matchmaking_arbitration:update");

    c_network_session* session = this->m_manager->get_group_session();

    // Arbitration Progression...

    if (session->is_host() && !(this->m_flags & 0x80)) {
        c_console::write_line("donkey:matchmaking:arbitration we're hosting, let's do some fudging!");

        this->mark_arbitration_complete();
        this->setup_initial_participants(session);
        this->m_flags |= 0x80;
    }

    HOOK_INVOKE_CLASS_MEMBER(, c_life_cycle_state_handler_matchmaking_arbitration, update);
}



char __cdecl network_arbitration_initiate(int managed_session_index, qword nonce)
{
    c_console::write_line("donkey:matchmaking:arbitration network_arbitration_initiate");

    int session; // esi

    session = managed_session_index;
    network_arbitration_globals.arbitration_status = _network_arbitration_status_registered;
    network_arbitration_globals.managed_session_index = managed_session_index;
    return 1;
}
HOOK_DECLARE(0x004D9B60, network_arbitration_initiate);

bool __cdecl c_life_cycle_state_handler_matchmaking_arbitration__session_membership_matches_arbitration_registration_results(void*) {
    c_console::write_line("donkey:matchmaking:arbitration c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results");
    return 1;
}
HOOK_DECLARE(0x00494C80, c_life_cycle_state_handler_matchmaking_arbitration__session_membership_matches_arbitration_registration_results);

HOOK_DECLARE_CLASS_MEMBER(0x00494AF0, c_life_cycle_state_handler_matchmaking_arbitration, ready_to_start);
bool __thiscall c_life_cycle_state_handler_matchmaking_arbitration::ready_to_start()
{
    auto m_group_session = this->m_manager->m_group_session;
    bool arbitration_completed = 0;
    bool all_players_have_initial_participants = 0;
    bool all_players_session_started = 0;
    bool stats_written = 0;

    c_console::write_line("donkey:matchmaking:arbitration c_life_cycle_state_handler_matchmaking_arbitration::ready_to_start");

    const c_network_session_membership* session_membership = m_group_session->get_session_membership();

    if ((this->m_flags & 8) != 0) // arbitration finished ?
        arbitration_completed = (this->m_flags & 0x100) == 0; // arbitration failed
    if ((this->m_flags & 0x80) != 0)
    {
        auto session_parameters = m_group_session->get_session_parameters();
        all_players_have_initial_participants = session_parameters->parameters_transmitted_to_peers(1 << _network_session_parameter_type_initial_participants) != 0;
        if (!all_players_have_initial_participants) c_console::write_line("donkey:matchmaking:arbitration Warning! It seems a player hasn't got initial participants!");
    }
    if ((this->m_flags & 0x400) != 0 && (this->m_flags & 0x800) == 0) // 0x400 == start match completed, 0x800 == start match failed
    {
        all_players_session_started = session_membership->peer_property_flag_test(0, 6) != 0; // 6 == start match complete
        if (!all_players_session_started) c_console::write_line("donkey:matchmaking:arbitration Warning! It seems a peer hasn't finished starting their session!");
    }
    if ((this->m_flags & 0x2000) == 0 || (this->m_flags & 0x4000) != 0) // 0x2000 == stats write completed? 0x4000 == stats write failed?
    {
        stats_written = 0;
    }
    else
    {
        stats_written = session_membership->peer_property_flag_test(0, 8) != 0; // initial stats write completd
    }

    if ((this->m_flags & 0x800) == 1) {
        c_console::write_line("donkey:matchmaking:arbitration Warning! Match start has failed!");
    }

    if ((this->m_flags & 0x4000) == 1) {
        c_console::write_line("donkey:matchmaking:arbitration Warning! Stats write has failed!");
    }

    c_console::write_line("donkey:matchmaking:arbitration Arbitration completed? %s", arbitration_completed ? "YES" : "NO");
    c_console::write_line("donkey:matchmaking:arbitration Peers have initial participants? %s", all_players_have_initial_participants ? "YES" : "NO");
    c_console::write_line("donkey:matchmaking:arbitration Session started? %s", all_players_session_started ? "YES" : "NO");
    c_console::write_line("donkey:matchmaking:arbitration Stats written? %s", stats_written ? "YES" : "NO");
    c_console::write_line("donkey:matchmaking:arbitration Are we ready? %s", arbitration_completed && all_players_have_initial_participants && all_players_session_started && stats_written ? "YES" : "NO");

    return arbitration_completed && all_players_have_initial_participants && all_players_session_started && stats_written;
}


//#define LODWORD(x) (*((dword*)&(x)))
//
//HOOK_DECLARE(0x00000000, player_find_best_spawn_location);
//bool player_find_best_spawn_location(int player_index, real_point3d* position, euler_angles2d* orientation, float* user_index_output, bool flag1, bool flag2) {
//    bool result;
//    HOOK_INVOKE(result =, player_find_best_spawn_location, player_index, position, orientation, user_index_output, flag1, flag2);
//    return result;
//}
//
//
//HOOK_DECLARE(0x00539F70, player_find_player_character_unit_and_variant_info);
//void player_find_player_character_unit_and_variant_info(int player_index, int* tag_index, int* variant_info) {
//    HOOK_INVOKE(, player_find_player_character_unit_and_variant_info, player_index, tag_index, variant_info);
//}
//
//HOOK_DECLARE(0x004FF020, vector3d_from_angle);
//void vector3d_from_angle(vector3d* vector, float angle) {
//    HOOK_INVOKE(, vector3d_from_angle, vector, angle);
//}
//
//HOOK_DECLARE(0x005360E0, player_update_armor_loadout);
//void player_update_armor_loadout(int player_index, void* player) {
//    HOOK_INVOKE(, player_update_armor_loadout, player_index, player);
//}
//
//HOOK_DECLARE(0x00536750, player_update_weapon_loadout);
//void player_update_weapon_loadout(int player_index, void* player) {
//    HOOK_INVOKE(, player_update_weapon_loadout, player_index, player);
//}
//
//HOOK_DECLARE(0x00536000, player_get_active_armor_loadout);
//void* player_get_active_armor_loadout(void* player) {
//    void* result;
//    HOOK_INVOKE(result =,player_get_active_armor_loadout, player);
//    return result;
//}
//
////HOOK_DECLARE(0x0054F250, game_engine_get_change_colors);
////void game_engine_get_change_colors(int player_index, s_player_appearance* appearance, e_game_team team, real_rgb_color* colors, bool* flag) {
////    HOOK_INVOKE(, game_engine_get_change_colors, player_index, appearance, team, colors, flag);
////}
//
//HOOK_DECLARE(0x005A4430, ApplyArmor);
//void ApplyArmor(void* armor_loadout, int biped) {
//    HOOK_INVOKE(, ApplyArmor, armor_loadout, biped);
//}
//
//HOOK_DECLARE(0x0069CAE1, player_has_gameplay_modifier);
//bool player_has_gameplay_modifier(void* player, int modifier) {
//    bool result;
//    HOOK_INVOKE(result = ,player_has_gameplay_modifier, player, modifier);
//    return result;
//}
//
//HOOK_DECLARE(0x004B3010, simulation_action_obplayerect_update_internal);
//void simulation_action_obplayerect_update_internal(int unit_index, void* flags) {
//    HOOK_INVOKE(, simulation_action_obplayerect_update_internal, unit_index, flags);
//}
//
//HOOK_DECLARE(0x0053C8A0, player_set_facing);
//void player_set_facing(int player_index, vector3d* facing) {
//    HOOK_INVOKE(, player_set_facing, player_index, facing);
//}
//
//HOOK_DECLARE(0x0053B120, player_increment_control_context);
//void player_increment_control_context(int player_index) {
//    HOOK_INVOKE(, player_increment_control_context, player_index);
//}
//
//HOOK_DECLARE(0x005A30F0, game_engine_add_starting_equipment);
//void game_engine_add_starting_equipment(int unit_index) {
//    HOOK_INVOKE(, game_engine_add_starting_equipment, unit_index);
//}
//
////HOOK_DECLARE(0x004B26B0, simulation_action_game_engine_player_update);
////void simulation_action_game_engine_player_update(float flag, void* flags) {
////    HOOK_INVOKE(, simulation_action_game_engine_player_update, flag, flags);
////}
//
//HOOK_DECLARE(0x006D1B10, game_engine_spawn_influencer_record_player_spawn);
//void game_engine_spawn_influencer_record_player_spawn(int player_index) {
//    HOOK_INVOKE(, game_engine_spawn_influencer_record_player_spawn, player_index);
//}
//
//// Hook Declaration
//HOOK_DECLARE(0x004B486D0, sub_B486D0);
//void sub_B486D0(unsigned int unit_index, void* emblem_info, int arg2) {
//    HOOK_INVOKE(, sub_B486D0, unit_index, emblem_info, arg2);
//}
//
//HOOK_DECLARE(0x0053CE60, player_spawn);
//bool __cdecl player_spawn(long player_index, union real_point3d const* a2, float const* a3) {
//    //bool result;
//    //HOOK_INVOKE(result = ,player_spawn, player_index, a2, a3);
//
//    //c_console::write_line("donkey:matchmaking:spawn player_spawn for player %d returning %hhd", player_index, result);
//
//    //return result;
//
//        // [COLLAPSED LOCAL DECLARATIONS. PRESS KEYPAD CTRL-"+" TO EXPAND]
//
//    TLS_DATA_GET_VALUE_REFERENCE(player_data);
//    TLS_DATA_GET_VALUE_REFERENCE(obplayerect_header_data);
//
//
//    euler_angles2d orientation;
//    real_point3d creation_data;                // Holds position data for obplayerect creation
//    int unit_info;                             // Holds unit and variant information
//    obplayerect_placement_data placement_data;    // Holds the placement data of the obplayerect
//    const float* arg8;                         // Pointer to a float value
//
//
//    auto* player = &player_data[player_index];
//    int teleport_flag = 0;
//    game_is_predicted();
//
//    //if (game_is_campaign()) {
//    //    int failed_teleport_unit_index = player->failed_teleport_unit_index;
//    //    player->failed_teleport_unit_index = -1;
//    //    if (failed_teleport_unit_index != -1) {
//    //        auto yaw = 16 * (unsigned __int16)failed_teleport_unit_index;
//    //        auto* data = &obplayerect_header_data[0];
//    //        orientation.yaw = yaw;
//    //        if (((*(obplayerect_datum**)((char*)&data->datum + LODWORD(yaw)))->flags & 0x80) != 0) {
//    //            obplayerect_set_in_limbo(failed_teleport_unit_index, 0);
//    //            yaw = orientation.yaw;
//    //        }
//    //        if (((*(obplayerect_datum**)((char*)&obplayerect_header_data->data->instance + LODWORD(yaw)))->obplayerect.damage_flags & 4) == 0) {
//    //            obplayerect_activate(failed_teleport_unit_index);
//    //            obplayerect_set_hidden(failed_teleport_unit_index, 0);
//    //            player_set_unit_index(player_index, failed_teleport_unit_index);
//    //            goto LABEL_38;
//    //        }
//    //        obplayerect_delete(failed_teleport_unit_index);
//    //    }
//    //}
//
//    if (!game_is_predicted()) {
//        if (!player_find_best_spawn_location(
//            player_index,
//            &creation_data.position,
//            &orientation,
//            (float*)&unit_info.output_user_index,
//            1,
//            0)) {
//            return teleport_flag;
//        }
//
//        player_find_player_character_unit_and_variant_info(player_index, &tag_index, &creation_data.editor_folder_index);
//        if (tag_index == -1) {
//            return teleport_flag;
//        }
//
//        placement_data.multiplayer_properties.game_engine_flags = 0;
//        placement_data.multiplayer_properties.spawn_flags = 0;
//        obplayerect_placement_data_new(&placement_data, tag_index, -1, 0);
//        placement_data.model_variant_index = creation_data.editor_folder_index;
//        placement_data.position = creation_data.position;
//        vector3d_from_angle(&placement_data.forward, orientation.yaw);
//        placement_data.up = *global_up3d;
//        int team_index = player->configuration.host.team_index;
//        placement_data.player_index = player_index;
//        placement_data.team_index = team_index;
//        team_index = -1;
//
//        if (!game_is_multiplayer()) {
//            goto LABEL_59;
//        }
//
//        if (game_engine_has_teams()) {
//            team_index = player->configuration.host.team_index;
//        }
//        else {
//        LABEL_59:
//            if (game_is_campaign() && !game_is_survival()) {
//                team_index = 0;
//            }
//        }
//
//        player_update_armor_loadout(player_index, player);
//        player_update_weapon_loadout(player_index, player);
//        auto* loadout = (int*)player_get_active_armor_loadout(player);
//        game_engine_get_change_colors(player_index, loadout, team_index, change_colors, (bool*)&orientation.pitch + 3);
//        if (HIBYTE(orientation.pitch) || game_is_survival()) {
//            placement_data.active_change_colors |= 0x1Fu;
//            qmemcpy(placement_data.change_colors, change_colors, sizeof(placement_data.change_colors));
//        }
//        if (a2) {
//            placement_data.position = *a2;
//        }
//        if (arg8) {
//            vector3d_from_angle(&placement_data.forward, *arg8);
//        }
//        creation_data.insertion_point_index = placement_data.forward.k;
//        creation_data.facing = *(real_euler_angles2d*)&placement_data.forward.i;
//
//        int biped = obplayerect_new(&placement_data);
//        auto player_biped = biped;
//        auto* active_armor_loadout = player_get_active_armor_loadout(player);
//        ApplyArmor(active_armor_loadout, biped);
//        if (biped == -1) {
//            return teleport_flag;
//        }
//
//        if (unit_info.output_user_index != 0.0f) {
//            auto* desired_aiming_vector = &obplayerect_header_data[(unsigned __int16)biped].datum.desired_aiming_vector;
//            unit_info.player_index = LODWORD(orientation.yaw);
//            vector3d_from_euler_angles2d(desired_aiming_vector, (real_euler_angles2d*)&unit_info);
//            creation_data.facing = *(real_euler_angles2d*)&desired_aiming_vector->i;
//            creation_data.insertion_point_index = desired_aiming_vector->k;
//        }
//        player_set_unit_index(player_index, biped);
//
//        if (player->cooldown_reset_unknown42) {
//            auto cooldown_reset_unknown44 = player->cooldown_reset_unknown44;
//            if (cooldown_reset_unknown44 > player->cooldown_reset_unknown40) {
//                LOWORD(cooldown_reset_unknown44) = player->cooldown_reset_unknown40;
//            }
//            player->cooldown_reset_unknown40 = cooldown_reset_unknown44;
//        }
//        else if (player_has_gameplay_modifier(player, _gameplay_modifier_cooldown_reset)) {
//            sub_53C860(player, 0.0f, 0.0f);
//        }
//        else {
//            sub_53C860(player, 10.0f, 10.0f);
//        }
//
//        simulation_action_obplayerect_create(biped);
//        auto v35 = 0i64;
//        c_simulation_obplayerect_update_flags::set_flag(&v35, biped, 0xEu);
//        simulation_action_obplayerect_update_internal(biped, &v35);
//        player_set_facing(player_index, (vector3d*)&creation_data.facing);
//        player_increment_control_context(player_index);
//        game_engine_add_starting_equipment(biped);
//
//        if ((player->flags & 8) != 0) {
//            v35.flags[0] = 0x40000;
//            v35.flags[1] = 0;
//            simulation_action_game_engine_player_update(player_index, &v35);
//        }
//
//        if (game_is_multiplayer()) {
//            game_engine_spawn_influencer_record_player_spawn(player_index);
//        }
//    }
//
//LABEL_38:
//    sub_B486D0(player->unit_index, &player->configuration.host.appearance.emblem_info, 0);
//    sub_B486D0(player->unit_index, &player->configuration.host.appearance.emblem_info, 1);
//    player->tank_mode_time2D64 = 0;
//    player->tank_mode_duration = 0.0;
//    player->tank_mode_unknown2D74 = 0.0;
//    player->tank_mode_unknown2D78 = 0.0;
//    player->stamina_restore_near_death_timer = 0;
//    player->grenade_scavenger_modifier_used = 0;
//    for (int i = 0; i < 8; ++i) {
//        player->tracking_obplayerects[i].obplayerect_index = -1;
//        player->tracking_obplayerects[i].__time8 = 0;
//    }
//    player->momentum_timer = 0;
//    player->momentum_decay_timer = 0;
//    player->momemtum_suppressed = 0;
//    player->sprinting = 0;
//    player->__unknown5E = 0;
//    player->magnification_level = -1;
//    player->__unknown62 = 0;
//
//    for (c_player_output_user_iterator it(player_index); it.next();) {
//        int output_user_index = it.get_output_user_index();
//        //chud_motion_sensor_invalidate(output_user_index);
//        sub_684D90(output_user_index);
//        sub_684AC0(output_user_index);
//        observer_obsolete_position(output_user_index);
//    }
//
//    //if (game_is_multiplayer() && player_is_local(player_index)) {
//    //    auto* runtime_data = scenario_multiplayer_globals_try_and_get_runtime_data();
//    //    if (runtime_data) {
//    //        int sound_index = player_has_gameplay_modifier(player, _gameplay_modifier_respawn_modifier) ? runtime_data->modifier_respawn_sound.index : runtime_data->respawn_sound.index;
//    //        if (sound_index != -1) {
//    //            unspatialized_impulse_sound_new(sound_index, 1.0f);
//    //        }
//    //    }
//    //}
//    player->__unknown2CD0 = 0;
//
//    int recently_spawned_grace_period_seconds = game_get_recently_spawned_grace_period_seconds();
//    player->recently_spawned_timer = game_seconds_integer_to_ticks(max(recently_spawned_grace_period_seconds, 1));
//    player->recently_spawned_timer_is_initial_spawn = (player->flags & 8) != 0;
//    player->multiplayer.respawn_target_player_index = -1;
//    player->respawn_in_progress = 0;
//
//    if (game_is_authoritative()) {
//        player->flags &= ~8u;
//        v35.flags[0] = 1;
//        v35.flags[1] = 0;
//        simulation_action_game_engine_player_update(player_index, &v35);
//    }
//
//    if (game_is_campaign() && game_is_cooperative()) {
//        player->flags |= 0x800u;
//    }
//
//    player->early_respawn_requested = 0;
//    v35.flags[0] = 2;
//    v35.flags[1] = 0;
//    simulation_action_game_engine_player_update(player_index, &v35);
//    return 1;
//}
//
//// Hook Declaration
//HOOK_DECLARE(0x0053BBE0, player_positions_initialize_for_new_structure_bsp);
//void player_positions_initialize_for_new_structure_bsp() {
//    HOOK_INVOKE(, player_positions_initialize_for_new_structure_bsp);
//}
//
//// Hook Declaration
//HOOK_DECLARE(0x00543560, players_update_activation);
//void players_update_activation() {
//    HOOK_INVOKE(, players_update_activation);
//    // Your hook code here, if any additional behavior is needed
//}
//
//// Hook declaration with address and function name
//HOOK_DECLARE(0x005D4690, player_control_set_deterministic_action_test_flags);
//// Hook implementation
//void __cdecl player_control_set_deterministic_action_test_flags(int index, qword action_test_flags) {
//    HOOK_INVOKE(, player_control_set_deterministic_action_test_flags, index, action_test_flags);
//}
//
//// Hook declaration with address and function name
//HOOK_DECLARE(0x005D46C0, player_control_set_external_action);
//
//// Hook implementation
//void __cdecl player_control_set_external_action(int index, const void* action) {
//    // Your custom logic here, before invoking the original function if needed
//    HOOK_INVOKE(, player_control_set_external_action, index, action);
//}
//
//// Hook declaration with address and function name
//HOOK_DECLARE(0x005523A0, game_engine_round_condition_test);
//
//// Hook implementation
//bool __cdecl game_engine_round_condition_test(int condition) {
//    bool result;
//    // Your custom logic here, before invoking the original function if needed
//    HOOK_INVOKE(result =, game_engine_round_condition_test, condition);
//    // Additional logic after the original function call if needed
//    return result;
//}
//
//// Hook declaration with address and function name
//HOOK_DECLARE(0x0053CE60, player_spawn);
//
//// Hook implementation
//bool __cdecl player_spawn(int player_index, real_point3d* a2, const float* arg) {
//    bool result;
//    // Your custom logic here, before invoking the original function if needed
//    HOOK_INVOKE(result =, player_spawn, player_index, a2, arg);
//    // Additional logic after the original function call if needed
//    return result;
//}
//
//// Hook declaration with address and function name
//HOOK_DECLARE(0x00530A20, game_coop_allow_respawn);
//
//// Hook implementation
//bool __cdecl game_coop_allow_respawn() {
//    bool result;
//    // Your custom logic here, before invoking the original function if needed
//    HOOK_INVOKE(result =, game_coop_allow_respawn);
//    // Additional logic after the original function call if needed
//    return result;
//}

// Hook declaration with address and function name
//HOOK_DECLARE(0x005B80D0, players_coop_desire_respawn, player_index);

// Hook implementation
//void __cdecl players_coop_desire_respawn(int player_index) {
//    // Your custom logic here, before invoking the original function if needed
//    HOOK_INVOKE(, players_coop_desire_respawn, player_index);
//    // Additional logic after the original function call if needed
//}

//HOOK_DECLARE(0x005CBF90, game_engine_assemble_player_traits);
//void __cdecl game_engine_assemble_player_traits(int a2)
//{
//    HOOK_INVOKE(, game_engine_assemble_player_traits, a2);
//}
//
//HOOK_DECLARE(0x00553320, game_engine_update_player_sitting_out);
//void __cdecl game_engine_update_player_sitting_out()
//{
//    HOOK_INVOKE(, game_engine_update_player_sitting_out);
//}
//
//#define SLOWORD(w) ((word)(w) & 0xFFFF)
//#define HIWORD(l) ((word)(((dword)(l) >> 16) & 0xFFFF))

//
//// STR: "cinematic skipped", "controller", "cinematics"
//// local variable allocation has failed, the output may be wrong!
//HOOK_DECLARE(0x00543D20, players_update_before_game);
//void __cdecl players_update_before_game(const struct simulation_update *update)
//{
//    HOOK_INVOKE(, players_update_before_game, update);
//  TLS_DATA_GET_VALUE_REFERENCE(players_globals);
//  TLS_DATA_GET_VALUE_REFERENCE(player_data);
//  TLS_DATA_GET_VALUE_REFERENCE(object_header_data);
//
//
//
//  //bool primary_skull_iron_is_active = players_globals->__unknown13C == 0;
//  //if ( !primary_skull_iron_is_active )
//    player_positions_initialize_for_new_structure_bsp();
//
//  players_update_activation();
//
//  s_data_iterator player_iterator;
//  player_datum* player;
//  dword v23[16];
//
//  data_iterator_begin(&player_iterator, player_data.m_data);
//  for (player = (player_datum*)data_iterator_next(&player_iterator); player; player = (player_datum*)data_iterator_next(&player_iterator)) {
//      if (player->flags == 0x8) {
//          if (current_game_engine())
//          {
//              current_game_engine()->player_about_to_spawn(player_iterator.index);
//          }
//          game_engine_assemble_player_traits(player_iterator.index);
//          if (player_spawn(player_iterator.index, 0, 0) && current_game_engine())// called
//          {
//              current_game_engine()->player_just_spawned(player_iterator.index);
//          }
//      }
//  }
//
//  return;
//
//  data_iterator_begin(&player_iterator, player_data.m_data);
//  for (player = (player_datum*)data_iterator_next(&player_iterator); player; player = (player_datum*)data_iterator_next(&player_iterator) )
//    player_control_set_deterministic_action_test_flags(
//      SLOWORD(player_iterator.index),
//      update->player_actions[SLOWORD(player_iterator.index)].action_test_flags);
//
//  if ( game_is_playback() )
//  {
//    for (int i = 0; i < 4; ++i )
//    {
//      int player_by_output_user = player_mapping_get_player_by_output_user((e_output_user_index)i);
//      if ( player_by_output_user != -1 && ((1 << player_by_output_user) & update->player_flags) != 0 )
//        player_control_set_external_action(i, &update->player_actions[(__int16)player_by_output_user]);
//    }
//  }
//  int multiplayer_player_indices_count = 0;
//  data_iterator_begin(&player_iterator, player_data.m_data);
//  player = (player_datum *)data_iterator_next(&player_iterator);
//  if ( player )
//  {
//    while ( 1 )
//    {
//      auto flags = player->flags;
//      if ( (flags & 2) != 0 || (flags & 1) == 0 || player->unit_index != -1 || game_in_editor() )
//        goto LABEL_38;                          // playerumps
//
//      if ( game_is_multiplayer() )              // no, multiplayer != matchmaking
//      {
//        if ( game_is_authoritative()
//          && game_engine_should_spawn_player(player_iterator.index)
//          && !game_engine_round_condition_test(4) )
//        {
//            v23[multiplayer_player_indices_count++] = player_iterator.index;
//        }
//        goto LABEL_38;
//      }
//      if ( game_is_survival() )                 // no
//      {
//        if ( game_is_predicted() )
//          goto LABEL_38;
//
//        if ( (player->flags & 0x2000) != 0 )
//          goto LABEL_38;
//
//        if ( (player->flags & 8) != 0 )
//        {
//          if ( !game_engine_should_spawn_player(player_iterator.index) || game_engine_round_condition_test(4) )
//            goto LABEL_38;
//LABEL_27:
//          player_spawn(player_iterator.index, 0, 0);
//          goto LABEL_38;
//        }
//
//        if ( game_survival_allow_respawn() )
//          goto LABEL_37;
//
//        //primary_skull_iron_is_active = (player->flags & 0x8000) == 0;
//      }
//      else
//      {
//        if ( !game_is_campaign() )              // not campaign...
//          goto LABEL_38;
//
//        if ( game_is_predicted() )
//          goto LABEL_38;
//
//        if ( (player->flags & 0x2000) != 0 )
//          goto LABEL_38;
//
//        if ( (player->flags & 8) != 0 )
//          goto LABEL_27;
//
//        if ( !game_is_cooperative() )
//          goto LABEL_38;
//
//        //primary_skull_iron_is_active = !game_coop_allow_respawn();
//      }
//      //if ( !primary_skull_iron_is_active )
//LABEL_37:
//        //players_coop_desire_respawn(player_iterator.index);
//LABEL_38:
//      player = (player_datum *)data_iterator_next(&player_iterator);
//      if ( !player )
//      {
//        if ( multiplayer_player_indices_count > 0 )
//        {
//          
//          do
//          {
//            auto v11 = 0;
//            if ( multiplayer_player_indices_count > 1 )
//            {
//              dword* random_seed_address = get_random_seed_address();
//              dword spawn_order_permutation = 0x19660D * *random_seed_address + 1013904223;
//              *random_seed_address = spawn_order_permutation;
//              v11 = (int)HIWORD(spawn_order_permutation) % multiplayer_player_indices_count;
//            }
//            dword v14 = v23[v11];
//            if ( game_engine_should_spawn_player(v14) )
//            {
//              if ( current_game_engine() )
//              {
//                current_game_engine()->player_about_to_spawn(v14);
//              }
//              game_engine_assemble_player_traits(v14);
//              if ( player_spawn(v14, 0, 0) && current_game_engine() )// called
//              {
//                current_game_engine()->player_just_spawned(v14);
//              }
//            }
//            if ( v11 != --multiplayer_player_indices_count )
//              v23[v11] = v23[multiplayer_player_indices_count];
//            --multiplayer_player_indices_count;
//          }
//          while ( multiplayer_player_indices_count > 0 );
//        }
//        break;
//      }
//    }
//  }
//  game_engine_update_player_sitting_out();
//  players_globals->respawn_failure = 0;
//  data_iterator_begin(&player_iterator, player_data.m_data);
  //for ( player = (player_datum *)data_iterator_next(&player_iterator);
  //      player;
  //      player = (player_datum *)data_iterator_next(&player_iterator) )
  //{
  //  players_coop_update_respawn(player_iterator.index);
  //}
  //data_iterator_begin(&player_iterator, player_data.m_data);
  //auto v17 = (player_datum *)data_iterator_next(&player_iterator);
  //for ( player = v17; v17; player = v17 )
  //{
  //  dword unit_index = v17->unit_index;
  //  if ( unit_index != -1 || (unit_index = v17->dead_unit_index, unit_index != -1) )
  //  {
  //    object_datum* instance = (object_datum *)object_header_data[(unsigned __int16)object_get_ultimate_parent(unit_index)].datum;
  //    if ( instance->location.cluster_reference.bsp_index != -1 )
  //      v17->cluster_reference. = (s_cluster_reference)instance->location;
  //  }
  //  v17 = (player_datum *)data_iterator_next(&player_iterator);
  //}
  //data_iterator_begin(&player_iterator, player_data.m_data);
  //player = (player_datum *)data_iterator_next(&player_iterator);
  //for ( player = player; player; player = player )
  //{
  //  long index = player_iterator.index;
  //  long v36 = player_iterator.index;
  //  if ( ((1 << SLOBYTE(player_iterator.index)) & update->player_flags) != 0 )
  //  {
  //    s_player_action v22 = &update->player_actions[player_iterator.index];
  //    if ( player_action_valid(v22) && v22 && player->unit_index != -1 && unit_controllable(player->unit_index) )
  //    {
  //      unit = (unit_datum *)tls->obplayerect_header_data->data[LOWORD(player->unit_index)].instance;
  //      v41 = unit;
  //      if ( (v22->action_flags & 0x1000) != 0 )
  //      {
  //        if ( cinematic_reverts_when_skipped() || bink_playback_should_be_playing_deterministic_bink() )
  //        {
  //          cinematic_start_user_skip_fade_out();
  //          c_datamine::c_datamine((c_datamine *)&a1, 0, "cinematic skipped", 1, "cinematics");
  //          v42 = 0;
  //          data_mine_usability_add_basic_information((struct c_datamine *)&a1);
  //          controller = player_mapping_get_input_controller(player_iterator.index);
  //          c_datamine::add_long((c_datamine *)&a1, "controller", controller);
  //          v42 = -1;
  //          c_datamine::~c_datamine((c_datamine *)&a1);
  //        }
  //        unit = v41;
  //      }

  //      if ( tls->players_globals->input_disabled )
  //      {
  //        if ( unit->actor_index == -1 )
  //          player_suppress_control(index, player->unit_index);
  //      }
  //      else
  //      {
  //        qmemcpy(&action, v22, sizeof(action));
  //        player_index = v36;
  //        map_editor_process_player_control(v36, &action);
  //        players_globals = tls->players_globals;
  //        if ( players_globals->mostly_inhibit )
  //        {
  //          // player_mostly_input_inhibit
  //          control_context = action.control_context;
  //          control_context_identifier = action.control_context_identifier;
  //          yaw = action.angles.yaw;
  //          pitch = action.angles.pitch;
  //          player_action_clear(&action);
  //          action.control_context = control_context;
  //          player_index = v36;
  //          action.control_context_identifier = control_context_identifier;
  //          action.angles.yaw = yaw;
  //          action.angles.pitch = pitch;
  //        }
  //        else
  //        {
  //          control_flags = action.control_flags;
  //          if ( players_globals->__unknown7 )
  //          {
  //            control_flags = action.control_flags & ~0x100u;
  //            action.control_flags &= ~0x100u;
  //          }

  //          if ( players_globals->disable_equipment_use )
  //            action.control_flags = control_flags & ~0x690u;
  //        }

  //        player_prepare_action(player_index, &action);
  //        player_training_examine_action(player_index, &action);

  //        if ( !game_engine_is_player_in_edit_mode(player_index, 0, 0) )
  //          player_submit_actions(player_index, player->unit_index, &action);

  //        if ( action.control_context == 1 && action.control_context_identifier == v41->control_context_identifier )
  //          player_submit_control(player_index, player->unit_index, &action);
  //      }
  //    }
  //  }
  //  player = (player_datum *)data_iterator_next(&player_iterator);
  //}
//}

//
//void __cdecl simulation_action_object_create(long object_index)
//{
//	INVOKE(0x004B2CD0, simulation_action_object_create, object_index);
//}

void __cdecl network_bandwidth_set_online_network_environment(char a2)
{
    //c_network_observer::set_online_network_environment(network_bandwidth_globals.observer, a2);
    //network_bandwidth_globals.online_network_environment = a2;
    //network_bandwidth_update_estimate();
}
HOOK_DECLARE(0x00455890, network_bandwidth_set_online_network_environment);