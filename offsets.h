#pragma once
#include "pointer.h"

namespace offset {

	// il2cpp
	constexpr pointer create_text = 0x1052040;
	constexpr pointer draw_text = 0x10669A0;
	constexpr pointer none_style = 0x1060210;

	// camera
	constexpr pointer camera = 0x10;
	constexpr pointer matrix = 0xDC;
	constexpr pointer camera_position = 0x42C;

	// unit3d
	constexpr pointer unity_list_len = 0x18;
	constexpr pointer unity_list_start = 0x20;
	constexpr pointer unity_list_offset = 0x8;

	// PlayerStats
	constexpr pointer PlayerStats_Update = 0x15D5900;
	constexpr pointer PlayerStats_ccm = 0x70;
	constexpr pointer ccm_team = 0x158;

	// render
	constexpr pointer GUI = 0x3A3330;
}
