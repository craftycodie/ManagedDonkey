#include "interface/user_interface_messages.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "interface/user_interface_memory.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"

#include <windows.h>
#include <assert.h>

//HOOK_DECLARE(0x00A93410, user_interface_messaging_initialize);
//HOOK_DECLARE(0x00A933B0, user_interface_messaging_dispose);
//HOOK_DECLARE(0x00A93420, user_interface_messaging_initialize_for_new_map);
//HOOK_DECLARE(0x00A933C0, user_interface_messaging_dispose_from_old_map);
//HOOK_DECLARE(0x00A934A0, user_interface_messaging_update);
HOOK_DECLARE(0x00A93450, user_interface_messaging_post);
//HOOK_DECLARE(0x00A933D0, user_interface_messaging_get_next_message);
//HOOK_DECLARE(0x00A93430, user_interface_messaging_pop);

// #TODO: find this
//HOOK_DECLARE(0x00000000, user_interface_message_queue_is_empty);

REFERENCE_DECLARE(0x052600D0, c_message_globals, g_message_globals);

//c_controller.obj
e_ui_message_type c_message::get_type() const
{
	FUNCTION_BEGIN(true);

	return m_type;
}

//user_interface_networking.obj
long c_message::get_screen_name() const
{
	FUNCTION_BEGIN(true);

	return m_screen_name;
}

//user_interface_networking.obj
e_controller_index c_message::get_controller() const
{
	FUNCTION_BEGIN(true);

	return m_controller;
}

//user_interface_messages.obj
e_window_index c_message::get_window() const
{
	FUNCTION_BEGIN(true);

	return m_window;
}

//user_interface_window_manager.obj
long c_message::get_game_time_at_creation() const
{
	FUNCTION_BEGIN(true);

	return m_game_time_at_creation;
}

//gui_screen_start_menu.obj
e_event_type c_controller_input_message::get_event_type() const
{
	FUNCTION_BEGIN(true);

	return m_event_type;
}

//gui_screen_start_menu.obj
e_controller_component c_controller_input_message::get_component() const
{
	FUNCTION_BEGIN(true);

	return m_component;
}

//gui_screen_scoreboard.obj
long c_controller_input_message::get_event_value() const
{
	FUNCTION_BEGIN(true);

	return m_event_value;
}

//c_controller.obj
c_xenon_message::e_xenon_message_type c_xenon_message::get_xenon_message_type() const
{
	FUNCTION_BEGIN(true);

	return m_xenon_message_type;
}

//c_controller.obj
long c_xenon_message::get_event_value() const
{
	FUNCTION_BEGIN(true);

	return m_event_value;
}

//saved_film_director.obj
void c_load_screen_message::set_focus_on_load_by_name(long list_name, long column_name, long column_value)
{
	FUNCTION_BEGIN(true);

	m_focus_on_load_list_name = list_name;
	m_focus_on_load_column_name = column_name;
	m_focus_on_load_column_value = column_value;
}

//gui_screen_start_menu.obj
void c_load_screen_message::set_transition_type(e_screen_transition_type transition_type)
{
	FUNCTION_BEGIN(true);

	m_transition_type = transition_type;
}

//gui_screen_start_menu.obj
void c_load_screen_message::set_focus_on_load(long list_name, long element_handle)
{
	FUNCTION_BEGIN(true);

	m_focus_on_load_list_name = list_name;
	m_focus_on_load_element_handle = element_handle;
}

//gui_screen_start_menu.obj
void c_load_screen_message::set_parent_screen_index(long parent_screen_index)
{
	FUNCTION_BEGIN(true);

	m_parent_screen_index = parent_screen_index;
}

//user_interface_window_manager.obj
e_screen_transition_type c_load_screen_message::get_transition_type() const
{
	FUNCTION_BEGIN(true);

	return m_transition_type;
}

//user_interface_window_manager.obj
bool c_load_screen_message::get_respond_to_controller_events() const
{
	FUNCTION_BEGIN(true);

	return m_respond_to_controller_events;
}

