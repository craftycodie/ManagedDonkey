#include "networking/logic/logic_session_tracker.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_memory.hpp"

#define SESSION_STORAGE_COUNT 16 // engine default is 100
#define SESSION_STORAGE_SIZE sizeof(s_network_session_tracker_session) * SESSION_STORAGE_COUNT

REFERENCE_DECLARE(0x0229EC20, s_session_tracker_globals, session_tracker_globals);

HOOK_DECLARE(0x004E2DB0, network_session_tracker_clear);
HOOK_DECLARE(0x004E2DC0, network_session_tracker_clear_unsuitable_sessions);
HOOK_DECLARE(0x004E2DF0, network_session_tracker_dispose);
HOOK_DECLARE(0x004E2E40, network_session_tracker_get_session_count);
HOOK_DECLARE(0x004E2E50, network_session_tracker_get_session_data);
HOOK_DECLARE(0x004E2E70, network_session_tracker_get_session_status);
HOOK_DECLARE(0x004E2E90, network_session_tracker_initialize);
HOOK_DECLARE(0x004E2EA0, network_session_tracker_mark_session_undesirable);
HOOK_DECLARE(0x004E2ED0, network_session_tracker_mark_session_unsuitable);
HOOK_DECLARE(0x004E2F00, network_session_tracker_session_contacted);
HOOK_DECLARE(0x004E2F30, network_session_tracker_start);
HOOK_DECLARE(0x004E2F50, network_session_tracker_stop);
HOOK_DECLARE(0x004E2F90, network_session_tracker_track_session);
HOOK_DECLARE(0x004E2FB0, network_session_tracker_update);

//// c_session_tracker::add_session
//t_value_type<byte> const session_storage_count = { .value = SESSION_STORAGE_COUNT };
//DATA_PATCH_DECLARE(0x004E253A + 2, session_storage_count, session_storage_count.bytes); // cmp     esi, 100
//DATA_PATCH_DECLARE(0x004E253F + 1, session_storage_count, session_storage_count.bytes); // push    100
//
//// c_session_tracker::allocate_storage
//t_value_type<dword> const session_storage_size = { .value = SESSION_STORAGE_SIZE };
//DATA_PATCH_DECLARE(0x004E2676 + 1, session_storage_size, session_storage_size.bytes); // push    8B9340h
//DATA_PATCH_DECLARE(0x004E267E + 3, session_storage_size, session_storage_size.bytes); // mov     dword ptr [esi+14h], 8B9340h

//bool __cdecl c_session_tracker::add_session(char const* name, s_transport_session_description const* description)
bool __cdecl c_session_tracker::add_session(char const* name, s_transport_session_description const* description)
{
	//return DECLFUNC(0x004E2530, bool, __thiscall, c_session_tracker*, char const*, s_transport_session_description const*)(this, name, description);

	ASSERT(m_flags.test(_session_tracker_initialized_bit));

	if (m_session_count < SESSION_STORAGE_COUNT)
	{
		s_network_session_tracker_session* tracked_session = &m_sessions[m_session_count];
		ASSERT(!tracked_session->flags.test(_session_tracker_session_valid_bit));

		csmemset(tracked_session, 0, sizeof(s_network_session_tracker_session));

		csstrnzcpy(tracked_session->name, name, sizeof(tracked_session->name));
		tracked_session->description = *description;
		tracked_session->session_index = m_session_count;
		tracked_session->qos_attempt_index = NONE;
		tracked_session->qos_attempt_target_index = NONE;
		tracked_session->flags.set(_session_tracker_session_bit2, session_is_unsuitable(&description->id));

		if (tracked_session->flags.test(_session_tracker_session_bit2))
			tracked_session->__unknown4C = 9;

		tracked_session->flags.set(_session_tracker_session_valid_bit, true);
		m_session_count++;

		// Matchmaking hack: We don't have a valid QoS payload from transport_qos_get_result,
		// but we can just set these to tru to skip the requirement for now.
		// MM TODO: Reimplement QoS payloads via API.
		tracked_session->qos_received[0] = true;
		tracked_session->qos_received[1] = true;
	}
	else
	{
		generate_event(_event_level_warning, "networking:logic:session_tracker: can't track session, session limit reached [%d]", SESSION_STORAGE_COUNT);
	}

	return false;
}

