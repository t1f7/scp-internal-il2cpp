#include "il2cpp.h"
#include "gui.h"
#include "utils.h"
#include "memory.h"


void render() {

	gui::text("il2cpp plague : v0.4", 15, 15, 255, 0, 0);

	if (!camera) return;

	auto screenCenterX = gui::Width * 0.5;
	auto screenCenterY = gui::Height * 0.5;

	char buf[100];
	sprintf(buf, "players: %i", entityList.size());
	gui::text(buf, 15, 30, 255, 0, 0);

	// draw players
	for (int i = 0; i < entityList.size(); i++) {
		vec2 out;

		if (WorldToScreen(matrix, entityList[i].position, out, screenCenterX, screenCenterY)) {

			float dist = GetDistance(cameraPosition, entityList[i].position);

			sprintf(buf, "%.2f", dist);
			gui::text(buf, out.X, out.Y, 255, 255, 255);
		}
	}

	// draw locations
	for (int i = 0; i < locationList.size(); i++) {
		vec2 out;

		if (WorldToScreen(matrix, locationList[i].position, out, screenCenterX, screenCenterY)) {

			float dist = GetDistance(cameraPosition, locationList[i].position);

			sprintf(buf, "%s - %.2f", locationList[i].name, dist);
			gui::text(buf, out.X, out.Y, 255, 255, 0);
		}
	}
}

DWORD WINAPI OnDllAttach(LPVOID inj_hModule)
{
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

uint64_t original;

void corrupt_the_game() {

	// something changes in runtime, let's fix it up
	il2cpp::Init();

	camera = il2cpp::get_current_camera();
	if (!camera) return;
	matrix = il2cpp::get_viewmatrix(camera);
	cameraPosition = il2cpp::get_camera_position(camera);

	// find players
	auto playerdata = il2cpp::find_entities("Player");
	int objects_num = Read<int>((uint64_t)playerdata + offset::unity_list_len);

	std::vector<Entity> tmp;

	// collect player data in game thread
	for (int i = 0; i < objects_num; i++) {

		auto player = Read<uint64_t>((uint64_t)playerdata + offset::unity_list_start + i * offset::unity_list_offset);

		vec3 position = il2cpp::get_transform(player);

		tmp.push_back({
			player,
			position
		});

	}

	entityList = tmp;
	tmp.clear();

	// collect location data
	// find scp-914
	auto location = il2cpp::find_entities("914_use");
	objects_num = Read<int>((uint64_t)location + offset::unity_list_len);
	if (objects_num) {
		auto object = Read<uint64_t>((uint64_t)location + offset::unity_list_start);

		vec3 position = il2cpp::get_transform(object, TRANSFORM_IMMOVABLE);
		tmp.push_back({
			object,
			position,
			"SCP-914"
		});
	}

	locationList = tmp;
	tmp.clear();
}

static __int64 __fastcall Console_Update(void* a1) {
	static auto fn = reinterpret_cast<__int64(__fastcall*)(void*)>(original);
	auto result = fn(a1);

	// do our dirty things here
	corrupt_the_game();

	return result;
}


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {

		if (hinstDll) {
			DisableThreadLibraryCalls(hinstDll);

			// this thread is only for rendering. to get rid of this thread find your way to render gui
			CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
		}

		// spoof function so the game calls hack :)
		auto target = (*(uint64_t*)(il2cpp::GetModuleBase() + 0x01D3DF48) + 0x50);
		original = *(uint64_t*)(target);
		*(uint64_t*)(target) = (DWORD64)(Console_Update);
	}
	return true;
}