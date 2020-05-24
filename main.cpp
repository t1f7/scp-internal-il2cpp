#include "il2cpp.h"
#include "gui.h"
#include "utils.h"
#include "memory.h"
#include "veh.h"


void render() {

	gui::text("il2cpp plague : v0.5", 15, 15, 255, 0, 0);

	if (!camera) {
		entityList.clear();
		return;
	}

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

			sprintf(buf, "%s - %.2f", TeamName[entityList[i].team], dist);
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

pointer original;

void corrupt_the_game() {

	// something changes in runtime, let's fix it up
	il2cpp::Init();

	camera = il2cpp::get_current_camera();
	if (!camera) return;
	matrix = il2cpp::get_viewmatrix(camera);
	cameraPosition = il2cpp::get_camera_position(camera);

	// collect location data
	// find scp-914
	std::vector<Entity> tmp;
	auto location = il2cpp::find_entities("914_use");
	int objects_num = Read<int>((pointer)location + offset::unity_list_len);
	if (objects_num) {
		auto object = Read<pointer>((pointer)location + offset::unity_list_start);

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

	// dirty things happen here
	corrupt_the_game();

	return result;
}

void PlayerStats_Update(PEXCEPTION_POINTERS ExceptionInfo) {

	// components are at Rcx
	auto PlayerStats_Component = ExceptionInfo->ContextRecord->Rcx;
	pointer ccm = Read<pointer>(PlayerStats_Component + offset::PlayerStats_ccm);
	auto team = Read<int>(ccm + offset::ccm_team);

	// this is how to combine player gameobject with component data
	// it's ok to call il2cpp api here as we're inside game thread inside any VEH hook
	auto player = il2cpp::get_gameobject(PlayerStats_Component);

	// why don't we collect other data here
	vec3 position = il2cpp::get_transform(player);

	// different thread forces us to recreate a list
	// it looks as a bad design but it works so I don't care actually if it's bad
	std::vector<Entity> tmp;
	bool found = false;

	for (int i = 0; i < entityList.size(); i++) {
		if (entityList[i].address == player) {
			found = true;
			// override position for cached entity
			entityList[i].position = position;
		}
		tmp.push_back(entityList[i]);
	}
	entityList = tmp;
	tmp.clear();

	if (!found) {
		entityList.push_back(Entity{
			player,
			position,
			"unnamed", // add nickname here after
			ccm,
			team
		});
	}

}


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {

		if (hinstDll) {
			DisableThreadLibraryCalls(hinstDll);

			// this thread is only for rendering. to get rid of this thread find your way to render gui
			CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
		}

		auto base = il2cpp::GetModuleBase();

		// hooking with VEH example
		AddVectoredExceptionHandler(1, CorruptionHandler);
		VEH.Append(base + offset::PlayerStats_Update, &PlayerStats_Update);

		// spoof function so the game calls hack :)
		auto target = (*(pointer*)(base + 0x01D3DF48) + 0x50); // Console::LateUpdate
		original = *(pointer*)(target);
		*(pointer*)(target) = (pointer)(Console_Update);
	}
	return true;
}