#include "networking/online/online_session.hpp"

#include "networking/session/network_managed_session.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "memory/module.hpp"

#include <windows.h>

//#include <winsock2.h>
#include "xlive/xdefs.hpp"


#define VALID_HANDLE(HANDLE) (HANDLE && HANDLE != INVALID_HANDLE_VALUE)

HOOK_DECLARE_CLASS_MEMBER(0x00442C00, c_managed_session_overlapped_task, process_add_players);
//HOOK_DECLARE_CLASS_MEMBER(0x00442C10, c_managed_session_overlapped_task, process_add_players_immediately);
HOOK_DECLARE_CLASS_MEMBER(0x00442C20, c_managed_session_overlapped_task, process_create);
HOOK_DECLARE_CLASS_MEMBER(0x00442C30, c_managed_session_overlapped_task, process_delete);
HOOK_DECLARE_CLASS_MEMBER(0x00442C40, c_managed_session_overlapped_task, process_game_end);
HOOK_DECLARE_CLASS_MEMBER(0x00442C50, c_managed_session_overlapped_task, process_game_start);
HOOK_DECLARE_CLASS_MEMBER(0x00442C60, c_managed_session_overlapped_task, process_modify);
//HOOK_DECLARE_CLASS_MEMBER(0x00442C70, c_managed_session_overlapped_task, process_modify_immediately);
HOOK_DECLARE_CLASS_MEMBER(0x00442C80, c_managed_session_overlapped_task, process_remove_players);
HOOK_DECLARE_CLASS_MEMBER(0x00442CA0, c_managed_session_overlapped_task, process_session_host_migrate);
HOOK_DECLARE_CLASS_MEMBER(0x00442CB0, c_managed_session_overlapped_task, start_);

//.text:00442C00 ; void __cdecl c_managed_session_overlapped_task::process_add_players(long, void(__cdecl*)(long, bool, dword), s_online_session*, qword const*, bool const*, bool const*, long);
void __thiscall c_managed_session_overlapped_task::process_add_players(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, qword const* a4, bool const* a5, bool const* a6, long player_count)
{
	m_session = session;
	m_managed_session_index = managed_session_index;
	m_callback = callback;
	m_context = _session_overlapped_task_context_add_players;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	// #TODO: implement `filter_local_users`
	//filter_local_users(player_count, a4, a5, a6);

	if (!m_player_count)
	{
		m_callback_value0 = true;
		reset();
	}
	else if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
	{
		reset();
	}
}

//.text:00442C10 ; bool __cdecl c_managed_session_overlapped_task::process_add_players_immediately(s_online_session*, qword const*, bool const*, bool const*, long)

