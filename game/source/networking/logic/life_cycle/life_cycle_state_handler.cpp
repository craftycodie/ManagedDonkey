#include "life_cycle_state_handler.hpp"
#include "networking/session/network_session.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0048E130, c_life_cycle_state_handler, setup_initial_participants);
//HOOK_DECLARE_CLASS_MEMBER(0x0048DAF0, c_life_cycle_state_handler, pick_teams_for_group);


bool __thiscall c_life_cycle_state_handler::setup_initial_participants(c_network_session* session)
{
    c_console::write_line("donkey:matchmaking c_life_cycle_state_handler::setup_initial_participants");
    if (!session->is_host())
        return 1;
    //return DECLFUNC(0x0048E130, bool, __thiscall, c_life_cycle_state_handler*, c_network_session*)(this, session);


    //this->pick_teams_for_group(true);

    bool returnVal;
    HOOK_INVOKE_CLASS_MEMBER(returnVal=, c_life_cycle_state_handler, setup_initial_participants, session);
    return returnVal;

}

//bool __thiscall c_life_cycle_state_handler::pick_teams_for_group(bool randomize_teams_for_team_selection)
//{
//    c_console::write_line("donkey:matchmaking c_life_cycle_state_handler::pick_teams_for_group");
//
//    bool returnVal;
//    HOOK_INVOKE_CLASS_MEMBER(returnVal=, c_life_cycle_state_handler, pick_teams_for_group, randomize_teams_for_team_selection);
//    return returnVal;
//
//}
