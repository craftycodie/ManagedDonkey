#include "interface/gui_screens/player_select/gui_player_select_screen_widget.hpp"

#include "game/game_results.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_text.hpp"
#include "interface/user_interface_text_parser.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B08A10, c_gui_player_select_screen_widget, initialize_);

// #TODO: reimplement `c_gui_player_select_screen_widget::update`
// #TODO: reimplement `c_gui_player_select_screen_widget::update_render_state`
// #TODO: reimplement `c_gui_player_select_screen_widget::handle_controller_input_message`
// #TODO: reimplement `c_gui_player_select_screen_widget::initialize_datasource`
// #TODO: reimplement `c_gui_player_select_screen_widget::handle_list_item_chosen`
// #TODO: reimplement `c_gui_player_select_screen_widget::handle_dialog_result`
// #TODO: reimplement `c_gui_player_select_screen_widget::__vftable54`

bool __cdecl parse_target_player_name(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<256> string = this_->m_player_data.host.name.get_string();
		string.copy_to(buffer, MAX(buffer_length, 256));

		return true;
	}
	return false;
};

//bool __cdecl user_interface_get_player_rank_string(e_experience_rank rank, e_experience_grade grade, c_static_wchar_string<1024>* out_string)
bool __cdecl user_interface_get_player_rank_string(long rank, long grade, c_static_wchar_string<1024>* out_string)
{
	return INVOKE(0x00AA55F0, user_interface_get_player_rank_string, rank, grade, out_string);
}

//bool __cdecl user_interface_get_player_grade_string(e_experience_rank rank, e_experience_grade grade, bool a3, bool a4, c_static_wchar_string<1024>* out_string)
bool __cdecl user_interface_get_player_grade_string(long rank, long grade, bool a3, bool a4, c_static_wchar_string<1024>* out_string)
{
	return INVOKE(0x00AA5410, user_interface_get_player_grade_string, rank, grade, a3, a4, out_string);
}

bool __cdecl parse_player_rank(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<1024> string;
		user_interface_get_player_rank_string(13, 3, &string);
		string.copy_to(buffer, MAX(buffer_length, 1024));
	
		return true;
	}
	return false;
};

bool __cdecl parse_player_grade(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<1024> string;
		user_interface_get_player_grade_string(13, 3, false, true, &string);
		string.copy_to(buffer, MAX(buffer_length, 1024));

		return true;
	}
	return false;
};

bool __cdecl parse_xp_value(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<256> string;
		string.print(L"%d", 69420);
		string.copy_to(buffer, MAX(buffer_length, 256));

		return true;
	}
	return false;
};

bool __cdecl parse_skill_value(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<256> string;
		string.print(L"%d", 69420);
		string.copy_to(buffer, MAX(buffer_length, 256));

		return true;
	}
	return false;
};

bool __cdecl parse_ranked_value(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<256> string;
		string.print(L"%d", 69420);
		string.copy_to(buffer, MAX(buffer_length, 256));

		return true;
	}
	return false;
};

bool __cdecl parse_social_value(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<256> string;
		string.print(L"%d", 69420);
		string.copy_to(buffer, MAX(buffer_length, 256));

		return true;
	}
	return false;
};

bool __cdecl parse_custom_value(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	if (this_ && this_->__unknown30D0 && this_ptr != (void*)-0x1AB0)
	{
		c_static_wchar_string<256> string;
		string.print(L"%d", 69420);
		string.copy_to(buffer, MAX(buffer_length, 256));

		return true;
	}
	return false;
};

short game_results_get_player_from_identifier(c_game_results const* game_results, s_player_identifier const* player_identifier)
{
	short player_index = 0;
	while (game_results->players[player_index].player_exists
		&& csmemcmp(&game_results->players[player_index].player_identifier, player_identifier, sizeof(s_player_identifier)) != 0)
	{
		player_index++;
	}

	return player_index;
}

bool __cdecl parse_kills(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;

	//c_gui_player_select_screen_widget* this_ = static_cast<c_gui_player_select_screen_widget*>(this_ptr);
	//
	//long kills = 0;
	//if (c_game_results const* game_results = user_interface_networking_get_final_game_results())
	//{
	//	short player_index = game_results_get_player_from_identifier(game_results, &this_->m_target_player_id);
	//	VALID_INDEX(player_index, NUMBEROF(game_results->statistics.player));
	//	kills = game_results->statistics.player[0].statistics[_game_results_statistic_kills].statistic & 0x7FFF;
	//}
	//
	//c_static_wchar_string<256> string;
	//string.print(L"%d", kills);
	//string.copy_to(buffer, MAX(buffer_length, 256));
	//
	//return true;
};

bool __cdecl parse_deaths(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_kd_ratio(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_headshots(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_best_spree(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_implement_name(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_implement_count(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_most_killed_name(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_most_killed_count(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_most_killed_by_name(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_most_killed_by_count(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

bool __cdecl parse_rematch_timer(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return false;
};

void __thiscall c_gui_player_select_screen_widget::initialize_()
{
	//c_gui_screen_widget::initialize
	DECLFUNC(0x00AB14D0, void, __thiscall, c_gui_screen_widget*)(this);

	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<target-player-name", this, parse_target_player_name));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<player-rank", this, parse_player_rank));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<player-grade", this, parse_player_grade));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<xp-value", this, parse_xp_value));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<skill-value", this, parse_skill_value));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<ranked-value", this, parse_ranked_value));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<social-value", this, parse_social_value));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<custom-value", this, parse_custom_value));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<kills", this, parse_kills));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<deaths", this, parse_deaths));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<kd-ratio", this, parse_kd_ratio));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<headshots", this, parse_headshots));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<best-spree", this, parse_best_spree));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<implement-name", this, parse_implement_name));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<implement-count", this, parse_implement_count));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<most-killed-name", this, parse_most_killed_name));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<most-killed-count", this, parse_most_killed_count));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<most-killed-by-name", this, parse_most_killed_by_name));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<most-killed-by-count", this, parse_most_killed_by_count));
	add_game_tag_parser(new c_magic_string_game_tag_parser(L"<rematch-timer", this, parse_rematch_timer));
}
