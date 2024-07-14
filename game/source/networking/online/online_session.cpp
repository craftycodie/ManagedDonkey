#include "networking/online/online_session.hpp"

#include "networking/session/network_managed_session.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "memory/module.hpp"

#include <windows.h>

//#include <winsock2.h>
//#include "xlive/xdefs.hpp"


#define VALID_HANDLE(HANDLE) (HANDLE && HANDLE != INVALID_HANDLE_VALUE)

HOOK_DECLARE_CLASS_MEMBER(0x00442C00, c_managed_session_overlapped_task, process_add_players);
HOOK_DECLARE_CLASS_MEMBER(0x00442C10, c_managed_session_overlapped_task, process_add_players_immediately);
HOOK_DECLARE_CLASS_MEMBER(0x00442C20, c_managed_session_overlapped_task, process_create);
HOOK_DECLARE_CLASS_MEMBER(0x00442C30, c_managed_session_overlapped_task, process_delete);
HOOK_DECLARE_CLASS_MEMBER(0x00442C40, c_managed_session_overlapped_task, process_game_end);
HOOK_DECLARE_CLASS_MEMBER(0x00442C50, c_managed_session_overlapped_task, process_game_start);
HOOK_DECLARE_CLASS_MEMBER(0x00442C60, c_managed_session_overlapped_task, process_modify);
//HOOK_DECLARE_CLASS_MEMBER(0x00442C70, c_managed_session_overlapped_task, process_modify_immediately);
HOOK_DECLARE_CLASS_MEMBER(0x00442C80, c_managed_session_overlapped_task, process_remove_players);
HOOK_DECLARE_CLASS_MEMBER(0x00442CA0, c_managed_session_overlapped_task, process_session_host_migrate);
HOOK_DECLARE_CLASS_MEMBER(0x00442CB0, c_managed_session_overlapped_task, start_);

void __thiscall c_managed_session_overlapped_task::filter_local_users(long a2, unsigned __int64 const* a3, bool const* a4, bool const* a5) {
	int v5; // [sp+50h] [-10h]
	int i; // [sp+54h] [-Ch]

	v5 = 0;
	for (i = 0; i < a2; ++i)
	{
		if (a4[i])
		{
			this->m_player_xuids[v5] = a3[i];
			if (a5)
				this->m_private_slots[v5] = a5[i];
			++v5;
		}
	}
	this->m_player_count = v5;
}


