#include "game/game_globals.hpp"

#include "cache/cache_files.hpp"
#include "memory/thread_local.hpp"
#include "scenario/scenario.hpp"

game_globals_storage* game_globals_get()
{
	if (!get_tls())
		return nullptr;

	TLS_DATA_GET_VALUE_REFERENCE(game_globals);
	return game_globals;
}

long get_map_minor_version()
{
	if (global_scenario)
		return cache_files_get_header()->minor_version;
	else
		return 0xFFFFFFFF;
}

void s_game_globals::update_reference_names()
{
	ai_globals_ref.get_name();
	multiplayer_globals.get_name();
	survival_mode_globals.get_name();
	rasterizer_globals_ref.get_name();
	default_camera_fx_settings.get_name();
	podium_definition.get_name();
	default_wind_settings.get_name();
	collision_damage_effect.get_name();
	collision_damage.get_name();
	effect_globals.get_name();
	game_progression.get_name();
	game_achievements.get_name();
	input_globals.get_name();

	UPDATE_REFERENCE_NAMES(havok_cleanup_resources);
	UPDATE_REFERENCE_NAMES(sound_globals);
	UPDATE_REFERENCE_NAMES(ai_globals);
	UPDATE_REFERENCE_NAMES(sounds);
	UPDATE_REFERENCE_NAMES(camera);
	UPDATE_REFERENCE_NAMES(grenades);
	UPDATE_REFERENCE_NAMES(cheat_weapons);
	UPDATE_REFERENCE_NAMES(cheat_powerups);
	UPDATE_REFERENCE_NAMES(player_information);
	UPDATE_REFERENCE_NAMES(player_representation);
	UPDATE_REFERENCE_NAMES(player_representation_debug);
}

void s_game_globals_havok_cleanup_resources::update_reference_names()
{
	object_cleanup_effect.get_name();
}

void s_game_globals::sound_block::update_reference_names()
{
	sound.get_name();
}

void s_game_globals_camera::update_reference_names()
{
	default_unit_camera_track.get_name();
}

void s_game_globals_grenade::update_reference_names()
{
	throwing_effect.get_name();
	equipment.get_name();
	projectile.get_name();
}

void s_game_globals_player_information::update_reference_names()
{
	coop_countdown_sound.get_name();
	coop_respawn_sound.get_name();
	coop_respawn_effect.get_name();
	flashlight_on.get_name();
	flashlight_off.get_name();
	default_damage_response.get_name();
}

void s_game_globals_player_representation::update_reference_names()
{
	first_person_hands.get_name();
	first_person_body.get_name();
	first_person_unit.get_name();
	binoculars_zoom_in_sounds.get_name();
	binoculars_zoom_out_sounds.get_name();
	voice.get_name();
}

template<tag group_tag>
void s_game_globals_tag_reference<group_tag>::update_reference_names()
{
	get_name();
}

