#pragma once
#include "ray.h"
#include "vec4.h"
extern struct iIntersectable;

struct intersection 
{
	fp intersectionDistance;//the distance the light has to travel to reach the intersection point
	vec3 intersectionColor;//the color of the intersection point
	fp intersectionOpacity;//the opacity of the intersection
	fp reflectivity;//the strength of the reflection
	//color equation: lerp(lerp(basecolor , reflectioncolor, reflectivity), behindcolor, intersectionOpacity)
	vec3 normal;//the normal of the edge the ray hit
	//TODO: the amount of angle a reflection can change its direction
	
	iIntersectable* intersectedWith;
	intersection(iIntersectable* intersectedWith, vec3 normal, fp intersectionOpacity, vec3 intersectionColor, fp intersectionDistance):
		intersectedWith(intersectedWith),normal(normal),intersectionOpacity(intersectionOpacity),intersectionColor(intersectionColor),intersectionDistance(intersectionDistance){}
};

