#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"

#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "life_cycle_manager.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/session/network_session.hpp"
#include <simulation/simulation_watcher.hpp>


// Below are patches which alter the arbitration lifecycle to call c_life_cycle_state_handler::setup_initial_participants on it's update and exit sucessfully.
// TODO: Fully rewrite the update function.
const byte call_setup_participants_bytes[] = { 0x99, 0x90 };
DATA_PATCH_DECLARE(0x00495093, call_setup_participants, call_setup_participants_bytes);
const byte jump_bytes[] = { 0xEB };
//DATA_PATCH_DECLARE(0x0049509C, jump1, jump_bytes);
//DATA_PATCH_DECLARE(0x004951C4, jump2, jump_bytes);
const byte nop_bytes[] = { 0x90, 0x90 };
//DATA_PATCH_DECLARE(0x004952C4, not1, nop_bytes); // might be a bad patch

// .text:004952CF                 jz      short loc_4952DA
DATA_PATCH_DECLARE(0x004952CF, not2, nop_bytes);

// .text:0049507B                 jz      loc_4952DA      ; A
const byte nop_bytes_2[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, };
DATA_PATCH_DECLARE(0x0049507B, not1, nop_bytes_2);

// .text:004952AD                 jz      short loc_4952B6
DATA_PATCH_DECLARE(0x004952AD, jump1, jump_bytes);

const byte one_byte[] = { 0x01 };
DATA_PATCH_DECLARE(0x004950CA, scuffed_teams_data, one_byte);

char _cdecl c_network_session_membership__all_peers_have_connectivity_information(
    c_network_session_membership* _this)
{
    int v1; // edi
    int v2; // esi
    dword v3; // edx
    int v4; // eax
    int v5; // esi

    v1 = -1;
    v2 = 0;

    /////
    for (auto peer : _this->m_shared_network_membership.peers) {
        peer.properties.connectivity.peer_connectivity_mask = 1;
        peer.properties.estimated_upstream_bandwidth_bps = 1;
    }

    return 1;
    /////

    while (((1 << (v2 & 0x1F)) & _this->m_shared_network_membership.peer_valid_mask.get_bits_direct()[v2 >> 5]) == 0)
    {
        if (++v2 >= 17)
            goto LABEL_6;
    }
    v1 = v2;
LABEL_6:
    if (v1 == -1)
        return 1;
    v3 = _this->m_shared_network_membership.peer_valid_mask.get_bits_direct()[0];
    while ((~_this->m_shared_network_membership.peers[v1].properties.connectivity.peer_probe_mask & v3) == 0
        && _this->m_shared_network_membership.peers[v1].properties.estimated_upstream_bandwidth_bps)
    {
        v4 = v1 + 1;
        v5 = -1;
        if (v1 + 1 < 17)
        {
            while (((1 << (v4 & 0x1F)) & _this->m_shared_network_membership.peer_valid_mask.get_bits_direct()[v4 >> 5]) == 0)
            {
                if (++v4 >= 17)
                    goto LABEL_15;
            }
            v5 = v4;
        LABEL_15:
            v3 = _this->m_shared_network_membership.peer_valid_mask.get_bits_direct()[0];
        }
        v1 = v5;
        if (v5 == -1)
            return 1;
    }
    return 0;
}
HOOK_DECLARE(0x0044D130, c_network_session_membership__all_peers_have_connectivity_information);


//HOOK_DECLARE_CLASS_MEMBER(0x00494EF0, c_life_cycle_state_handler_matchmaking_arbitration, update_);
//void __thiscall c_life_cycle_state_handler_matchmaking_arbitration::update_()
//{
//    c_console::write_line("donkey:matchmaking c_life_cycle_state_handler_matchmaking_arbitration:update");
//    c_console::write_line("donkey:matchmaking m_local_state = %d flags = %d", this->m_state, this->m_handler_flags);
//
//	HOOK_INVOKE_CLASS_MEMBER(, c_life_cycle_state_handler_matchmaking_arbitration, update_);
//}


