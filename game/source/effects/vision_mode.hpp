#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "tag_files/tag_groups.hpp"

enum e_vision_mode_global_flags
{
	_vision_mode_global_first_person_only_bit = 0,

	k_vision_mode_global_flags
};

struct s_vision_mode_ping_falloff
{
	// maximum distance affected
	real radius; // world units

	// intensity control for the pings
	real ping_intensity;

	// speed of the ping wave
	real ping_speed; // world units per second

	// width of the ping wave
	real ping_width; // world units

	// how fast the ping falls off
	real ping_falloff; // power [0-10]

	// delay between the end of one ping, and the beginning of the next, note there can only be one ping at a time
	real ping_delay;
};
static_assert(sizeof(s_vision_mode_ping_falloff) == sizeof(real) * 6);

struct s_vision_mode_function
{
	c_function_definition mapping;
};
static_assert(sizeof(s_vision_mode_function) == sizeof(c_function_definition));

struct s_vision_mode_ping_color
{
	s_vision_mode_function default_color;
	s_vision_mode_function ping_color;

	// controls transparency
	real alpha; // [0-1]

	real intensity;
	real ping_intensity;
	real overlapping_dimming_factor;
};
static_assert(sizeof(s_vision_mode_ping_color) == (sizeof(s_vision_mode_function) * 2) + (sizeof(real) * 4));

struct s_vision_mode_definition
{
	c_flags<e_vision_mode_global_flags, word, k_vision_mode_global_flags> global_flags;
	char internal_version;

	// pad
	byte FJRE[0x1];

	// falloff for on_foot
	// falloff for in_vehicle
	// falloff for flying_camera
	s_vision_mode_ping_falloff ping_falloff[3];

	c_typed_tag_reference<SOUND_TAG> ping_start_sound;

	// WEAPONS EQUIPMENT VEHICLES AND DEVICES
	// FRIENDLY BIPEDS
	// ENEMY BIPEDS
	// TERMINALS
	// EVERYTHING ELSE
	s_vision_mode_ping_color ping_color[5];

	// MASK BITMAP
	// masks the area on screen where the vision mode appears
	c_typed_tag_reference<BITMAP_TAG> mask_bitmap;

	// CAMERA FX OVERALY
	// allows override of camera fx settings when vision mode is on
	c_typed_tag_reference<CAMERA_FX_SETTINGS_TAG> camera_fx_overlay;
};
static_assert(sizeof(s_vision_mode_definition) == 0x194);

struct s_vision_mode_state
{
	byte __data[0x3C];
};
static_assert(sizeof(s_vision_mode_state) == 0x3C);