//user_interface_window_manager.obj
long c_load_screen_message::get_focus_on_load_list_name() const
{
	FUNCTION_BEGIN(true);

	return m_focus_on_load_list_name;
}

//user_interface_window_manager.obj
long c_load_screen_message::get_focus_on_load_element_handle() const
{
	FUNCTION_BEGIN(true);

	return m_focus_on_load_element_handle;
}

//user_interface_window_manager.obj
long c_load_screen_message::get_focus_on_load_column_name() const
{
	FUNCTION_BEGIN(true);

	return m_focus_on_load_column_name;
}

//user_interface_window_manager.obj
long c_load_screen_message::get_focus_on_load_column_value() const
{
	FUNCTION_BEGIN(true);

	return m_focus_on_load_column_value;
}

//user_interface_window_manager.obj
long c_load_screen_message::get_parent_screen_index() const
{
	FUNCTION_BEGIN(true);

	return m_parent_screen_index;
}

//user_interface_window_manager.obj
long c_load_screen_message::get_layered_position() const
{
	FUNCTION_BEGIN(true);

	return m_layered_position;
}

//user_interface_window_manager.obj
bool c_load_screen_message::get_applies_even_to_codeless_screens() const
{
	FUNCTION_BEGIN(true);

	return m_applies_even_to_codeless_screens;
}

//gui_screen_start_menu.obj
long c_screen_custom_message::get_sub_type() const
{
	FUNCTION_BEGIN(true);

	return m_sub_type;
}

//user_interface_networking.obj
long c_dialog_result_message::get_dialog_name() const
{
	FUNCTION_BEGIN(true);

	return m_dialog_name;
}

//user_interface_networking.obj
e_gui_dialog_choice c_dialog_result_message::get_dialog_result() const
{
	FUNCTION_BEGIN(true);

	return m_dialog_result;
}

//gui_pregame_setup_manager.obj
long c_dialog_result_message::get_dispose_on_success_screen_index() const
{
	FUNCTION_BEGIN(true);

	return m_dispose_on_success_screen_index;
}

//gui_screen_dialog.obj
void c_dialog_result_message::set_dispose_on_success_screen_index(long dispose_on_success_screen_index)
{
	FUNCTION_BEGIN(true);

	m_dispose_on_success_screen_index = dispose_on_success_screen_index;
}

long c_load_dialog_screen_message::get_dialog_screen_name(long dialog_name)
{
	// #TODO: iterate through `user_interface_shared_globals->dialog_descriptions`,
	// check `dialog_name` against `dialog_description->dialog_name`
	// and return `(dialog_description->third_item != -1) + STRING_ID(gui, gui_dialog_two_items)`
	// if `dialog_description` doesn't contain `dialog_name` return `_string_id_invalid`
	
	return _string_id_invalid;
}

//gui_pregame_setup_manager.obj
void c_load_dialog_screen_message::set_dispose_on_success_screen_index(long dispose_on_success_screen_index)
{
	m_dispose_on_success_screen_index = dispose_on_success_screen_index;
}

//gui_screen_dialog.obj
void c_load_dialog_screen_message::set_test_mode(bool test_mode)
{
	m_test_mode = test_mode;
}

c_message_globals::c_message_globals()
{
	FUNCTION_BEGIN(true);

	memset(m_queue, 0, sizeof(m_queue));
	m_next_read = nullptr;
	m_prev_read = nullptr;
	m_xbox_guide_is_active = false;
	//m_system_message_notification_handle = nullptr;
}

c_message_globals::~c_message_globals()
{
	FUNCTION_BEGIN(true);
}

void c_message_globals::initialize()
{
	FUNCTION_BEGIN(true);

	//m_system_message_notification_handle = XNotifyCreateListener(0x6F);
	//XCustomRegisterDynamicActions();
}

