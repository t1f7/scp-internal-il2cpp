#pragma once
#include <vector>

struct Entity {
	uint64_t address;
	vec3 position;
	char name[50];
};

// setup some global to work in different threads
vec3 cameraPosition;
Matrix matrix;
uint64_t camera;
std::vector<Entity> entityList;
std::vector<Entity> locationList;
