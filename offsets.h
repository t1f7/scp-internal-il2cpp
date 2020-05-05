#pragma once
#include <cstdint>

namespace offset {

	// il2cpp
	constexpr ::std::uint64_t il2cpp_string_new = 0x1D6C60;
	constexpr ::std::uint64_t il2cpp_resolve_icall = 0x1DA710;

	// camera
	constexpr ::std::uint64_t camera = 0x10;
	constexpr ::std::uint64_t matrix = 0xDC;
	constexpr ::std::uint64_t camera_position = 0x42C;

	// unit3d
	constexpr ::std::uint64_t unity_list_len = 0x18;
	constexpr ::std::uint64_t unity_list_start = 0x20;
	constexpr ::std::uint64_t unity_list_offset = 0x8;

	// transform [Player]
	constexpr ::std::uint64_t transform_component = 0x10;
	constexpr ::std::uint64_t transform_component_data = 0x38;
	constexpr ::std::uint64_t transform_data_vector = 0x90;

	// transform [Structure]
	constexpr ::std::uint64_t transform_data_vector_structure = 0x1D0;

	// (*(uint64_t*)(il2cpp::GetModuleBase() + 0x01D3DF48) + 0x50); // Console::LateUpdate
}
