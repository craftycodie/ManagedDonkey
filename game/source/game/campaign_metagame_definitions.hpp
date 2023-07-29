#pragma once


struct s_campaign_metagame_style_type;
struct s_campaign_metagame_difficulty;
struct s_campaign_metagame_skull_data;
struct s_campaign_metagame_globals
{
	c_typed_tag_block<s_campaign_metagame_style_type> styles;
	c_typed_tag_block<s_campaign_metagame_difficulty> difficulty;
	c_typed_tag_block<s_campaign_metagame_skull_data> primary_skulls;
	c_typed_tag_block<s_campaign_metagame_skull_data> secondary_skulls;

	long friendly_death_point_count;
	long player_death_point_count;
	long player_betrayal_point_count;

	// how many kills for this to happen
	long multi_kill_count;

	// in what period of time does this have to happen (seconds)
	real multi_kill_window;

	// time after taking a guys shields down with emp damage you have to get the emp kill bonus (seconds)
	real emp_kill_window;

	// like EMP but for the jack-then-kill medal (seconds)
	real jack_kill_window_period;

	// like EMP but for the airborne jack-then-kill medal (seconds)
	real airborne_jack_kill_window_period;

	// window after which death from the grave medal can be awarded (seconds)
	real death_from_grave_required_period;

	// for sword/shotgun/snipes/etc medals
	long minor_spree_count;
	long major_spree_count;

	long killing_spree_threshold;
	long killing_frenzy_threshold;
	long running_riot_threshold;
	long rampage_threshold;
	long untouchable_threshold;
	long invincible_threshold;
	long double_kill_threshold;
	long triple_kill_threshold;
	long overkill_threshold;
	long killtacular_threshold;
	long killtrocity_threshold;
	long killamanjaro_threshold;
	long killtastrophe_threshold;
	long killpocalpyse_threshold;
	long killionaire_threshold;
};
static_assert(sizeof(s_campaign_metagame_globals) == 0x98);

struct s_campaign_metagame_style_type
{
	real style_multiplier;
	long point_count;
	long display_ticks;
	c_string_id survival_mode_event_name;
};
static_assert(sizeof(s_campaign_metagame_style_type) == 0x10);

struct s_campaign_metagame_difficulty
{
	real difficulty_multiplier;
};
static_assert(sizeof(s_campaign_metagame_difficulty) == sizeof(real));

struct s_campaign_metagame_skull_data
{
	real difficulty_multiplier;
};
static_assert(sizeof(s_campaign_metagame_skull_data) == sizeof(real));

struct s_campaign_metagame_scenario;

