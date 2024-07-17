#include "network_session_parameters.hpp"
#include "networking/session/network_session.hpp"

bool c_network_session_parameters::parameters_transmitted_to_peers(unsigned __int64 parameter_mask)
{
    const c_network_session_membership* session_membership = this->m_session->get_session_membership();
    long host_peer_index = session_membership->m_shared_network_membership.host_peer_index;
    long current_peer = session_membership->get_first_peer();
    if (current_peer != -1)
    {
        while (1)
        {
            if (current_peer != host_peer_index)
            {
                if (parameter_mask && this->get_outstanding_parameter_update_mask_for_peer(current_peer) != 0)
                    return 0;
            }
            current_peer = session_membership->get_next_peer(current_peer);
            if (current_peer == -1)
                return 1;
        }
    }
    return 1;
}

unsigned __int64 c_network_session_parameters::get_outstanding_parameter_update_mask_for_peer(long peer_index) const
{
    __int64 outstanding_parameter_update_mask = 0;
    for (long parameter_index = 0; parameter_index < 49; ++parameter_index)
    {
        if (this->m_parameter_interfaces[parameter_index]->peer_requires_update(peer_index))
        {
            outstanding_parameter_update_mask |= 1ULL << parameter_index;
        }
    }

    return outstanding_parameter_update_mask;
}
