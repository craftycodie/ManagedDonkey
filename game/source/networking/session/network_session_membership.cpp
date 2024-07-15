#include "memory/module.hpp"
#include "networking/session/network_session_membership.hpp"
#include "networking/messages/network_messages_session_membership.hpp"
#include "networking/session/network_session.hpp"
#include "networking/logic/network_life_cycle.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0044D130, c_network_session_membership, all_peers_have_connectivity_information);


long c_network_session_membership::get_player_index_from_peer(long peer_index)
{
	return DECLFUNC(0x0052E280, long, __cdecl, dword*, long)(m_shared_network_membership.peers[peer_index].player_mask, 16);
}

long c_network_session_membership::get_first_peer() const
{
	long first_peer = NONE;
	for (long i = 0; i < 17; i++)
	{
		if (is_peer_valid(i))
		{
			first_peer = i;
			break;
		}
	}
	return first_peer;
}

long c_network_session_membership::get_next_peer(long peer_index) const
{
	ASSERT(peer_index != NONE);

	long next_peer = NONE;
	for (long i = peer_index + 1; i < 17; i++)
	{
		if (is_peer_valid(i))
		{
			next_peer = i;
			break;
		}
	}
	return next_peer;
}

long c_network_session_membership::get_observer_channel_index(long peer_index) const
{
	ASSERT(is_peer_valid(peer_index));
	return m_local_peers[peer_index].channel_index;
}

long c_network_session_membership::get_peer_from_observer_channel(long observer_channel_index) const
{
	long peer_index = NONE;
	if (observer_channel_index != NONE)
	{
		for (peer_index = get_first_peer();
			peer_index != NONE && get_observer_channel_index(peer_index) != observer_channel_index;
			peer_index = get_next_peer(peer_index))
		{
			;
		}
	}
	c_console::write_line("donkey:matchmaking: get_peer_from_observer_channel for channel %d returned peer_index %d", observer_channel_index, peer_index);
	return peer_index;
}

void c_network_session_membership::set_player_properties(long player_index, long player_update_number, long controller_index, void const* player_data_, long player_voice)
{
	s_network_session_player* player = get_player(player_index);

	bool update_increment = false;
	if (player->desired_configuration_version != player_update_number)
	{
		player->desired_configuration_version = player_update_number;
		update_increment = true;
	}

	if (player->controller_index != controller_index)
	{
		player->controller_index = controller_index;
		update_increment = true;
	}

	s_player_configuration_for_player_properties const* player_data = static_cast<s_player_configuration_for_player_properties const*>(player_data_);
	if (csmemcmp(&player->configuration.client, &player_data->client, sizeof(s_player_configuration_from_client)))
	{
		player->configuration.client = player_data->client;
		update_increment = true;
	}

	if (csmemcmp(&player->configuration.host.appearance.service_tag, &player_data->host_partial.service_tag, sizeof(c_static_wchar_string<5>)))
	{
		player->configuration.host.appearance.service_tag = player_data->host_partial.service_tag;
		update_increment = true;
	}

	if (player->configuration.host.weapon.loadouts[0].bungienet_user != player_data->host_partial.bungienet_user)
	{
		player->configuration.host.weapon.loadouts[0].bungienet_user = player_data->host_partial.bungienet_user;
		update_increment = true;
	}

	if (csmemcmp(&player->configuration.host.armor.loadouts[0].colors, &player_data->host_partial.colors, sizeof(c_static_array<rgb_color, k_color_type_count>)))
	{
		player->configuration.host.armor.loadouts[0].colors = player_data->host_partial.colors;
		update_increment = true;
	}

	if (csmemcmp(&player->configuration.host.armor.loadouts[0].armors, &player_data->host_partial.armors, sizeof(c_static_array<byte, k_armor_type_count>)))
	{
		player->configuration.host.armor.loadouts[0].armors = player_data->host_partial.armors;
		update_increment = true;
	}

	if (csmemcmp(&player->configuration.host.weapon.loadouts[0].consumables, &player_data->host_partial.consumables, sizeof(c_static_array<char, 4>)))
	{
		player->configuration.host.weapon.loadouts[0].consumables = player_data->host_partial.consumables;
		update_increment = true;
	}

	if (player->voice_settings != player_voice)
	{
		player->voice_settings = player_voice;
		update_increment = true;
	}

	if (update_increment)
		increment_update();
}

