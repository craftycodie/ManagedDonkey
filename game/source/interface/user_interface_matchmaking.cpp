#include "interface/user_interface_matchmaking.hpp"

#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"

REFERENCE_DECLARE(0x0191D298, long, g_network_matchmaking_fake_progress_stage);
REFERENCE_DECLARE(0x052604C8, s_life_cycle_matchmaking_progress, g_network_matchmaking_fake_progress);

HOOK_DECLARE(0x00A98920, user_interface_matchmaking_get_matchmaking_progress);

void __cdecl user_interface_matchmaking_get_matchmaking_progress(s_life_cycle_matchmaking_progress* progress_out)
{
	network_life_cycle_get_matchmaking_progress(progress_out);

	if (g_network_matchmaking_fake_progress_stage == NONE)
		csmemcpy(&g_network_matchmaking_fake_progress, progress_out, sizeof(g_network_matchmaking_fake_progress));
	else
		csmemcpy(progress_out, &g_network_matchmaking_fake_progress, sizeof(s_life_cycle_matchmaking_progress));
}

