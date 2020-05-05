#pragma once
#include <math.h>

struct Matrix
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};

struct vec3 {
	float X;
	float Y;
	float Z;

	vec3 operator * (float other)
	{
		vec3 buf;
		buf.X *= other;
		buf.Y *= other;
		buf.Z *= other;
		return buf;
	}
	vec3 operator + (float other)
	{
		vec3 buf;
		buf.X += other;
		buf.Y += other;
		buf.Z += other;
		return buf;
	}
	vec3 operator + (vec3 other)
	{
		vec3 buf;
		buf.X = X + other.X;
		buf.Y = Y + other.Y;
		buf.Z = Z + other.Z;
		return buf;
	}
	vec3 operator / (float other)
	{
		vec3 buf;
		buf.X /= other;
		buf.Y /= other;
		buf.Z /= other;
		return buf;
	}
	vec3& operator *= (float other)
	{
		X *= other;
		Y *= other;
		Z *= other;
		return *this;
	}
	float get_length_sqr(void) const
	{
		float buf;
		buf += (X * X);
		buf += (Y * Y);
		buf += (Z * Z);

		return buf;
	}
};

struct vec2 {
	float X;
	float Y;
};
