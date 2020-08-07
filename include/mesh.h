#include "graphics.h"
#include "fastlist.h"
#pragma once

struct mesh 
{
	static bufferobject<uint>* GenerateIndiceBuffer(cuint size);
	void CalculateLightLevels();
	//the sun shines from +z to -z
	static vec3 CalculateLightlevelZ(cvec3& normal);

	void ApplyMatrix(mat4x4 mat);

	//for OBJ files
	mesh(std::wstring path);
	
	void save(std::wstring path);

	mesh(bufferobject<fp>* vertices, bufferobject<uint>* indices, bufferobject<fp>* textureCoordinates, Texture* tex, bufferobject<fp>* lightLevels = nullptr);
	mesh(bufferobject<fp>* vertices, bufferobject<uint>* indices, bufferobject<color>* colors, bufferobject<fp>* lightLevels);
	


	bufferobject<fp>* vertices = nullptr;//positions of points
	bufferobject<fp>* textureCoordinates = nullptr;//texture coordinates for each vertice
	bufferobject<fp>* lightLevels = nullptr;//the colors get multiplied by this
	bufferobject<uint>* indices = nullptr;//the indices that you want to draw
	bufferobject<color>* colors = nullptr;//if you want to draw in plain colors
	Texture* tex = nullptr;
	inline void Draw(const graphicsObject* graphics, const vec3& position, const mat4x4& view, const vec3& lookdirection) const
	{
		if (textureCoordinates && tex) 
		{
			if (lightLevels) 
			{
				graphics->DrawTrianglesTexLight(vertices, textureCoordinates, lightLevels, indices, position, view, *tex, lookdirection);
			}
			else 
			{
				graphics->DrawTrianglesTex(vertices, textureCoordinates, indices, position, view, *tex, lookdirection);
			}
		}
		else 
		{
			if (lightLevels)
			{
				graphics->DrawTrianglesPlainLight(vertices, colors, lightLevels, indices, position, view, lookdirection);
			}
			else 
			{
				graphics->DrawTrianglesPlain(vertices, colors, indices, position, view, lookdirection);
			}
		}
	}
};