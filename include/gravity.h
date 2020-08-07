#pragma once
#include "GlobalFunctions.h"

//calculates speed per second for object 1 to object 2 with mass 1
// distance is squared
inline fp calculateGravity1Outside(cfp distancesquared, cfp gravityConstant)
{
	return gravityConstant / distancesquared;
}
inline fp calculateGravityMassOutside(cfp distanceSquared, cfp m2, cfp gravityConstant)
{
	return calculateGravity1Outside(distanceSquared, gravityConstant) * m2;
}
//calculates speed per second for object 1 inside object 2 with mass 1
//distance is not squared
inline fp calculateGravity1Inside(cfp distance, cfp gravityConstant, cfp radius3)
{
	return (gravityConstant * distance) / radius3;
}
inline fp calculateGravityMassInside(cfp distance, cfp gravityConstant, cfp radius3, cfp m2)
{
	return calculateGravity1Inside(distance, gravityConstant, radius3) * m2;
}

//calculates speed per second for object 1 to object 2 with mass 1
//distance is squared
inline fp calculateGravity1(cfp distanceSquared, cfp gravityConstant, cfp radius)
{
	cfp radius2 = radius * radius;
	return distanceSquared < radius ?
		//inside
		calculateGravity1Inside(sqrt(distanceSquared), gravityConstant, radius * radius2) :
		//outside
		calculateGravity1Outside(distanceSquared, gravityConstant);
}

inline fp calculateGravityMass(cfp distanceSquared, cfp m2, cfp gravityConstant, cfp radius)
{
	return calculateGravity1(distanceSquared, gravityConstant, radius) * m2;
}
//force between two objects
inline fp CalculateGravityForce(cfp distancesquared, cfp m1, cfp m2, cfp gravityConstant, cfp radius)
{
	return m1 * calculateGravityMass(distancesquared, m2, gravityConstant, radius);
}
//calculates acceleration for object 0
inline vec2 calculateAcceleration(vec2 pos0, vec2 pos1, cfp m1, cfp gravityConstant, cfp radius1)
{
	if (pos0 == pos1)
	{
		return vec2();
	}
	vec2 difference = pos1 - pos0;
	fp distanceSquared = difference.lengthsquared();
	//add some distance
	vec2 normal = difference / sqrt(distanceSquared);
	return calculateGravityMass(distanceSquared, m1, gravityConstant,radius1) * normal;
}
//distance is not squared
inline fp calculateOrbitalVelocity(cfp& distance, cfp m2, cfp gravityConstant)
{
	//https://www.softschools.com/formulas/physics/orbital_velocity_formula/76/
	return sqrt((m2 * gravityConstant) / distance);
}