void __cdecl c_session_tracker::add_session_to_unsuitable_sessions(s_transport_secure_identifier const* session_id)
{
	//DECLFUNC(0x004E2600, void, __thiscall, c_session_tracker*, s_transport_secure_identifier const*)(this, session_id);

	if (!session_is_unsuitable(session_id))
	{
		if (m_unsuitable_session_count < m_unsuitable_session_maximum_count)
			m_unsuitable_sessions[m_unsuitable_session_count++].session_id = *session_id;
	}
}

//bool __cdecl c_session_tracker::allocate_storage(e_network_session_tracker_sort_method tracker_sort_method, e_network_session_qos_status_data_type qos_status_data_type, c_matchmaking_quality* matchmaking_quality)
bool __cdecl c_session_tracker::allocate_storage(long tracker_sort_method, long qos_status_data_type, c_matchmaking_quality* matchmaking_quality)
{
	//return DECLFUNC(0x004E2640, bool, __thiscall, c_session_tracker*, long, long, c_matchmaking_quality*)(this, tracker_sort_method, qos_status_data_type, matchmaking_quality);

	m_sort_method = tracker_sort_method;
	m_matchmaking_quality = matchmaking_quality;

	m_expected_qos_data_type = qos_status_data_type;
	m_qos_attempt_count = MIN(MAX(QOS_ATTEMPT_MIN_COUNT, g_network_configuration.logic_qos_attempt_count), QOS_ATTEMPT_MAX_COUNT);

	// logic-session-array
	m_session_storage_size = sizeof(s_network_session_tracker_session) * SESSION_STORAGE_COUNT;
	m_sessions = (s_network_session_tracker_session*)network_heap_allocate_block(m_session_storage_size);

	// logic-unsuitable-session-array
	m_unsuitable_session_maximum_count = g_network_configuration.logic_unsuitable_session_count;
	m_unsuitable_session_storage_size = sizeof(s_session_tracker_unsuitable_session) * m_unsuitable_session_maximum_count;
	m_unsuitable_sessions = (s_session_tracker_unsuitable_session*)network_heap_allocate_block(sizeof(s_session_tracker_unsuitable_session) * m_unsuitable_session_maximum_count);

	if (m_sessions && m_unsuitable_sessions)
	{
		m_flags.set(_session_tracker_initialized_bit, true);
		return true;
	}

	release_storage();

	char network_heap_description[1024]{};
	generate_event(_event_level_error, "networking:logic:session_tracker: unable to allocate storage of sizes [%d/%d/%d] for session arrays [%s]",
		m_session_storage_size,
		m_session_storage_size,
		m_unsuitable_session_storage_size,
		network_heap_describe(network_heap_description, sizeof(network_heap_description)));

	return false;
}

void __cdecl c_session_tracker::build_qos_target_list(e_transport_qos_type qos_type, long* qos_targets, long max_qos_target_count, long* qos_target_count)
{
	DECLFUNC(0x004E2640, void, __thiscall, c_session_tracker*, long, long*, long, long*)(this, qos_type, qos_targets, max_qos_target_count, qos_target_count);
}

//.text:004E27D0 ; 

void __cdecl c_session_tracker::clear()
{
	//DECLFUNC(0x004E27E0, void, __thiscall, c_session_tracker*)(this);

	ASSERT(m_flags.test(_session_tracker_initialized_bit));
	generate_event(_event_level_message, "networking:logic:session_tracker: clearing tracked sessions");

	for (long qos_attemp_index = 0; qos_attemp_index < m_qos_attempts.get_count(); qos_attemp_index++)
		release_qos_attempt(qos_attemp_index);

	m_session_count = 0;
	csmemset(m_sessions, 0, m_session_storage_size);
}

void __cdecl c_session_tracker::clear_qos_attempt(long qos_attempt_index)
{
	//DECLFUNC(0x004E2850, void, __thiscall, c_session_tracker*, long)(this, qos_attempt_index);

	s_session_tracker_qos_attempt& qos_attempt = m_qos_attempts[qos_attempt_index];
	qos_attempt.target_count = 0;
	qos_attempt.time = 0;
	qos_attempt.qos_index = NONE;
	qos_attempt.qos_type = k_transport_qos_type_none;
}

void __cdecl c_session_tracker::clear_unsuitable_sessions()
{
	//DECLFUNC(0x004E2880, void, __thiscall, c_session_tracker*)(this);

	generate_event(_event_level_message, "networking:logic:session_tracker: clearing unsuitable sessions");

	m_unsuitable_session_count = 0;
	csmemset(m_unsuitable_sessions, 0, m_unsuitable_session_storage_size);
}

