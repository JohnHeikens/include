#include "rectangle2.h"
#include "graphics.h"
#pragma once
struct bodyPart2D
{
	rectangle2i textureRect;
	vec2 scale;//texture pixels to meters
	//the position of the joint relative to the rectangular box. mid = vec2(0.5) * scale
	vec2 rotationcentre;
	fp angle;
	//the constant translation between the joint of the parent and the current joint
	vec2 translate;
	mat3x3 applied;
	mat3x3 scalecentre;//scale + rotationcentre, only apply to this, calculate once
	bool changed;
	bodyPart2D* parent;
	std::vector<bodyPart2D*>* childs;
	bodyPart2D(crectangle2i& textureRect,bodyPart2D* parent = NULL, vec2 translate = vec2(), vec2 scale = vec2(1), vec2 rotationcentre = vec2(0.5), std::initializer_list<bodyPart2D*> childs = {}, cfp& angle = 0);
	void CalculateTransform();
	void Draw(const graphicsObject& obj, mat3x3 transform, const Texture* tex);
};