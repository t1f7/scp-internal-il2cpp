#include "utils.h"


float GetDistance(vec3 p1, vec3 p2)
{
	float diffY = p1.Y - p2.Y;
	float diffX = p1.X - p2.X;
	float diffZ = p1.Z - p2.Z;
	return sqrtf((diffY * diffY) + (diffX * diffX) + (diffZ * diffZ));
}

bool WorldToScreen(Matrix &matrix, vec3 &from, vec2 &to, int ScreenCenterX, int ScreenCenterY)
{
	float ViewW = matrix._14 * from.X + matrix._24 * from.Y + matrix._34 * from.Z + matrix._44;

	if (ViewW > 0.01f)
	{
		ViewW = 1.0f / ViewW;

		to.X = ScreenCenterX + (matrix._11 * from.X + matrix._21 * from.Y + matrix._31 * from.Z + matrix._41) * ViewW * ScreenCenterX;
		to.Y = ScreenCenterY - (matrix._12 * from.X + matrix._22 * from.Y + matrix._32 * from.Z + matrix._42) * ViewW * ScreenCenterY;
		return true;
	}
	return false;
}