//.text:00442C20 ; void __cdecl c_managed_session_overlapped_task::process_create(long, void(__cdecl*)(long, bool, dword), s_online_session*, c_flags<e_online_session_flags, word, 9>);
void __thiscall c_managed_session_overlapped_task::process_create(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, word_flags flags)
{
	m_context = _session_overlapped_task_context_create;
	m_session = session;
	m_managed_session_index = managed_session_index;
	m_callback = callback;
	m_callback_value0 = false;
	m_callback_value1 = 0;
	m_online_session_flags = flags;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

//.text:00442C30 ; void __cdecl c_managed_session_overlapped_task::process_delete(long, void(__cdecl*)(long, bool, dword), s_online_session*);
void __thiscall c_managed_session_overlapped_task::process_delete(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session)
{
	m_managed_session_index = managed_session_index;
	m_callback = callback;
	m_session = session;
	m_context = _session_overlapped_task_context_delete;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

//.text:00442C40 ; void __cdecl c_managed_session_overlapped_task::process_game_end(long, void(__cdecl*)(long, bool, dword), s_online_session*);
void __thiscall c_managed_session_overlapped_task::process_game_end(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session)
{
	m_managed_session_index = managed_session_index;
	m_callback = callback;
	m_session = session;
	m_context = _session_overlapped_task_context_end;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

//.text:00442C50 ; void __cdecl c_managed_session_overlapped_task::process_game_start(long, void(__cdecl*)(long, bool, dword), s_online_session*);
void __thiscall c_managed_session_overlapped_task::process_game_start(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session)
{
	m_managed_session_index = managed_session_index;
	m_callback = callback;
	m_session = session;
	m_context = _session_overlapped_task_context_start;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

//.text:00442C60 ; void __cdecl c_managed_session_overlapped_task::process_modify(long, void(__cdecl*)(long, bool, dword), s_online_session*, s_online_session*, s_online_session*);
void __thiscall c_managed_session_overlapped_task::process_modify(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, s_online_session* desired_session, s_online_session* actual_session)
{
	m_managed_session_index = managed_session_index;
	m_desired_session = desired_session;
	m_session = session;
	m_actual_session = actual_session;
	m_context = _session_overlapped_task_context_modify;
	m_callback = callback;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

//.text:00442C70 ; static bool __cdecl c_managed_session_overlapped_task::process_modify_immediately(s_online_session*, s_online_session*);

//.text:00442C80 ; void __cdecl c_managed_session_overlapped_task::process_remove_players(long, void(__cdecl*)(long, bool, dword), s_online_session*, qword const*, bool const*, long);
void __thiscall c_managed_session_overlapped_task::process_remove_players(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, qword const* a4, bool const* a5, long player_count)
{
	m_session = session;
	m_managed_session_index = managed_session_index;
	m_callback = callback;
	m_context = _session_overlapped_task_context_remove_players;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	// #TODO: implement `filter_local_users`
	//filter_local_users(player_count, a5, a6, NULL);

	if (!m_player_count)
	{
		m_callback_value0 = true;
		reset();
	}
	else if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
	{
		reset();
	}
}

//.text:00442C90 ; bool __cdecl c_managed_session_overlapped_task::process_remove_players_immediately(s_online_session*, qword const*, bool const*, long);

//.text:00442CA0 ; void __cdecl c_managed_session_overlapped_task::process_session_host_migrate(long, void(__cdecl*)(long, bool, dword), s_online_session*, bool, s_transport_session_description*);
void __thiscall c_managed_session_overlapped_task::process_session_host_migrate(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, bool is_host, s_transport_session_description* host_migration_description)
{
	m_managed_session_index = managed_session_index;
	m_is_host = is_host;
	m_session = session;
	m_host_migration_description = host_migration_description;
	m_context = _session_overlapped_task_context_migrate_host;
	m_callback = callback;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	if (!overlapped_task_start_internal(this, __FILE__, __LINE__))
		reset();
}

//.text:00442CB0 ; virtual dword __cdecl c_managed_session_overlapped_task::start(void*);
dword __thiscall c_managed_session_overlapped_task::start_(PXOVERLAPPED pXOverlapped)
{
	dword result = 0;

	e_session_overlapped_task_context context = m_context;
	switch (context)
	{
	case _session_overlapped_task_context_create:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);

		DWORD dwFlags = m_session->controller_index;
		DWORD dwUserIndex = m_session->controller_index;
		DWORD dwMaxPublicSlots = m_session->public_slots_flags;
		DWORD dwMaxPrivateSlots = m_session->private_slots_flags;
		ULONGLONG* qwSessionNonce = &m_session->nonce;
		PXSESSION_INFO SessionInfo = (PXSESSION_INFO)&m_session->description;
		
		*qwSessionNonce = 0xDEADBEEF;
		SessionInfo->hostAddress.inaOnline.S_un.S_addr = 0x7F000001;
		SessionInfo->hostAddress.wPortOnline = 8080;
		memset(SessionInfo->sessionID.ab, 0x12, sizeof(SessionInfo->sessionID));
		memset(SessionInfo->keyExchangeKey.ab, 0x34, sizeof(SessionInfo->keyExchangeKey));
		memset(SessionInfo->hostAddress.abEnet, 0x8B, sizeof(SessionInfo->hostAddress.abEnet));
		memset(SessionInfo->hostAddress.abOnline, 0x56, sizeof(SessionInfo->hostAddress.abOnline));

		this->success(0);
		this->complete();

		//if (pXOverlapped) {
		//	//asynchronous

		//	pXOverlapped->InternalLow = ERROR_SUCCESS;
		//	pXOverlapped->InternalHigh = 0;
		//	pXOverlapped->dwExtendedError = ERROR_SUCCESS;

		//	Check_Overlapped(pXOverlapped);

		//	return ERROR_IO_PENDING;
		//}

		//synchronous
		return ERROR_SUCCESS;






		//if (controller_get(m_session->controller_index)->is_signed_in_to_live())
		//{
			//result = XSessionCreate(dwFlags, dwUserIndex, dwMaxPublicSlots, dwMaxPrivateSlots, qwSessionNonce, SessionInfo, (PXOVERLAPPED)overlapped, (PHANDLE)m_session->handle);
		//}
		//else
		//{
		//	result = E_FAIL;
		//}
		
		generate_event(_event_level_message, "networking:managed_session: created XSession handle %08X",
			m_session->handle);
	}
	break;
	case _session_overlapped_task_context_delete:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);
		ASSERT(VALID_HANDLE(m_session->handle));

		//DWORD cbResultsBuffer = 640;
		//XSESSION_LOCAL_DETAILS SessionDetails{};
		//if (XSessionGetDetails(m_session->handle, &cbResultsBuffer, &SessionDetails, NULL))
		//{
		//	generate_event(_event_level_warning, "networking:managed_session: attempting to delete XSession handle %08X, but GetDetails() failed, so we are assuming the session is toast!",
		//		m_session->handle);
		//	result = E_FAIL;
		//}
		//else
		//{
		//	result = XSessionDelete(m_session->handle, (PXOVERLAPPED)overlapped);
		//}
	}
	break;
	case _session_overlapped_task_context_migrate_host:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);

		//DWORD userIndex = m_is_host ? m_session->controller_index : 0xFE;
		//result = XSessionMigrateHost(m_session->handle, userIndex, m_host_migration_description, (PXOVERLAPPED)overlapped);
	}
	break;
	case _session_overlapped_task_context_modify:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(VALID_HANDLE(m_session->handle));

		//DWORD dwMaxPublicSlots;
		//DWORD dwMaxPrivateSlots;
		//DWORD dwFlags = calculate_slot_counts(m_desired_session, m_actual_session, &dwMaxPublicSlots, &dwMaxPrivateSlots);
		//
		//generate_event(_event_level_message, "networking:managed_session:process_modify: flags=%X, private=%d, public=%d",
		//	dwFlags,
		//	dwMaxPublicSlots,
		//	dwMaxPrivateSlots);
		//
		//result = XSessionModify(m_session->handle, dwFlags, dwMaxPublicSlots, dwMaxPrivateSlots, (PXOVERLAPPED)overlapped);

		this->success(0);
		this->complete();

		return 0;
	}
	break;
	case _session_overlapped_task_context_add_players:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(VALID_HANDLE(m_session->handle));
		ASSERT(m_player_xuids);
		ASSERT(m_player_count > 0);
		ASSERT(m_private_slots);

		//result = XSessionJoinRemote(m_session->handle, m_player_count, m_player_xuids, m_private_slots, (PXOVERLAPPED)overlapped);
	}
	break;
	case _session_overlapped_task_context_remove_players:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_player_xuids);
		ASSERT(m_player_count > 0);
		ASSERT(m_session);
		ASSERT(VALID_HANDLE(m_session->handle));

		//result = XSessionLeaveRemote(m_session->handle, m_player_count, m_player_xuids, (PXOVERLAPPED)overlapped);
	}
	break;
	case _session_overlapped_task_context_start:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);
		ASSERT(VALID_HANDLE(m_session->handle));

		//result = XSessionStart(m_session->handle, (PXOVERLAPPED)overlapped);
	}
	break;
	case _session_overlapped_task_context_end:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);
		ASSERT(VALID_HANDLE(m_session->handle));

		//result = XSessionEnd(m_session->handle, (PXOVERLAPPED)overlapped);
	}
	break;
	default:
	{
		c_console::write_line("managed-session-context: unknown %d", context);
	}
	break;
	}

	return result;
}

