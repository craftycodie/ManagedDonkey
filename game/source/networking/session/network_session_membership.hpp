#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/players.hpp"
#include "networking/online/online.hpp"
#include "networking/transport/transport_security.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

struct s_network_session_peer_connectivity
{
	word_flags peer_connectivity_mask;
	word_flags peer_probe_mask;
	long peer_latency_min;
	long peer_latency_est;
	long peer_latency_max;
};

struct s_network_session_peer_properties
{
	c_static_wchar_string<16> peer_name;
	c_static_wchar_string<32> peer_session_name;
	dword peer_mp_map_mask[1];
	long peer_map;
	c_enum<e_network_session_map_status, long, _network_session_map_status_none, k_network_session_map_status_count> peer_map_status;
	long peer_map_progress_percentage;
	qword peer_game_instance;
	short ready_hopper_id;
	short : 16;
	c_enum<e_session_game_start_error, long, _session_game_start_error_none, k_session_game_start_error_count> game_start_error;
	bool peer_has_hard_drive;
	long estimated_downstream_bandwidth_bps;
	long estimated_upstream_bandwidth_bps;
	bool estimated_upstream_is_reliable;
	c_enum<e_online_nat_type, long, _online_nat_type_none, k_online_nat_type_count> nat_type;
	long connectivity_badness_rating;
	long host_badness_rating;
	long client_badness_rating;
	s_network_session_peer_connectivity connectivity;
	c_enum<e_language, long, _language_invalid, k_language_count> language;
	long determinism_version;
	long determinism_compatible_version;
	dword_flags flags;
};
static_assert(sizeof(s_network_session_peer_properties) == 0xC0);

struct s_network_session_peer
{
	s_transport_secure_address secure_address;
	c_enum<e_network_session_peer_state, long, _network_session_peer_state_none, k_network_session_peer_state_count> connection_state;
	long version;
	dword join_start_time;
	long __unknown1C;
	s_network_session_peer_properties properties;
	qword party_nonce;
	qword join_nonce;
	dword player_mask[1];
};
static_assert(sizeof(s_network_session_peer) == 0xF8);

struct s_network_session_player
{
	long desired_configuration_version;
	s_player_identifier player_identifier;
	long peer_index;
	long peer_user_index;
	bool left_game;
	byte __pad15[3];
	long controller_index;
	dword __unknown1C;
	s_player_configuration configuration;
	dword voice_settings;
	dword __unknown1644;
};
static_assert(sizeof(s_network_session_player) == 0x1648);

struct s_network_session_shared_membership
{
	long update_number;
	long leader_peer_index;
	long host_peer_index;
	long private_slot_count;
	long public_slot_count;
	bool friends_only;
	bool are_slots_locked;
	byte __unknown16;
	byte __unknown17;
	long peer_count;
	c_static_flags<17> peer_valid_mask;
	c_static_array<s_network_session_peer, 17> peers;
	long player_count;
	c_static_flags<16> player_valid_mask;
	c_static_array<s_network_session_player, 16> players;
	long player_addition_number;
	long __unknown17524;
};
constexpr size_t agt = OFFSETOF(s_network_session_shared_membership, __unknown17524);
static_assert(sizeof(s_network_session_shared_membership) == 0x17528);

struct s_network_session_peer_channel
{
	dword_flags flags;
	long channel_index;
	long expected_update_number;
};
static_assert(sizeof(s_network_session_peer_channel) == 0xC);

struct s_player_voice_settings
{
	dword_flags flags;
};
static_assert(sizeof(s_player_voice_settings) == 0x4);

struct s_player_add_queue_entry
{
	s_player_identifier player_identifier;
	long player_index;
	long output_user_index;
	long controller_index;
	s_player_configuration_from_client client_configuration;
	s_player_voice_settings voice_settings;
};
static_assert(sizeof(s_player_add_queue_entry) == 0x48);

struct c_network_session;
struct c_network_session_membership
{
	c_network_session* m_session;
	s_network_session_shared_membership m_shared_network_membership;
	c_static_array<s_network_session_shared_membership, 17> m_transmitted_shared_network_membership;
	c_static_array<dword, 17> m_transmitted_shared_network_membership_checksums;
	bool __unknown1A3D1C;
	bool __unknown1A3D1D;
	bool __unknown1A3D1E;
	bool __unknown1A3D1F;
	long m_local_peer_index;
	long m_player_configuration_version;
	s_network_session_peer_channel m_local_peers[17];
	s_player_add_queue_entry m_player_add_queue[4];
	long m_player_add_queue_current_index;
	long m_player_add_queue_count;
};
static_assert(sizeof(c_network_session_membership) == 0x1A3F20);

