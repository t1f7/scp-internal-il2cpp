#pragma once
#include "math.h"
#include <cstdint>
#include <stdio.h>

float GetDistance(vec3 p1, vec3 p2);
bool WorldToScreen(Matrix &matrix, vec3 &from, vec2 &to, int ScreenCenterX, int ScreenCenterY);
template <typename T> T Read(uint64_t addr)
{
	T Novalue = {};
	if (!IsBadReadPtr((const void*)addr, sizeof(T)))
	{
		return *(T*)(addr);
	}
	else
	{
		return Novalue;
	}
};
