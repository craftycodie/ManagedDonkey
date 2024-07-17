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
    c_network_session* session = this->m_manager->get_group_session();

    if (session->is_host() && !(this->m_flags & 0x80)) {
        // Arbitration isn't implemented so we mark it as complete to make the game progress.
        this->mark_arbitration_complete();
        // setup_initial_participants should be called by this function but isn't, so we call it manually.
        this->setup_initial_participants(session);
        this->m_flags |= 0x80;
    }

    HOOK_INVOKE_CLASS_MEMBER(, c_life_cycle_state_handler_matchmaking_arbitration, update);
}



char __cdecl network_arbitration_initiate(int managed_session_index, qword nonce)
{
    int session; // esi

    session = managed_session_index;
    network_arbitration_globals.arbitration_status = _network_arbitration_status_registered;
    network_arbitration_globals.managed_session_index = managed_session_index;
    return 1;
}
HOOK_DECLARE(0x004D9B60, network_arbitration_initiate);

bool __cdecl c_life_cycle_state_handler_matchmaking_arbitration__session_membership_matches_arbitration_registration_results(void*) {
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
