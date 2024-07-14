#include "networking/logic/logic_matchmaking_seeker.hpp"
#include "memory/module.hpp"
#include "networking/logic/logic_session_tracker.hpp"
#include "networking/transport/transport_security.hpp"
#include <networking/network_time.hpp>

HOOK_DECLARE_CLASS_MEMBER(0x004D4560, c_matchmaking_seeker, search_in_progress);
HOOK_DECLARE_CLASS_MEMBER(0x004D41F0, c_matchmaking_seeker, get_session_to_join);

bool __thiscall c_matchmaking_seeker::search_in_progress()
{

	//this->m_total_sessions_contacted_count1 = 1;
	//this->m_total_sessions_contacted_count2 = 1;
	////this->m_total_session_search_count = 3;
	this->m_session_search_stage = 2;

	this->m_session_search_data[this->m_current_session_search_index].search_start_time = network_time_get();

	// flag 5 - online session search running task completed, we can initiate our search

	SET_BIT(this->m_flags, 2, false);
	SET_BIT(this->m_flags, 4, true);
	SET_BIT(this->m_flags, 6, false);

	bool result;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_matchmaking_seeker, search_in_progress);

	//_return = true;

	return result;
}

bool __thiscall c_matchmaking_seeker::get_session_to_join(s_suitable_matchmaking_session* session_out)
{ 


	bool result;

	//SET_BIT(this->m_flags, 4, true);

	HOOK_INVOKE_CLASS_MEMBER(result =, c_matchmaking_seeker, get_session_to_join, session_out);

	return result;
}

