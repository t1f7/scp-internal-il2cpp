#pragma once
#pragma warning(disable : 4244 4996 ) 
#include <Windows.h>
#include "offsets.h"
#include "math.h"
#include "utils.h"

struct Il2CppString;

typedef void*         il2cpp_resolve_icall(const char*);
typedef Il2CppString* il2cpp_string_new(const char*);

typedef void*     t_unity_get_main_camera();
typedef uint64_t* t_unity_find_objects(Il2CppString*);
typedef uint64_t* t_unity_get_transform(uint64_t gameObject);

namespace il2cpp
{

	// strings
	constexpr const char* assemblyName = "GameAssembly.dll";
	constexpr const char* moduleName = "SCPSL.exe";
	constexpr const char* fname_find_gameobjects = "UnityEngine.GameObject::FindGameObjectsWithTag(System.String)";
	constexpr const char* fname_get_transform = "UnityEngine.GameObject::get_transform(System.IntPtr)";
	constexpr const char* fname_get_current_camera = "UnityEngine.Camera::get_main()";
	

	// functions
	DWORD64 GetModuleBase();
	template<class T> T* FindFunction(DWORD64 offset);
	void Init();

	// usefull functions
	uint64_t get_current_camera();
	uint64_t* find_entities(const char* tag);
	vec3 get_camera_position(uint64_t camera);
	vec3 get_transform(uint64_t entity);
	Matrix get_viewmatrix();
	Matrix get_viewmatrix(uint64_t);
}