int __cdecl compare_session_desirability(void const* a, void const* b)
{
	return INVOKE(0x004E28B0, compare_session_desirability, a, b);
}

void __cdecl c_session_tracker::dispose()
{
	//DECLFUNC(0x004E2920, void, __thiscall, c_session_tracker*)(this);

	if (m_flags.test(_session_tracker_initialized_bit))
		release_storage();
}

//.text:004E2960 ; 
//.text:004E2970 ; 
//.text:004E2980 ; returns 10, something like `get_maximum_qos_attempt_count`?

long __cdecl c_session_tracker::get_maximum_qos_target_count(e_transport_qos_type qos_type)
{
	return DECLFUNC(0x004E2990, long, __thiscall, c_session_tracker*, long)(this, qos_type);
}

long __cdecl c_session_tracker::get_session_count()
{
	//return DECLFUNC(0x004E29C0, long, __thiscall, c_session_tracker*)(this);

	return m_session_count;
}

bool __cdecl c_session_tracker::get_session_data(long tracked_session_index, s_network_session_tracker_session_data* session_data)
{
	return DECLFUNC(0x004E29D0, bool, __thiscall, c_session_tracker*, long, s_network_session_tracker_session_data*)(this, tracked_session_index, session_data);
}

void __cdecl c_session_tracker::get_session_status(long tracked_session_index, s_network_session_tracker_session_status* session_status)
{
	//DECLFUNC(0x004E2AB0, void, __thiscall, c_session_tracker*, long, s_network_session_tracker_session_status*)(this, tracked_session_index, session_status);

	s_network_session_tracker_session* tracked_session = &m_sessions[tracked_session_index];

	ASSERT(m_flags.test(_session_tracker_initialized_bit));
	ASSERT(VALID_INDEX(tracked_session_index, m_session_count));

	csmemset(session_status, 0, sizeof(s_network_session_tracker_session_status));
	if (tracked_session->flags.test(_session_tracker_session_bit2))
	{
		session_status->type = 5;
		session_status->__unknown4 = tracked_session->__unknown4C;
	}
	else if (tracked_session->flags.test(_session_tracker_session_bit3))
	{
		session_status->type = 4;
		session_status->__unknown8 = tracked_session->__unknown48;
	}
	else if (tracked_session->qos_received[_transport_qos_type_probe_only])
	{
		session_status->type = tracked_session->qos_received[_transport_qos_type_default] + 2;
	}
	else
	{
		session_status->type = 1;
	}
}

long __cdecl c_session_tracker::get_tracked_session_index(s_transport_secure_identifier const* session_id)
{
	//return DECLFUNC(0x004E2B20, long, __thiscall, c_session_tracker*, s_transport_secure_identifier const*)(this, session_id);

	for (long session_index = 0; session_index < m_session_count; session_index++)
	{
		s_network_session_tracker_session* session = &m_sessions[session_index];
		if (csmemcmp(&session->description.id, session_id, sizeof(s_transport_secure_identifier)) == 0)
		{
			ASSERT(session->flags.test(_session_tracker_session_valid_bit));
			return session_index;
		}
	}

	return NONE;
}

bool __cdecl c_session_tracker::initialize(bool a1)
{
	//return DECLFUNC(0x004E2B90, bool, __thiscall, c_session_tracker*, bool)(this, a1);

	m_sort_method = 0;

	m_session_count = 0;
	m_session_storage_size = 0;
	m_sessions = 0;

	m_unsuitable_session_count = 0;
	m_unsuitable_session_maximum_count = 0;
	m_unsuitable_session_storage_size = 0;
	m_unsuitable_sessions = 0;

	m_qos_attempt_count = 0;

	for (long qos_attempt_index = 0; qos_attempt_index < m_qos_attempts.get_count(); qos_attempt_index++)
	{
		if (a1)
		{
			s_session_tracker_qos_attempt* qos_attempt = &m_qos_attempts[qos_attempt_index];
			ASSERT(qos_attempt->qos_index == NONE);
		}

		clear_qos_attempt(qos_attempt_index);
	}

	m_flags.clear();

	return true;
}

