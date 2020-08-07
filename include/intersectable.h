#pragma once
#include "vec3.h"
#include "intersection.h"
struct iIntersectable 
{
	virtual bool intersect(ray& r, intersection*& result);
};