const s_network_session_peer* c_network_session_membership::get_peer(long peer_index) const {
	return &(this->m_shared_network_membership.peers[peer_index]);
}

bool c_network_session_membership::peer_property_flag_test(int test_type, int flag) const {
	int peer_index;
	bool is_valid_peer_type;

	for (peer_index = 0; peer_index < 16; ++peer_index)
	{
		if (this->is_peer_valid(peer_index))
		{
			const s_network_session_peer* peer = this->get_peer(peer_index);
			is_valid_peer_type = 1;
			if (test_type)
			{
				if (test_type == 1)
				{
					if (this->host_peer_index() != peer_index)
						is_valid_peer_type = 0;
				}
				else if (test_type < 3)
				{
					if (this->host_peer_index() == peer_index)
						is_valid_peer_type = 0;
				}
				else {
					ASSERT2("unreachable");
				}
			}
			else
			{
				is_valid_peer_type = 1;
			}
			if (is_valid_peer_type && !((1 << flag) & peer->properties.flags))
			{
				return 0;
			}
		}
	}
	return 1;
}
//
bool c_network_session_membership::all_peers_have_connectivity_information()
{
	c_console::write_line("donkey:matchmaking c_network_session_membership::all_peers_have_connectivity_information");

	//if (m_session->is_host()) {
		//c_console::write_line("donkey:matchmaking:arbitration we're hosting, setup initial participants!");

		//// This shouldn't be here, but doesn't have a home yet.
		//life_cycle_globals.handler_matchmaking_arbitration.setup_initial_participants(this->m_session);
		//life_cycle_globals.handler_matchmaking_arbitration.m_flags |= 0x80;

	//}


	for (auto peer : this->m_shared_network_membership.peers) {
		peer.properties.connectivity.peer_connectivity_mask = 3;
		peer.properties.estimated_upstream_bandwidth_bps = 20 * 1024;
		peer.properties.connectivity.peer_probe_mask = 3;
	}
	this->m_shared_network_membership.peer_valid_mask.set(0, true);
	this->m_shared_network_membership.peer_valid_mask.set(1, true);

	bool result;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_network_session_membership, all_peers_have_connectivity_information);

	c_console::write_line("donkey:matchmaking all_peers_have_connectivity_information = %s", result ? "YES" : "NO");

	return result;

	//return 1;

	/////

//	int peer_index; // edi
//	int first_peer_index; // esi
//	dword v3; // edx
//	int v4; // eax
//	int v5; // esi
//
//	peer_index = -1;
//	first_peer_index = 0;
//	while (!this->m_shared_network_membership.peer_valid_mask.test(first_peer_index))
//	{
//		if (++first_peer_index >= 17)
//			goto LABEL_6;
//	}
//	peer_index = first_peer_index;
//LABEL_6:
//	if (peer_index == -1)
//		return 1;
//	v3 = this->m_shared_network_membership.peer_valid_flags[0];
//	while ((~this->m_shared_network_membership.peers[peer_index].properties.connectivity.peer_probe_mask & v3) == 0
//		&& this->m_shared_network_membership.peers[peer_index].properties.estimated_upstream_bandwidth_bps)
//	{
//		v4 = peer_index + 1;
//		v5 = -1;
//		if (peer_index + 1 < 17)
//		{
//			while (((1 << (v4 & 0x1F)) & this->m_shared_network_membership.peer_valid_flags[v4 >> 5]) == 0)
//			{
//				if (++v4 >= 17)
//					goto LABEL_15;
//			}
//			v5 = v4;
//		LABEL_15:
//			v3 = this->m_shared_network_membership.peer_valid_flags[0];
//		}
//		peer_index = v5;
//		if (v5 == -1)
//			return 1;
//	}
//	return 0;
}
