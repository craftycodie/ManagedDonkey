#include "game/game_engine_slayer.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F70, c_game_engine_slayer_variant*, slayer_variant);

void c_game_engine_slayer_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_score_to_win);
	bswap_word_inplace(m_score_unknown);
	bswap_word_inplace(m_kill_points);
	m_leader_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}


void c_slayer_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine::dump_settings(file);

}