//.text:00442C00 ; void __cdecl c_managed_session_overlapped_task::process_add_players(long, void(__cdecl*)(long, bool, dword), s_online_session*, qword const*, bool const*, bool const*, long);
void __thiscall c_managed_session_overlapped_task::process_add_players(long managed_session_index, void(__cdecl* callback)(long, bool, dword), s_online_session* session, qword const* a4, bool const* a5, bool const* a6, long player_count)
{
	c_console::write_line("donkey:matchmaking c_managed_session_overlapped_task::process_add_players with session index %d, player count %d", managed_session_index, player_count);
	m_session = session;
	m_managed_session_index = managed_session_index;
	m_callback = callback;
	m_context = _session_overlapped_task_context_add_players;
	m_callback_value0 = false;
	m_callback_value1 = 0;

	filter_local_users(player_count, a4, a5, a6);

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
bool __thiscall c_managed_session_overlapped_task::process_add_players_immediately(s_online_session* a1, qword const* a2, bool const* a3, bool const* a4, long a5) {
	filter_local_users( a5, a2, a3, a4);
	c_console::write_line("donkey:matchmaking process_add_players_immediately with xuid_count %d", this->m_player_count);
	return 1;
}

bool __cdecl managed_session_build_add_xuid_list(int managed_session_index)
{
	s_online_managed_session* managed_session; // edi
	int desired_player_iterator; // ebx
	s_online_session_player* actual_player; // eax

	managed_session = &online_session_manager_globals.managed_sessions[managed_session_index];
	desired_player_iterator = 16;
	s_online_session_player* desired_player = &managed_session->desired_online_session_state.players[desired_player_iterator - 1];
	do
	{
		desired_player = &managed_session->desired_online_session_state.players[desired_player_iterator - 1];
		if ((desired_player->flags & 1) != 0)
		{
			auto actual_player_iterator = 0;
			actual_player = &managed_session->actual_online_session_state.players[actual_player_iterator];
			while ((actual_player->flags & 1) == 0
				|| desired_player->xuid != actual_player->xuid)
			{
				actual_player = &managed_session->actual_online_session_state.players[actual_player_iterator];

				++actual_player_iterator;
				if (actual_player_iterator >= 16)
				{
					managed_session->xuids[managed_session->session_player_operation_count] = desired_player->xuid;
					managed_session->xuid_next_flags[managed_session->session_player_operation_count++] = (desired_player->flags & 2) != 0;
					break;
				}
			}
		}
		--desired_player_iterator;
	} while (desired_player_iterator);

	network_debug_print(
		"MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_add_xuid_list: session 0x%8X, operating on %d xuids",
		managed_session_index,
		managed_session->session_player_operation_count);

	//c_console::write_line("donkey:matchmaking managed_session_build_add_xuid_list returning %d", managed_session->session_player_operation_count > 0);
	//c_console::write_line("donkey:matchmaking desired player 1: I64u / %d", managed_session->desired_online_session_state.players[0].xuid, managed_session->desired_online_session_state.players[0].flags);
	//c_console::write_line("donkey:matchmaking actual player 1: I64u / %d", managed_session->actual_online_session_state.players[0].xuid, managed_session->actual_online_session_state.players[0].flags);

	//c_console::write_line("donkey:matchmaking desired player 2: I64u / %d", managed_session->desired_online_session_state.players[1].xuid, managed_session->desired_online_session_state.players[1].flags);
	//c_console::write_line("donkey:matchmaking actual player 2: I64u / %d", managed_session->actual_online_session_state.players[1].xuid, managed_session->actual_online_session_state.players[1].flags);

	//c_console::write_line("donkey:matchmaking desired player 3: I64u / %d", managed_session->desired_online_session_state.players[2].xuid, managed_session->desired_online_session_state.players[2].flags);
	//c_console::write_line("donkey:matchmaking actual player 3: I64u / %d", managed_session->actual_online_session_state.players[2].xuid, managed_session->actual_online_session_state.players[2].flags);


	return managed_session->session_player_operation_count > 0;
}
//HOOK_DECLARE(0x00480790, managed_session_build_add_xuid_list);

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
dword __thiscall c_managed_session_overlapped_task::start_(void* pXOverlapped)
{
	c_console::write_line("donkey:matchmaking managed-session-context: %s", get_context_string());

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
		
		// If we have a vaiid session desc already, roll with it.
		if (m_session->description.id.part0 == 0) {
			transport_secure_random(sizeof(m_session->nonce), (byte*)&m_session->nonce);
			transport_secure_random(sizeof(m_session->handle), (byte*)&m_session->handle);
			transport_secure_random(sizeof(m_session->description.id), (byte*)&m_session->description.id);
			transport_secure_address_get(&m_session->description.address);
			m_session->players[0].xuid = 1;
			//transport_secure_random(sizeof(m_session->description.address), (byte*)&m_session->description.address);
		}
		//transport_secure_random(sizeof(m_session->description.key), (byte*)&m_session->description.key); // not used

		//SessionInfo->hostAddress.inaOnline.S_un.S_addr = 0x7F000001;
		//SessionInfo->hostAddress.wPortOnline = 8080;
		//memset(SessionInfo->hostAddress.abEnet, 0x8B, sizeof(SessionInfo->hostAddress.abEnet));
		//memset(SessionInfo->hostAddress.abOnline, 0x56, sizeof(SessionInfo->hostAddress.abOnline));

		c_console::write_line("donkey-matchmaking: Made session with ID '%s'", (const char*)transport_secure_identifier_get_string(&m_session->description.id));

		this->success_(0);
		this->complete_();

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
		//ASSERT(VALID_HANDLE(m_session->handle));

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

		this->success_(0);
		this->complete_();
	}
	break;
	case _session_overlapped_task_context_migrate_host:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);

		//DWORD userIndex = m_is_host ? m_session->controller_index : 0xFE;
		//result = XSessionMigrateHost(m_session->handle, userIndex, m_host_migration_description, (PXOVERLAPPED)overlapped);

		this->success_(0);
		this->complete_();
	}
	break;
	case _session_overlapped_task_context_modify:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		c_console::write_line("donkey-matchmaking: Modifying session '%s'", (const char*)transport_secure_identifier_get_string(&m_session->description.id));


		//ASSERT(VALID_HANDLE(m_session->handle));

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

		this->success_(0);
		this->complete_();

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

		this->success_(0);
		this->complete_();
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

		this->success_(0);
		this->complete_();
	}
	break;
	case _session_overlapped_task_context_start:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);
		ASSERT(VALID_HANDLE(m_session->handle));

		//result = XSessionStart(m_session->handle, (PXOVERLAPPED)overlapped);

		this->success_(0);
		this->complete_();
	}
	break;
	case _session_overlapped_task_context_end:
	{
		c_console::write_line("managed-session-context: %s", get_context_string());

		ASSERT(m_session);
		ASSERT(VALID_HANDLE(m_session->handle));

		//result = XSessionEnd(m_session->handle, (PXOVERLAPPED)overlapped);

		this->success_(0);
		this->complete_();
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

