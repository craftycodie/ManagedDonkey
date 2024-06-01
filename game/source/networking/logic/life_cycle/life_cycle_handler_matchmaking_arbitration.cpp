#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_arbitration.hpp"

#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "life_cycle_manager.hpp"
#include "networking/logic/network_life_cycle.hpp"

// Below are patches which alter the arbitration lifecycle to call c_life_cycle_state_handler::setup_initial_participants on it's update and exit sucessfully.
// TODO: Fully rewrite the update function.
const byte call_setup_participants_bytes[] = { 0x99, 0x90 };
DATA_PATCH_DECLARE(0x00495093, call_setup_participants, call_setup_participants_bytes);
const byte jump_bytes[] = { 0xEB };
DATA_PATCH_DECLARE(0x0049509C, jump1, jump_bytes);
DATA_PATCH_DECLARE(0x004951C4, jump2, jump_bytes);
const byte nop_bytes[] = { 0x90, 0x90 };
DATA_PATCH_DECLARE(0x004952C4, not1, nop_bytes);
DATA_PATCH_DECLARE(0x004952CF, not2, nop_bytes);