//bool __cdecl c_session_tracker::mark_session_undesireable(s_transport_session_description const* description, e_network_session_tracker_session_undesirable_reason undesirable_reason)
bool __cdecl c_session_tracker::mark_session_undesireable(s_transport_session_description const* description, long undesirable_reason)
{
	//return DECLFUNC(0x004E2D30, bool, __thiscall, c_session_tracker*, s_transport_session_description const*, long)(this, description, undesirable_reason);

	long tracked_session_index = get_tracked_session_index(&description->id);
	ASSERT(m_flags.test(_session_tracker_initialized_bit));

	if (tracked_session_index != NONE)
	{
		ASSERT(VALID_INDEX(tracked_session_index, m_session_count));

		// #TODO: add back `mark_tracked_session_undesireable`
		return true;
	}

	generate_event(_event_level_error, "networking:logic:session_tracker: not tracking session, can't mark as undesireable");
	return false;
}

//bool __cdecl c_session_tracker::mark_session_unsuitable(struct s_transport_session_description const* description, e_network_session_tracker_session_unsuitable_reason unsuitable_reason)
bool __cdecl c_session_tracker::mark_session_unsuitable(s_transport_session_description const* description, long unsuitable_reason)
{
	//return DECLFUNC(0x004E2D60, bool, __thiscall, c_session_tracker*, s_transport_session_description const*, long)(this, description, unsuitable_reason);

	long tracked_session_index = get_tracked_session_index(&description->id);
	ASSERT(m_flags.test(_session_tracker_initialized_bit));

	if (tracked_session_index != NONE)
	{
		ASSERT(VALID_INDEX(tracked_session_index, m_session_count));

		// #TODO: add back `mark_tracked_session_unsuitable`
		return true;
	}

	generate_event(_event_level_error, "networking:logic:session_tracker: not tracking session, can't mark as unsuitable");
	return false;
}

void __cdecl network_session_tracker_clear()
{
	//INVOKE(0x004E2DB0, network_session_tracker_clear);

	session_tracker_globals.session_tracker.clear();
}

void __cdecl network_session_tracker_clear_unsuitable_sessions()
{
	//INVOKE(0x004E2DC0, network_session_tracker_clear_unsuitable_sessions);

	session_tracker_globals.session_tracker.clear_unsuitable_sessions();
}

void __cdecl network_session_tracker_dispose()
{
	//INVOKE(0x004E2DF0, network_session_tracker_dispose);

	session_tracker_globals.session_tracker.dispose();
}

long __cdecl network_session_tracker_get_session_count()
{
	//return INVOKE(0x004E2E40, network_session_tracker_get_session_count);

	return session_tracker_globals.session_tracker.get_session_count();
}

bool __cdecl network_session_tracker_get_session_data(long tracked_session_index, s_network_session_tracker_session_data* session_data)
{
	//return INVOKE(0x004E2E50, network_session_tracker_get_session_data, tracked_session_index, session_data);

	return session_tracker_globals.session_tracker.get_session_data(tracked_session_index, session_data);
}

void __cdecl network_session_tracker_get_session_status(long tracked_session_index, s_network_session_tracker_session_status* session_status)
{
	//return INVOKE(0x004E2E70, network_session_tracker_get_session_status, tracked_session_index, session_status);

	session_tracker_globals.session_tracker.get_session_status(tracked_session_index, session_status);
}

bool __cdecl network_session_tracker_initialize()
{
	//return INVOKE(0x004E2E90, network_session_tracker_initialize);

	return session_tracker_globals.session_tracker.initialize(false);
}

//bool __cdecl network_session_tracker_mark_session_undesirable(s_transport_session_description const* description, e_network_session_tracker_session_undesirable_reason undesirable_reason)
bool __cdecl network_session_tracker_mark_session_undesirable(s_transport_session_description const* description, long undesirable_reason)
{
	//return INVOKE(0x004E2EA0, network_session_tracker_mark_session_undesirable, description, undesirable_reason);

	return session_tracker_globals.session_tracker.mark_session_undesireable(description, undesirable_reason);
}

//bool __cdecl network_session_tracker_mark_session_unsuitable(s_transport_session_description const* description, e_network_session_tracker_session_unsuitable_reason unsuitable_reason)
bool __cdecl network_session_tracker_mark_session_unsuitable(s_transport_session_description const* description, long unsuitable_reason)
{
	//return INVOKE(0x004E2ED0, network_session_tracker_mark_session_unsuitable, description, unsuitable_reason);

	return session_tracker_globals.session_tracker.mark_session_unsuitable(description, unsuitable_reason);
}

bool __cdecl network_session_tracker_session_contacted(long tracked_session_index)
{
	//return INVOKE(0x004E2F00, network_session_tracker_session_contacted, tracked_session_index);

	return session_tracker_globals.session_tracker.session_completed_initial_qos(tracked_session_index);
}