void c_message_globals::dispose()
{
	FUNCTION_BEGIN(true);

	//XCustomUnregisterDynamicActions();
	//if (m_system_message_notification_handle && m_system_message_notification_handle != INVALID_HANDLE_VALUE)
	//{
	//	CloseHandle(m_system_message_notification_handle);
	//	m_system_message_notification_handle = nullptr;
	//}
}

void c_message_globals::initialize_for_new_map()
{
	FUNCTION_BEGIN(true);

	for (long i = 0; i < NUMBEROF(m_queue); ++i)
	{
		s_message_queue_node* next = m_queue[i].m_next;
		if (i == NUMBEROF(m_queue) - 1)
			next = m_queue;
		else
			next = &m_queue[i + 1];

		s_message_queue_node* prev = m_queue[i].m_prev;
		if (i)
			prev = &m_queue[i - 1];
		else
			prev = &m_queue[NUMBEROF(m_queue) - 1];

		m_queue[i].m_message = nullptr;
	}

	m_prev_read = m_queue;
	m_next_read = m_queue;
}

void c_message_globals::dispose_from_old_map()
{
	FUNCTION_BEGIN(true);

	empty_queue();
}

void c_message_globals::empty_queue()
{
	FUNCTION_BEGIN(true);

	while (can_read())
	{
		c_message* message = dequeue_node(m_next_read, true);

		//ui_track_delete<c_message const>(message);
		message->destructor(0);
		user_interface_free(message);
	}
}

c_message* c_message_globals::dequeue_node(s_message_queue_node* node, bool unknown)
{
	FUNCTION_BEGIN(true);

	assert(&m_queue[0] <= node && node <= &m_queue[NUMBEROF(m_queue) - 1]);
	assert(node->m_message != nullptr);

	c_message* message = node->m_message;

	if (unknown)
	{
		if (node == m_next_read)
			m_next_read = m_next_read->m_next;

		s_message_queue_node* prev = node->m_prev;
		node->m_next->m_prev = prev;
		prev->m_next = node->m_next;
		node->m_message = 0;

		s_message_queue_node* next = m_prev_read->m_next;
		next->m_prev = node;
		m_prev_read->m_next = node;
		node->m_prev = m_prev_read;
		node->m_next = next;
	}

	return nullptr;
}

bool c_message_globals::can_read()
{
	FUNCTION_BEGIN(false);

	if (m_next_read)
		return m_next_read->m_message == 0;

	return false;
}

void c_message_globals::queue(c_message* message)
{
	FUNCTION_BEGIN(true);

	assert(message != nullptr);
	assert(can_write());

	if (m_prev_read)
	{
		m_prev_read->m_message = message;
		m_prev_read = m_prev_read->m_next;
	}
}

bool c_message_globals::can_write()
{
	FUNCTION_BEGIN(true);

	return !game_in_progress() || g_message_globals.m_next_read->m_message == 0;
}

void c_message_globals::get_next_message(long screen_name, e_controller_index controller, e_window_index window, c_message** message_reference)
{
	FUNCTION_BEGIN(false);

	assert(message_reference != nullptr);

	if (*message_reference)
	{
		if (m_next_read)
		{
			while (m_next_read != m_prev_read)
			{
				if (m_next_read->m_message && m_next_read->m_message == *message_reference)
				{
					m_next_read = m_next_read->m_next;
					break;
				}

				m_next_read = m_next_read->m_next;
				if (!m_next_read)
					break;
			}
		}
	}

	if (m_next_read)
	{
		for (s_message_queue_node* prev_read = m_prev_read; m_next_read != prev_read; prev_read = m_prev_read)
		{
			if (m_next_read->m_message && message_match(m_next_read->m_message, screen_name, controller, window))
			{
				*message_reference = m_next_read->m_message;
				return;
			}

			m_next_read = m_next_read->m_next;
			if (!m_next_read)
				break;
		}
	}

	*message_reference = nullptr;
}

