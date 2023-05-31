#include "networking/messages/network_messages_session_membership.hpp"

#include "networking/messages/network_message_type_collection.hpp"

void __cdecl c_network_message_membership_update::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_membership_update::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_peer_properties::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_peer_properties::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_delegate_leadership::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_delegate_leadership::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_boot_machine::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_boot_machine::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_player_add::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_player_add::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_player_refuse::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_player_refuse::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_player_remove::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_player_remove::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl c_network_message_player_properties::encode(c_bitstream* packet, long message_storage_size, void const* message_storage)
{
}

bool __cdecl c_network_message_player_properties::decode(c_bitstream* packet, long message_storage_size, void* message_storage)
{
	return false;
}

void __cdecl network_message_types_register_session_membership(c_network_message_type_collection* message_collection)
{
	ASSERT(message_collection);

	message_collection->register_message_type(
		_network_message_membership_update,
		"membership-update",
		0,
		sizeof(s_network_message_membership_update),
		sizeof(s_network_message_membership_update),
		c_network_message_membership_update::encode,
		c_network_message_membership_update::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_peer_properties,
		"peer-properties",
		0,
		sizeof(s_network_message_peer_properties),
		sizeof(s_network_message_peer_properties),
		c_network_message_peer_properties::encode,
		c_network_message_peer_properties::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_delegate_leadership,
		"delegate-leadership",
		0,
		sizeof(s_network_message_delegate_leadership),
		sizeof(s_network_message_delegate_leadership),
		c_network_message_delegate_leadership::encode,
		c_network_message_delegate_leadership::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_boot_machine,
		"boot-machine",
		0,
		sizeof(s_network_message_boot_machine),
		sizeof(s_network_message_boot_machine),
		c_network_message_boot_machine::encode,
		c_network_message_boot_machine::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_add,
		"player-add",
		0,
		sizeof(s_network_message_player_add),
		sizeof(s_network_message_player_add),
		c_network_message_player_add::encode,
		c_network_message_player_add::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_refuse,
		"player-refuse",
		0,
		sizeof(s_network_message_player_refuse),
		sizeof(s_network_message_player_refuse),
		c_network_message_player_refuse::encode,
		c_network_message_player_refuse::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_remove,
		"player-remove",
		0,
		sizeof(s_network_message_player_remove),
		sizeof(s_network_message_player_remove),
		c_network_message_player_remove::encode,
		c_network_message_player_remove::decode,
		nullptr,
		nullptr);

	message_collection->register_message_type(
		_network_message_player_properties,
		"player-properties",
		0,
		sizeof(s_network_message_player_properties),
		sizeof(s_network_message_player_properties),
		c_network_message_player_properties::encode,
		c_network_message_player_properties::decode,
		nullptr,
		nullptr);
}

