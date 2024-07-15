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
//#include <minwindef.h>
#include "objects/objects.hpp"




// Below are patches which alter the arbitration lifecycle to call c_life_cycle_state_handler::setup_initial_participants on it's update and exit sucessfully.
// TODO: Fully rewrite the update function.
//const byte call_setup_participants_bytes[] = { 0x99, 0x90 };
////DATA_PATCH_DECLARE(0x00495093, call_setup_participants, call_setup_participants_bytes);
//const byte jump_bytes[] = { 0xEB };
////DATA_PATCH_DECLARE(0x0049509C, jump1, jump_bytes);
////DATA_PATCH_DECLARE(0x004951C4, jump2, jump_bytes);
//const byte nop_bytes[] = { 0x90, 0x90 };
//DATA_PATCH_DECLARE(0x004952C4, not1, nop_bytes); // might be a bad patch

//// .text:004952CF                 jz      short loc_4952DA
//DATA_PATCH_DECLARE(0x004952CF, not2, nop_bytes);

// .text:0049507B                 jz      loc_4952DA      ; A
//const byte nop_bytes_2[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, };
//DATA_PATCH_DECLARE(0x0049507B, not1, nop_bytes_2);

// .text:004952AD                 jz      short loc_4952B6
//DATA_PATCH_DECLARE(0x004952AD, jump1, jump_bytes);

//const byte one_byte[] = { 0x01 };
//DATA_PATCH_DECLARE(0x004950CA, scuffed_teams_data, one_byte);

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


HOOK_DECLARE(0x0053CE60, player_spawn);
bool __cdecl player_spawn(long player_index, union real_point3d const* a2, float const* a3) {
    bool result;
    HOOK_INVOKE(result = ,player_spawn, player_index, a2, a3);

    c_console::write_line("donkey:matchmaking:spawn player_spawn for player %d returning %hhd", player_index, result);

    return result;
}
