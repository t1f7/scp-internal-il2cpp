#pragma once
#pragma warning(disable : 4244 4996 ) 
#include <Windows.h>
#include "offsets.h"
#include "math.h"
#include "utils.h"

constexpr const int TRANSFORM_MOVABLE = 0;
constexpr const int TRANSFORM_IMMOVABLE = 1;

struct Il2CppString;

typedef void*         il2cpp_resolve_icall(const char*);
typedef Il2CppString* il2cpp_string_new(const char*);

typedef void*     t_unity_get_main_camera();
typedef pointer* t_unity_find_objects(Il2CppString*);
typedef pointer* t_unity_get_transform(pointer gameObject);
typedef pointer t_unity_get_gameobject(pointer component);

namespace il2cpp
{

	// strings
	constexpr const char* assemblyName = "GameAssembly.dll";
	constexpr const char* moduleName = "SCPSL.exe";
	constexpr const char* fname_find_gameobjects = "UnityEngine.GameObject::FindGameObjectsWithTag(System.String)";
	constexpr const char* fname_get_transform = "UnityEngine.GameObject::get_transform(System.IntPtr)";
	constexpr const char* fname_get_current_camera = "UnityEngine.Camera::get_main()";
	constexpr const char* fname_get_gameobject = "UnityEngine.Component::get_gameObject()";
	

	// functions
	pointer GetModuleBase();
	template<class T> T* FindFunction(pointer offset);
	void Init();

	// usefull functions
	pointer get_current_camera();
	pointer* find_entities(const char* tag);
	pointer get_gameobject(pointer component);
	vec3 get_camera_position(pointer camera);
	vec3 get_transform(pointer entity, int transform_type = TRANSFORM_MOVABLE);
	Matrix get_viewmatrix();
	Matrix get_viewmatrix(pointer);
}