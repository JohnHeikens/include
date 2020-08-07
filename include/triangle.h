#include "vec3.h"
#include "vec2.h"
#pragma once
struct triangle
{
	vec3 p[3];
	vec2 t[3];
	vec3 screen[3];
	vec3 light[3];
};