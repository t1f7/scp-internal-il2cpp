#include "il2cpp.h"

namespace il2cpp
{
	// variables
	pointer moduleBase;
	pointer assemblyBase;

	// api
	il2cpp_string_new unity_string_new;
	il2cpp_resolve_icall unity_resolve_icall;
	t_unity_find_objects unity_find_objects;
	t_unity_get_transform unity_get_transform;
	t_unity_transform_get_vector unity_transform_get_vector;
	t_unity_get_main_camera unity_get_main_camera;
	t_unity_get_gameobject unity_get_gameobject;
	t_unity_create_gui_text unity_create_text;
	t_unity_label unity_draw_text;
	t_unity_no_style unity_none_style;
	t_unity_get_screen_data unity_get_screen_width;
	t_unity_get_screen_data unity_get_screen_height;

	pointer GetModuleBase()
	{
		if (assemblyBase == 0)
		{
			assemblyBase = (pointer)GetModuleHandleA(assemblyName);
		}
		return assemblyBase;
	}

	void Init() {

		// grab data
		unity_string_new = (il2cpp_string_new)GetProcAddress((HMODULE)assemblyBase, "il2cpp_string_new");
		unity_resolve_icall = (il2cpp_resolve_icall)GetProcAddress((HMODULE)assemblyBase, "il2cpp_resolve_icall");
		unity_find_objects = (t_unity_find_objects)unity_resolve_icall(fname_find_gameobjects);
		unity_get_transform = (t_unity_get_transform)unity_resolve_icall(fname_get_transform);
		unity_transform_get_vector = (t_unity_transform_get_vector)unity_resolve_icall(fname_get_transform_vector);
		unity_get_main_camera = (t_unity_get_main_camera)unity_resolve_icall(fname_get_current_camera);
		unity_get_gameobject = (t_unity_get_gameobject)unity_resolve_icall(fname_get_gameobject);
		unity_get_screen_width = (t_unity_get_screen_data)unity_resolve_icall(fname_screen_width);
		unity_get_screen_height = (t_unity_get_screen_data)unity_resolve_icall(fname_screen_height);

		// unity3D rendering
		unity_create_text = (t_unity_create_gui_text)(assemblyBase + offset::create_text);
		unity_draw_text = (t_unity_label)(assemblyBase + offset::draw_text);
		unity_none_style = (t_unity_no_style)(assemblyBase + offset::none_style);

	}

	int get_screen_width() {
		return unity_get_screen_width();
	}

	int get_screen_height() {
		return unity_get_screen_height();
	}

	void draw_text(Rect position, const char* text) {
		auto il2cpp_string = unity_string_new(text);
		auto content = unity_create_text(il2cpp_string);
		auto style = unity_none_style();
		unity_draw_text(position, content, style);
	}

	pointer get_current_camera() {
		return unity_get_main_camera();
	}

	pointer* find_entities(const char* tag) {
		auto il2cpp_string = unity_string_new(tag);
		return unity_find_objects(il2cpp_string);
	}

	void get_transform(pointer entity, vec3* pos) {
		auto transform = (pointer*)unity_get_transform(entity);
		unity_transform_get_vector(transform, pos);
	}

	pointer get_gameobject(pointer component) {
		auto go = unity_get_gameobject(component);
		return go; 
	}

	vec3 get_camera_position(pointer camera) {
		if (camera != NULL) {
			camera = Read<pointer>((pointer)camera + offset::camera);
			return Read<vec3>(camera + offset::camera_position);
		}
		return vec3{};
	}

	Matrix get_viewmatrix() {
		auto camera = get_current_camera();
		return get_viewmatrix(camera);
	}
	Matrix get_viewmatrix(pointer camera) {
		if (camera != NULL) {
			camera = Read<pointer>((pointer)camera + offset::camera);
			return Read<Matrix>(camera + offset::matrix);
		}
		return Matrix{};
	}

}
