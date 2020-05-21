#include "il2cpp.h"
#include "gui.h"
#include "utils.h"

void render() {
	gui::text("il2cpp plague : v0.3", 15, 15, 255, 0, 0);

	// something is changed during runtime, simple fix - update data each frame
	il2cpp::Init();

	auto camera = il2cpp::get_current_camera();
	if (!camera) return;
	auto matrix = il2cpp::get_viewmatrix(camera);
	auto cameraPosition = il2cpp::get_camera_position(camera);
	auto screenCenterX = gui::Width * 0.5;
	auto screenCenterY = gui::Height * 0.5;

	// find players
	auto playerdata = il2cpp::find_entities("Player");
	int objects_num = Read<int>((uint64_t)playerdata + offset::unity_list_len);

	char buf[100];
	sprintf(buf, "players: %i", objects_num);
	gui::text(buf, 15, 30, 255, 0, 0);

	// draw players
	for (int i = 0; i < objects_num; i++) {

		auto player = Read<uint64_t>((uint64_t)playerdata + offset::unity_list_start + i * offset::unity_list_offset);

		vec3 position = il2cpp::get_transform(player);
		vec2 out;

		if (WorldToScreen(matrix, position, out, screenCenterX, screenCenterY)) {

			float dist = GetDistance(cameraPosition, position);

			sprintf(buf, "%.2f", dist);
			gui::text(buf, out.X, out.Y, 255, 255, 255);
		}
	}

	// find 914
	auto location = il2cpp::find_entities("914_use");
	objects_num = Read<int>((uint64_t)location + offset::unity_list_len);
	if (objects_num) {
		auto object = Read<uint64_t>((uint64_t)location + offset::unity_list_start);

		vec3 position = il2cpp::get_transform(object, TRANSFORM_IMMOVABLE);
		vec2 out;

		if (WorldToScreen(matrix, position, out, screenCenterX, screenCenterY)) {

			float dist = GetDistance(cameraPosition, position);

			sprintf(buf, "SCP-914: %.2f", dist);
			gui::text(buf, out.X, out.Y, 255, 255, 0);
		}
	}
}

DWORD WINAPI OnDllAttach(LPVOID inj_hModule)
{
	il2cpp::Init();

	// gui
	gui::RegisterDLLWindowClass((wchar_t*)L"Plague-SCPSL");
	auto hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_COMPOSITED | WS_EX_TRANSPARENT, "overlay", "overlay", WS_EX_TOPMOST | WS_POPUP, 100, 100, gui::Width, gui::Height, NULL, NULL, NULL, inj_hModule);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
	ShowWindow(hWnd, SW_SHOW);
	gui::DirectXxInit(hWnd);
	gui::render_hack = &render;

	for (;;)
	{
		gui::RenderLoop("SCPSL");
	}
	return 1;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		if (hinstDll) DisableThreadLibraryCalls(hinstDll);
		
		// you would like to replace "CreateThread" with injection to game thread to prevent GC crash and possible detection vector.
		CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
	}
	return true;
}