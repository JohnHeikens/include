#include "meshConstants.h"
#pragma once
struct rotation
{
	inline rotation(vec3 axis = zup::up, fp angle = 0) :axis(axis), angle(angle) {}
	vec3 axis;
	fp angle;
};
struct bodypart
{
	vec3 scale;
	vec3 rotationcentre;//the position of the joint relative to the rectangular box. mid = scale * 0.5
	//vec3 rotationaxis;
	std::vector<rotation> rotations;
	//fp angle;
	vec3 translate;
	mat4x4 applied;
	mat4x4 scalecentre;//scale + rotationcentre, only apply to this, calculate once
	bool changed;
	bodypart* parent;
	std::vector<bodypart*> childs;
	bodypart(bodypart* parent = nullptr, vec3 translate = vec3(), vec3 scale = vec3(1), vec3 rotationcentre = vec3(0.5), std::initializer_list<rotation> rotations = {}, std::initializer_list<bodypart*> childs = {});
	void CalculateTransform();
	void Draw(graphicsObject* obj, mat4x4 transform, const color* fill, const vec3 camera, const vec3 direction);
};