//bool __cdecl network_session_tracker_start(e_network_session_tracker_sort_method tracker_sort_method, e_network_session_qos_status_data_type qos_status_data_type, c_matchmaking_quality* matchmaking_quality)
bool __cdecl network_session_tracker_start(long tracker_sort_method, long qos_status_data_type, c_matchmaking_quality* matchmaking_quality)
{
	//return INVOKE(0x004E2F30, network_session_tracker_start, tracker_sort_method, qos_status_data_type, matchmaking_quality);

	return session_tracker_globals.session_tracker.allocate_storage(tracker_sort_method, qos_status_data_type, matchmaking_quality);
}

void __cdecl network_session_tracker_stop()
{
	//return INVOKE(0x004E2F50, network_session_tracker_stop);

	session_tracker_globals.session_tracker.release_storage();
}

bool __cdecl network_session_tracker_track_session(char const* name, s_transport_session_description const* description)
{
	//return INVOKE(0x004E2F90, network_session_tracker_track_session, name, description);

	return session_tracker_globals.session_tracker.add_session(name, description);
}

void __cdecl network_session_tracker_update()
{
	//INVOKE(0x004E2FB0, network_session_tracker_update);

	session_tracker_globals.session_tracker.update();
}

void __cdecl c_session_tracker::release_qos_attempt(long qos_attempt_index)
{
	//DECLFUNC(0x004E3020, void, __thiscall, c_session_tracker*, long)(this, qos_attempt_index);

	s_session_tracker_qos_attempt& qos_attempt = m_qos_attempts[qos_attempt_index];
	if (qos_attempt.qos_index != NONE)
	{
		transport_qos_delete(qos_attempt.qos_index);
		clear_qos_attempt(qos_attempt.qos_index);
	}
}

void __cdecl c_session_tracker::release_storage()
{
	//DECLFUNC(0x004E3060, void, __thiscall, c_session_tracker*)(this);

	if (m_sessions)
		network_heap_free_block(m_sessions);
	
	if (m_unsuitable_sessions)
		network_heap_free_block(m_unsuitable_sessions);
	
	clear();
	initialize(true);
}

bool __cdecl c_session_tracker::session_completed_initial_qos(long tracked_session_index)
{
	//return DECLFUNC(0x004E30A0, bool, __thiscall, c_session_tracker*, long)(this, tracked_session_index);

	s_network_session_tracker_session* tracked_session = &m_sessions[tracked_session_index];
	ASSERT(m_flags.test(_session_tracker_initialized_bit));
	ASSERT(VALID_INDEX(tracked_session_index, m_session_count));

	return tracked_session->qos_received[0] || tracked_session->qos_received[1];
}

bool __cdecl c_session_tracker::session_is_unsuitable(s_transport_secure_identifier const* session_id)
{
	return DECLFUNC(0x004E30D0, bool, __thiscall, c_session_tracker*, s_transport_secure_identifier const*)(this, session_id);
}

void __cdecl c_session_tracker::update()
{
	//DECLFUNC(0x004E31C0, void, __thiscall, c_session_tracker*)(this);

	if (m_flags.test(_session_tracker_initialized_bit))
	{
		update_sessions_for_refresh();
		for (long qos_attempt_index = 0; qos_attempt_index < m_qos_attempt_count; qos_attempt_index++)
		{
			update_qos_receive(qos_attempt_index);
			update_qos_send(qos_attempt_index);
		}
		update_sort();
	}
}

void __cdecl c_session_tracker::update_qos_receive(long qos_attempt_index)
{
	DECLFUNC(0x004E3220, void, __thiscall, c_session_tracker*, long)(this, qos_attempt_index);
}

void __cdecl c_session_tracker::update_qos_send(long qos_attempt_index)
{
	DECLFUNC(0x004E35D0, void, __thiscall, c_session_tracker*, long)(this, qos_attempt_index);
}

void __cdecl c_session_tracker::update_sessions_for_refresh()
{
	DECLFUNC(0x004E3750, void, __thiscall, c_session_tracker*)(this);
}

void __cdecl c_session_tracker::update_sort()
{
	DECLFUNC(0x004E37D0, void, __thiscall, c_session_tracker*)(this);
}

bool __cdecl c_session_tracker::update_sort_by_desirability()
{
	return DECLFUNC(0x004E3810, bool, __thiscall, c_session_tracker*)(this);
}
