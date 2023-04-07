#include "game/game_engine_vip.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F88, c_game_engine_vip_variant*, vip_variant);

void c_game_engine_vip_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_score_to_win_round);
	bswap_word_inplace(m_score_unknown);
	bswap_word_inplace(m_variant_flags);

	ASSERT(array_is_zeroed(m_pad1));

	bswap_word_inplace(m_influence_radius);
	m_vip_team_traits.byteswap();
	m_vip_influence_traits.byteswap();
	m_vip_traits.byteswap();
}

void c_vip_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine::dump_settings(file);

}