//void __thiscall c_life_cycle_state_handler_matchmaking_arbitration::update_()
//{
//     [COLLAPSED LOCAL DECLARATIONS. PRESS KEYPAD CTRL-"+" TO EXPAND]
//
//    auto m_group_session = this->m_manager->m_group_session;
//    auto session_membership = m_group_session->get_session_membership();
//    auto m_local_state = m_group_session->m_local_state;
//    auto v5 = session_membership;
//    if (m_local_state == _network_session_state_host_established || m_local_state == _network_session_state_host_disband)
//    {
//        auto session_parameters = this->m_manager->m_group_session->get_session_parameters();
//        memset(&a2.storage, 0, sizeof(a2.storage));
//        a2.progress_type = _life_cycle_matchmaking_progress_type_unknown3;
//        c_generic_network_session_parameter<s_replicated_life_cycle_matchmaking_progress>::set(
//            &session_parameters->parameters_internal.matchmaking_progress,
//            &a2);
//    }
//    if (m_group_session->is_host()
//        && (this->m_handler_flags & 1) == 0)
//    {
//        network_debug_print(
//            "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: looks like origi"
//            "nal host bailed in the middle of arbitration, disbanding");
//        c_life_cycle_state_handler::disband_group_session(this);
//    }
//    else if (m_group_session->is_host()
//        && (this->m_flags & 0x40000) == 0
//        && ((p_session_mode = (c_network_session_parameter_session_mode*)this->m_time, !network_time_globals.locked)
//            ? (v8 = system_milliseconds())
//            : (v8 = network_time_globals.time),
//            (int)(v8 - (_DWORD)p_session_mode) >= (signed __int32)(1000 * g_network_configuration.__unknown2A4)))
//    {
//        network_debug_print(
//            "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: exceeded the arb"
//            "itration establishment and conectivity giveup timer, disbanding");
//        c_life_cycle_state_handler::disband_group_session(this);
//    }
//    else
//    {
//        if (m_group_session->is_host()
//            && (this->m_flags & 0x40000) != 0
//            && ((p_session_mode = *(c_network_session_parameter_session_mode**)this->__data30, !network_time_globals.locked)
//                ? (v10 = system_milliseconds())
//                : (v10 = network_time_globals.time),
//                (int)(v10 - (_DWORD)p_session_mode) >= (signed __int32)(1000 * g_network_configuration.__unknown2A8)))
//        {
//            network_debug_print(
//                "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: exceeded the a"
//                "rbitration completion giveup timer, disbanding");
//            c_life_cycle_state_handler::disband_group_session(this);
//        }
//        else
//        {
//            if (!m_group_session->is_host()
//                || c_network_session_membership::all_peers_established(v5))
//            {
//                if (m_group_session->is_host()
//                    || c_network_session_membership::all_peers_have_connectivity_information(v5))
//                {
//                    if (m_group_session->is_host() && !c_life_cycle_state_handler::session_composition_valid(m_group_session))
//                    {
//                        network_debug_print(
//                            "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: group se"
//                            "ssion composition has become invalid, disbanding");
//                        c_life_cycle_state_handler::disband_group_session(this);
//                    }
//                    else
//                    {
//                        if (m_group_session->is_host() && !c_life_cycle_state_handler::pick_teams_for_group(this, 0))
//                        {
//                            network_debug_print(
//                                "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: teams "
//                                "have become invalid during arbitration, disbanding");
//                            c_life_cycle_state_handler::disband_group_session(this);
//                        }
//                        else
//                        {
//                            auto v15 = m_group_session->get_session_parameters();
//                            auto v16 = this->m_handler_flags >> 3;
//                            p_session_mode = &v15->parameters_internal.session_mode;
//                            if ((v16 & 1) == 0)
//                            {
//                                if (!m_group_session->is_host())
//                                    c_life_cycle_state_handler_matchmaking_arbitration::update_arbitration(this);
//                            }
//                            if ((this->m_handler_flags & 0x10) == 0)
//                            {
//                                if (m_group_session->is_host())
//                                {
//                                    if ((this->m_handler_flags & 0x40000) == 0)
//                                    {
//                                        network_debug_print(
//                                            "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: "
//                                            "started waiting for clients to arbitrate");
//                                        if (network_time_globals.locked)
//                                            time = network_time_globals.time;
//                                        else
//                                            time = system_milliseconds();
//                                        *(_DWORD*)this->__data30 = time;
//                                        this->m_flags |= 0x40000u;
//                                    }
//                                    if (c_network_session_membership::peer_property_flag_test_any_peer(v5, 3))
//                                    {
//                                        network_debug_print(
//                                            "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: "
//                                            "a peer failed arbitration, disbanding");
//                                        c_life_cycle_state_handler::disband_group_session(this);
//                                    }
//                                    else if (c_network_session_membership::peer_property_flag_test(v5, 2, 2))
//                                    {
//                                        c_life_cycle_state_handler_matchmaking_arbitration::update_arbitration(this);// called on joining client
//                                    }
//                                }
//                            }
//                            auto m_flags = this->m_handler_flags;
//                            if ((m_flags & 0x10) != 0)
//                            {
//                                if (m_group_session->is_host())
//                                {
//                                    if ((m_flags & 0x100) != 0)
//                                    {
//                                        network_debug_print(
//                                            "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: "
//                                            "host failed arbitration, disbanding");
//                                        c_life_cycle_state_handler::disband_group_session(this);
//                                    }
//                                    else if ((m_flags & 0x20) == 0)
//                                    {
//                                        if (c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results(this))
//                                        {
//                                            if (c_life_cycle_state_handler::pick_teams_for_group(this, 1))// randomize_teams_for_team_selection
//                                            {
//                                                network_session_interface_set_peer_status_flag(4, 1);
//                                                this->m_handler_flags |= 0x20u;
//                                            }
//                                            else
//                                            {
//                                                network_debug_print(
//                                                    "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::upda"
//                                                    "te: failed to pick teams (for reals!), disbanding");
//                                                c_life_cycle_state_handler::disband_group_session(this);
//                                            }
//                                        }
//                                        else
//                                        {
//                                            network_debug_print(
//                                                "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update"
//                                                ": failed to resolve arbitration results with session membership, disbanding");
//                                            c_life_cycle_state_handler::disband_group_session(this);
//                                        }
//                                    }
//                                }
//                            }
//                            if ((this->m_handler_flags & 0x40) == 0 && c_network_session_membership::peer_property_flag_test(v5, 1, 4))
//                            {
//                                if (c_life_cycle_state_handler_matchmaking_arbitration::ready_to_update_repeated_play(this))
//                                {
//                                    c_life_cycle_state_handler_matchmaking_arbitration::update_repeated_play(this);
//                                }
//                                else if ((this->m_handler_flags & 0x20000) == 0)
//                                {
//                                    network_debug_print(
//                                        "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::update: wa"
//                                        "iting for being able to update repeated play");
//                                    this->m_handler_flags |= 0x20000u;
//                                }
//                            }
//                            auto v22 = p_session_mode;
//                            if (c_network_session_parameter_base::get_allowed((c_network_session_parameter_session_mode*)((char*)p_session_mode + 1400)))
//                                c_life_cycle_state_handler_matchmaking_arbitration::update_start_match((c_life_cycle_state_handler_end_game_write_stats*)this);
//                            if ((this->m_handler_flags & 0x400) != 0)
//                                c_life_cycle_state_handler_matchmaking_arbitration::update_initial_stats_write(this);
//                            auto v23 = m_group_session->m_local_state;
//                            if ((v23 == _network_session_state_host_established || v23 == _network_session_state_host_disband)
//                                && c_life_cycle_state_handler_matchmaking_arbitration::ready_to_start(this))
//                            {
//                                c_network_session_parameter_session_mode::set(v22, _network_session_mode_setup);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//}


