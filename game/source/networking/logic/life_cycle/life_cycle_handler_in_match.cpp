#include "networking/logic/life_cycle/life_cycle_handler_in_match.hpp"

void c_life_cycle_state_handler_in_match::update()
{
	DECLFUNC(0x00491270, void, __thiscall, c_life_cycle_state_handler_in_match*)(this);
}

e_life_cycle_state_transition_type c_life_cycle_state_handler_in_match::update_for_state_transition()
{
	return DECLFUNC(0x00491480, e_life_cycle_state_transition_type, __thiscall, c_life_cycle_state_handler_in_match*)(this);
}

void c_life_cycle_state_handler_in_match::enter(c_life_cycle_state_handler* handler, long entry_data_size, void* entry_data)
{
	DECLFUNC(0x00491080, void, __thiscall, c_life_cycle_state_handler_in_match*, c_life_cycle_state_handler*, long, void*)(this, handler, entry_data_size, entry_data);
}

void c_life_cycle_state_handler_in_match::exit(c_life_cycle_state_handler* handler)
{
	DECLFUNC(0x004910E0, void, __thiscall, c_life_cycle_state_handler_in_match*, c_life_cycle_state_handler*)(this, handler);
}

char const* c_life_cycle_state_handler_in_match::get_state_string()
{
	return DECLFUNC(0x00454700, char const*, __thiscall, c_life_cycle_state_handler_in_match*)(this);
}

void c_life_cycle_state_handler_in_match::initialize(c_life_cycle_state_manager* manager)
{
	//DECLFUNC(0x004911C0, void, __thiscall, c_life_cycle_state_handler_in_match*, c_life_cycle_state_manager*)(this, manager);

	c_flags<e_life_cycle_state_handler_flags, byte, k_life_cycle_state_handler_flags> handler_flags{};
	handler_flags.set(_life_cycle_state_handler_unknown_bit0, true);
	handler_flags.set(_life_cycle_state_handler_unknown_bit1, true);
	handler_flags.set(_life_cycle_state_handler_allows_group_session_bit, true);
	handler_flags.set(_life_cycle_state_handler_group_session_disconnect_recreates_group_bit, true);

	c_life_cycle_state_handler::initialize(manager, _life_cycle_state_in_match, &handler_flags, 0, 0);
}

