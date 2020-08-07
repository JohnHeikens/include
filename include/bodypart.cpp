#include "bodypart.h"

bodypart::bodypart(bodypart* parent, vec3 translate, vec3 scale, vec3 rotationcentre, std::initializer_list<rotation> rotations, std::initializer_list<bodypart*> childs)
{
	this->rotations = rotations;
	this->rotationcentre = rotationcentre;
	this->translate = translate;
	this->parent = parent;
	this->scale = scale;
	CalculateTransform();
	scalecentre = mat4x4::cross(mat4x4::translate3d(-rotationcentre), mat4x4::scale3d(scale));
	this->childs = std::vector<bodypart*>(childs);
}

void bodypart::CalculateTransform()
{
	changed = false;
	std::vector<mat4x4> transforms = std::vector<mat4x4>();
	for (rotation r : rotations)
	{
		transforms.push_back(mat4x4::rotate3d(r.axis, r.angle));
	}
	transforms.push_back(mat4x4::translate3d(translate));
	applied = mat4x4::combine(transforms);
}

void bodypart::Draw(graphicsObject* obj, mat4x4 transform, const color* fill, const vec3 camera, const vec3 direction)
{
	bufferobject<color> c = bufferobject<color>(fill, 1, 0, 12);
	if (changed)CalculateTransform();
	mat4x4 view = mat4x4::cross(transform, applied);
	obj->DrawTrianglesPlain(&cubevb0, &c, &cubeibo, camera, mat4x4::cross(view, scalecentre), direction);
	for (bodypart* b : childs)
	{
		b->Draw(obj, view, fill, camera, direction);
	}
}
