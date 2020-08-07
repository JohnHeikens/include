#pragma once
#include "intersectable.h"

struct rayCaster 
{
	ray currentRay;
	vec3 resultingColor = vec3();
	fp smallestOpaqueDistance = INFINITY;//the distance after which everything gets cut off
	std::list<intersection*> intersections = std::list<intersection*>();

	//constructor
	rayCaster(ray currentRay) :currentRay(currentRay) {}


	void test(iIntersectable* i)
	{
		intersection* newIntersection;
		if (i->intersect(currentRay, newIntersection)) 
		{
			if (newIntersection->intersectionDistance < smallestOpaqueDistance)//else it will be out of reach
			{
				if (newIntersection->intersectionOpacity = 1) 
				{
					smallestOpaqueDistance = newIntersection->intersectionDistance;
					//later we will remove the items behind this distance from the list
				}
				intersections.push_back(newIntersection);
			}
		}
	}
	static array2d<rayCaster*>* rayCastBatch(vec3 position, vec3 screenRectMiddle, vec3 screenRectMiddleLeft, vec3 screenRectTopMiddle, std::list<iIntersectable*>* intersectables, const graphicsObject* drawOn)
	{
		array2d<rayCaster*>* casters = new array2d<rayCaster*>(drawOn->width, drawOn->height, false);

		rayCaster** casterPtr = casters->basearray;
		color* colorPtr = drawOn->colors;
		for (int j = 0; j < drawOn->height; j++)
		{
			cfp jPart = (j / (fp)drawOn->height) * 2 - 1;
			vec3 screen3DPosJ = screenRectMiddle + screenRectTopMiddle * jPart;
			for (int i = 0; i < drawOn->width; i++, colorPtr++)
			{
				cfp iPart = (j / (fp)drawOn->width) * 2 - 1;
				vec3 pointOnScreen3D = screen3DPosJ + screenRectMiddleLeft * iPart;
				ray r = ray(position, (pointOnScreen3D - position));
				*casterPtr = new rayCaster(r);
				for (iIntersectable* currentIntersectable : *intersectables) 
				{
					(*casterPtr)->test(currentIntersectable);
				}
				
				*colorPtr = (*casterPtr)->intersections.size() > 0 ? colorPalette::white : colorPalette::black;
			}
		}
		return casters;
	}
};