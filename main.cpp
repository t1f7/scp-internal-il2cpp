#include "il2cpp.h"
#include "utils.h"
#include "memory.h"
#include "veh.h"


void OnGUI(PEXCEPTION_POINTERS ExceptionInfo) {

	il2cpp::draw_text(Rect{ 5, 30, 100.0f, 100.0f }, "<color=red>il2cpp plague : v0.9</color>");

	camera = il2cpp::get_current_camera();
	if (!camera) return;
	matrix = il2cpp::get_viewmatrix(camera);
	cameraPosition = il2cpp::get_camera_position(camera);

	if (!camera) {
		entityList.clear();
		return;
	}

	auto screenCenterX = il2cpp::get_screen_width() * 0.5;
	auto screenCenterY = il2cpp::get_screen_height() * 0.5;

	char buf[100];
	sprintf(buf, "<color=white>players: %i</color>", entityList.size());
	il2cpp::draw_text(Rect{ 5, 45, 100.0f, 100.0f }, buf);

	// draw players
	for (int i = 0; i < entityList.size(); i++) {
		vec2 out;

		if (WorldToScreen(matrix, entityList[i].position, out, screenCenterX, screenCenterY)) {

			float dist = GetDistance(cameraPosition, entityList[i].position);
			auto color = "white";

			sprintf(buf, "<color=%s>%s - %.2f</color>", color, TeamName[entityList[i].team], dist);
			il2cpp::draw_text(Rect{ out.X, out.Y, 100.0f, 100.0f }, buf);
		}
	}

	// find scp-914
	std::vector<Entity> tmp;
	auto location = il2cpp::find_entities("914_use");
	int objects_num = Read<int>((pointer)location + offset::unity_list_len);
	if (objects_num) {
		auto object = Read<pointer>((pointer)location + offset::unity_list_start);

		vec3 position;
		il2cpp::get_transform(object, &position);
		tmp.push_back({
			object,
			position,
			"SCP-914"
		});
	}

	locationList = tmp;

	// draw locations
	for (int i = 0; i < locationList.size(); i++) {
		vec2 out;

		if (WorldToScreen(matrix, locationList[i].position, out, screenCenterX, screenCenterY)) {

			float dist = GetDistance(cameraPosition, locationList[i].position);
			auto color = "yellow";

			sprintf(buf, "<color=%s>%s - %.2f</color>", color, locationList[i].name, dist);
			il2cpp::draw_text(Rect{ out.X, out.Y, 100.0f, 100.0f }, buf);
		}
	}
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
	vec3 position;
	il2cpp::get_transform(player, &position);

	// different thread forces us to recreate a list
	// it looks as a bad design but it works so I don't care actually if it's bad
	std::vector<Entity> tmp;
	bool found = false;

	for (int i = 0; i < entityList.size(); i++) {
		if (entityList[i].address == player) {
			found = true;
			// override position for cached entity
			entityList[i].position = position;
			entityList[i].ccm = ccm;
			entityList[i].team = team;
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

		auto base = il2cpp::GetModuleBase();
		il2cpp::Init();

		// hooking with VEH example
		AddVectoredExceptionHandler(1, CorruptionHandler);
		VEH.Append(base + offset::PlayerStats_Update, &PlayerStats_Update);
		VEH.Append(base + offset::GUI, &OnGUI);

	}
	return true;
}