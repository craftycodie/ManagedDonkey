#include "networking/logic/life_cycle/life_cycle_handler_matchmaking_start.hpp"

#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00493980, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role);

bool net_matchmaking_force_gather = true;
bool net_matchmaking_force_search = false;

long __thiscall c_life_cycle_state_handler_matchmaking_start::determine_matchmaking_find_match_role(bool a1)
{
	long result = 1;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_life_cycle_state_handler_matchmaking_start, determine_matchmaking_find_match_role, a1);

	if (net_matchmaking_force_gather)
	{
		console_printf("networking:logic:life-cycle:matchmaking_start: FORCE GATHER");
		result = 2;
	}
	else if (net_matchmaking_force_search)
	{
		console_printf("networking:logic:life-cycle:matchmaking_start: FORCE SEARCH");
		result = 1;
	}

	return result;
}

