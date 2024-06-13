#include "life_cycle_state_handler.hpp"
#include "networking/session/network_session.hpp"

bool __cdecl c_life_cycle_state_handler::setup_initial_participants(c_network_session* session)
{
    return DECLFUNC(0x0048E130, bool, __thiscall, c_life_cycle_state_handler*, c_network_session*)(this, session);
}
