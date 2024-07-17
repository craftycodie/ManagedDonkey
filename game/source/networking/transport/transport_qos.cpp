#include "networking/transport/transport_qos.hpp"

#include "memory/module.hpp"

void __cdecl transport_qos_delete(long index)
{
	INVOKE(0x00433FF0, transport_qos_delete, index);
}

void __cdecl transport_qos_dispose()
{
	INVOKE(0x00434000, transport_qos_dispose);
}

bool __cdecl transport_qos_get_result(long index, long target_index, s_transport_qos_result* result)
{
	result->bandwidth_downstream_bps = 20 * 1024;
	result->bandwidth_upstream_bps = 20 * 1024;
	result->payload_byte_count = 0;
	result->ping_msec_median = 30;
	result->ping_msec_minimum = 70;
	result->probes_received = 1;
	result->probes_sent = 1;
	return true;
	//return INVOKE(0x00434010, transport_qos_get_result, index, target_index, result);
}
HOOK_DECLARE(0x00434010, transport_qos_get_result);

long __cdecl transport_qos_get_target_count(long index)
{
	return INVOKE(0x00434020, transport_qos_get_target_count, index);
}

e_transport_qos_target_status __cdecl transport_qos_get_target_status(long index, long target_index)
{
	return (e_transport_qos_target_status)5;
	//return INVOKE(0x00434030, transport_qos_get_target_status, index, target_index);
}
HOOK_DECLARE(0x00434030, transport_qos_get_target_status);

void __cdecl transport_qos_initialize()
{
	return INVOKE(0x00434040, transport_qos_initialize);
}

bool __cdecl transport_qos_is_complete(long index)
{
	return true;
	//return INVOKE(0x00434050, transport_qos_is_complete, index);
}
HOOK_DECLARE(0x00434050, transport_qos_is_complete);

bool __cdecl transport_qos_listener_get_stats(s_transport_secure_identifier const* secure_identifier, s_transport_qos_listener_stats* stats)
{
	return INVOKE(0x00434060, transport_qos_listener_get_stats, secure_identifier, stats);
}

bool __cdecl transport_qos_listener_initiate(s_transport_secure_identifier const* secure_identifier)
{
	return true;
	//return INVOKE(0x00434070, transport_qos_listener_initiate, secure_identifier);
}
HOOK_DECLARE(0x00434070, transport_qos_listener_initiate);

void __cdecl transport_qos_listener_set_bandwidth_limit(s_transport_secure_identifier const* secure_identifier, bool a2, long bandwidth_limit)
{
	INVOKE(0x00434080, transport_qos_listener_set_bandwidth_limit, secure_identifier, a2, bandwidth_limit);
}

void __cdecl transport_qos_listener_set_reply_block(s_transport_secure_identifier const* secure_identifier, bool a2, long reply_block_size, void const* reply_block)
{
	INVOKE(0x00434090, transport_qos_listener_set_reply_block, secure_identifier, a2, reply_block_size, reply_block);
}

void __cdecl transport_qos_listener_terminate(s_transport_secure_identifier const* secure_identifier)
{
	INVOKE(0x004340A0, transport_qos_listener_terminate, secure_identifier);
}

long __cdecl transport_qos_service_new()
{
	return true;
	//return INVOKE(0x004340B0, transport_qos_service_new);
}
HOOK_DECLARE(0x4340B0, transport_qos_service_new);

void __cdecl transport_qos_shutdown()
{
	INVOKE(0x004340C0, transport_qos_shutdown);
}

void __cdecl transport_qos_startup()
{
	INVOKE(0x004340D0, transport_qos_startup);
}

long __cdecl transport_qos_target_new(e_transport_qos_type type, long target_count, s_transport_qos_target const* target, long a4)
{
	return 1;
	//return INVOKE(0x004340E0, transport_qos_target_new, type, target_count, target, a4);
}
HOOK_DECLARE(0x004340E0, transport_qos_target_new);