char __cdecl network_arbitration_initiate(int managed_session_index, qword nonce)
{
    int session; // esi

    session = managed_session_index;
    network_arbitration_globals.arbitration_status = _network_arbitration_status_registered;
    network_arbitration_globals.managed_session_index = managed_session_index;
    return 1;

    // ms23
    //session = managed_session_index;
    //network_arbitration_globals.arbitration_status = _network_arbitration_status_registration_failed;
    //network_arbitration_globals.managed_session_index = managed_session_index;
    //if (managed_session_get_handle(managed_session_index, (void**)&managed_session_index))
    //    network_debug_print(
    //        "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_arbitration_initiate: failed to initiate registration for session 0x%08X",
    //        session);
    //else
    //    network_debug_print(
    //        "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_arbitration_initiate: failed to get session handle for session 0x%08X",
    //        session);
    //return 0;
}
HOOK_DECLARE(0x004D9B60, network_arbitration_initiate);

//
//dword _cdecl c_simulation_watcher__get_machine_valid_mask(c_simulation_watcher* _this)
//{
//    qword local_machine_id = transport_secure_address_get_local_machine_id();
//    c_console::write_line("donkey:matchmaking local machine id %I64u", local_machine_id);
//    c_console::write_line("donkey:matchmaking local machine index %I64u", _this->m_local_machine_index);
//    c_console::write_line("donkey:matchmaking local machine id 1 %I64u", _this->m_local_machine_identifiers[0]);
//    c_console::write_line("donkey:matchmaking local machine id 2 %I64u", _this->m_local_machine_identifiers[1]);
//    c_console::write_line("donkey:matchmaking local machine id 3 %I64u", _this->m_local_machine_identifiers[2]);
//    c_console::write_line("donkey:matchmaking c_simulation_watcher::get_machine_valid_mask for %I64u with mask %d", _this->m_local_machine_identifiers[_this->m_local_machine_index], _this->m_local_machine_valid_mask);
//    return _this->m_local_machine_valid_mask;
//    //return 1;
//}
//HOOK_DECLARE(0x0046C730, c_simulation_watcher__get_machine_valid_mask);
//
