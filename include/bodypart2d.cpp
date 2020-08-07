#include "bodypart2d.h"

bodyPart2D::bodyPart2D(crectangle2i& textureRect, bodyPart2D* parent, vec2 translate, vec2 scale, vec2 rotationcentre, std::initializer_list<bodyPart2D*> childs,cfp& angle)
{
	this->rotationcentre = rotationcentre;
	this->translate = translate;
	this->parent = parent;
	this->scale = scale;
	this->angle = angle;
	this->textureRect = textureRect;
	CalculateTransform();
	//first scale it up, then translate to the rotationcentre 
	scalecentre = mat3x3::cross(mat3x3::translate2d(-rotationcentre), mat3x3::scale2d(scale));
	this->childs = new std::vector<bodyPart2D*>(childs);
}

void bodyPart2D::CalculateTransform()
{
	changed = false;

	std::vector<mat3x3> transforms = std::vector<mat3x3>();
	if (angle != 0) 
	{
		transforms.push_back(mat3x3::rotate2d(angle));
	}
	transforms.push_back(mat3x3::translate2d(translate));
	applied = mat3x3::combine(transforms);
}

void bodyPart2D::Draw(const graphicsObject& obj, mat3x3 transform, const Texture* tex)
{
	if (changed)
		CalculateTransform();
	mat3x3 view = mat3x3::cross(transform, applied);
	obj.fillTexture(textureRect, mat3x3::cross(view, scalecentre), *tex);
	for (bodyPart2D* b : *childs)
	{
		b->Draw(obj, view, tex);
	}
}