bool c_message_globals::message_match(c_message* message, long screen_name, e_controller_index controller, e_window_index window)
{
	FUNCTION_BEGIN(false);

	bool result = false;
	if ((screen_name == _string_id_invalid || message->get_screen_name() == screen_name)
		&& (controller == k_any_controller || message->get_controller() == controller)
		&& (window == 0xFF || window == 0xFFFFFFFF || message->get_window() == window))
	{
		result = true;
	}
	return result;
}

void c_message_globals::dequeue(c_message* message)
{
	FUNCTION_BEGIN(true);

	for (s_message_queue_node* node = m_next_read; node && node != m_prev_read; node = node->m_next)
	{
		if (node->m_message && node->m_message == message)
		{
			dequeue_node(node, true);
			break;
		}
	}
}

bool c_message_globals::get_xbox_guide_is_active()
{
	FUNCTION_BEGIN(true);

	return m_xbox_guide_is_active;
}

void c_message_globals::set_xbox_guide_is_active(bool xbox_guide_is_active)
{
	FUNCTION_BEGIN(true);

	m_xbox_guide_is_active = xbox_guide_is_active;
}

void* c_message_globals::get_system_message_notification_handle()
{
	FUNCTION_BEGIN(true);

	//return m_system_message_notification_handle;
	return nullptr;
}

void __cdecl user_interface_messaging_initialize()
{
	FUNCTION_BEGIN(true);

	g_message_globals.initialize();
}

void __cdecl user_interface_messaging_dispose()
{
	FUNCTION_BEGIN(true);

	g_message_globals.dispose();
}

void __cdecl user_interface_messaging_initialize_for_new_map()
{
	FUNCTION_BEGIN(true);

	g_message_globals.initialize_for_new_map();
}

void __cdecl user_interface_messaging_dispose_from_old_map()
{
	FUNCTION_BEGIN(true);

	g_message_globals.dispose_from_old_map();
}

void __cdecl user_interface_messaging_update()
{
	FUNCTION_BEGIN(false);

	// #TODO: implement
}

void __cdecl user_interface_messaging_post(c_message* message)
{
	FUNCTION_BEGIN(true);

	assert(message != nullptr);

	HOOK_INVOKE(, user_interface_messaging_post, message);
	return;

	bool message_queued = false;
	if (game_in_progress())
	{
		if (g_message_globals.can_write())
		{
			g_message_globals.queue(message);
			message_queued = true;
		}
		else
		{
			c_console::write_line("ui: failed to post message because message queue is not writeable!");
		}
	}
	else
	{
		c_console::write_line("ui: failed to post message because game is not in progress!");
	}

	if (!message_queued)
	{
		//ui_track_delete<c_message const>(message);
		message->destructor(0);
		user_interface_free(message);
	}
}

bool __cdecl user_interface_messaging_get_next_message(long screen_name, e_controller_index controller, e_window_index window, c_message** message_reference)
{
	FUNCTION_BEGIN(false);

	//return INVOKE(0x00A933D0, user_interface_messaging_get_next_message, screen_name, controller, window, message_reference);

	g_message_globals.get_next_message(screen_name, controller, window, message_reference);
	return message_reference == nullptr;
}

void __cdecl user_interface_messaging_pop(c_message* message)
{
	FUNCTION_BEGIN(true);

	//INVOKE(0x00A93430, user_interface_messaging_pop, message);

	if (game_in_progress())
		g_message_globals.dequeue(message);
}

bool __cdecl user_interface_message_queue_is_empty()
{
	FUNCTION_BEGIN(true);

	if (game_in_progress())
		return !g_message_globals.can_read();

	return true;
}

// c_load_screen_message::c_load_screen_message
c_load_screen_message* load_screen_message_ctor(c_load_screen_message* message, long screen_name, e_controller_index controller, e_window_index window, long layered_position)
{
	FUNCTION_BEGIN(true);

	return DECLFUNC(0x00A92780, c_load_screen_message*, __thiscall, c_load_screen_message*, long, e_controller_index, e_window_index, long)(message, screen_name, controller, window, layered_position);
}
