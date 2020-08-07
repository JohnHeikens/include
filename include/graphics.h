#pragma once
#include "mathfunctions.h"
#include "image.h"
#include "GlobalFunctions.h"
#include "textures.h"
#include "triangle.h"
#include "bufferobject.h"
#include "array2d.h"

namespace rendersettings {
	extern bool checkopacity;
	namespace s3d {
		extern fp mindistance;
		extern fp maxdistance;
		namespace backfaceculling 
		{
			extern bool enabled;
			extern bool clockwise;
		}
	}
}
//multiply a color by a light level
inline color operator *(const color& c, cvec3& light)
{
	if (rendersettings::checkopacity)
	{
		return color(c.a, (byte)(c.r * light.r), (byte)(c.g * light.g), (byte)(c.b * light.b));
	}
	else
	{
		return color((byte)(c.r * light.r), (byte)(c.g * light.g), (byte)(c.b * light.b));
	}
}
struct graphicsObject:public Texture
{
	//contains values between 0 and maxdistance
	fp* depthbuffer = NULL;

	
	virtual color getColor(const vec2& pos) const override;

	//begin
	graphicsObject();
	graphicsObject(cint& width, cint& height, color* colors, fp* depthbuffer);
	graphicsObject(cint& width, cint& height, bool generateDepthBuffer);
	static graphicsObject* FromImage(const Image& img);
	static graphicsObject* CopyObj(const graphicsObject& other);
	
	void createColorBuffer();
	void createDepthBuffer();

	//end
	void DeleteColors() const;
	void DeleteDepthBuffer() const;

	//set
	void fillTriangle3D(const fp& x0, const fp& y0, const fp& d0, const vec2& tex0, const fp& x1, const fp& y1, const fp& d1, const vec2& tex1, const fp& x2, const fp& y2, const fp& d2, const vec2& tex2, const Texture& c) const;
	void fillTriangle3DLight(const fp& x0, const fp& y0, const fp& d0, const vec2& tex0, const vec3& l0, const fp& x1, const fp& y1, const fp& d1, const vec2& tex1, const vec3& l1, const fp& x2, const fp& y2, const fp& d2, const vec2& tex2, const vec3& l2, const Texture& c) const;
	void fillTriangle3D(const fp& x0, const fp& y0, const fp& d0, const fp& x1, const fp& y1, const fp& d1, const fp& x2, const fp& y2, const fp& d2, const color& c) const;
	void fillTriangle3DLight(const fp& x0, const fp& y0, const fp& d0, const vec3& l0, const fp& x1, const fp& y1, const fp& d1, const vec3& l1, const fp& x2, const fp& y2, const fp& d2, const vec3& l2, const color& c) const;
	void fillTriangle3DOpacity(const fp& x0, const fp& y0, const fp& d0, const fp& x1, const fp& y1, const fp& d1, const fp& x2, const fp& y2, const fp& d2, const color& c) const;
	int Triangle_ClipAgainstScreen(const mat4x4& view, triangle& in_tri, triangle& out_tri0, triangle& out_tri1) const;
	void ClearDepthBuffer(cfp MaxDistance = rendersettings::s3d::maxdistance) const;
	void Fog(color FogColor, cfp& multiplier = 1.0 / rendersettings::s3d::maxdistance) const;
	void DrawTrianglesTex(const bufferobject<fp>* vertices, const bufferobject<fp>* texturecoords, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const Texture& tex, const vec3& lookdirection) const;
	void DrawTrianglesTexLight(const bufferobject<fp>* vertices, const bufferobject<fp>* texturecoords, const bufferobject<fp>* multiplylight, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const Texture& tex, const vec3& lookdirection) const;
	void DrawTrianglesPlain(const bufferobject<fp>* vertices, bufferobject<color>* tricolors, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const vec3& lookdirection) const;
	void DrawTrianglesPlainLight(const bufferobject<fp>* vertices, bufferobject<color>* tricolors, const bufferobject<fp>* multiplylight, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const vec3& lookdirection) const;
	void ClearColor(const color BackGroundColor) const;
	void drawRectangle(crectangle2i& rect, cint borderThickness, const color c) const;
	void fill(const color c) const;
	void fillRectangle(cfp x, cfp y, cfp w, cfp h, const color c) const;
	void fillRectangle(crectangle2& rect, const color c) const;
	void fillRectangle(rectangle2i rect, const color c) const;
	void fillRectangle(rectangle2i rect, const brush* b) const;
	void fillRectangleUnsafe(crectangle2i& rect, const color c) const;
	void fillRectangleUnsafe(crectangle2i& rect, const brush* b) const;
	void fillTexture(cvec2i& position, const Texture& tex) const;
	void fillTexture(crectangle2& drawRect, const Texture& tex) const;
	void fillTexture(crectangle2i& textureRect, mat3x3 transform, const Texture& tex) const;
	void fillTexture(crectangle2i& textureRect, crectangle2& drawRect, const Texture& tex) const;
	void fillTexture(crectangle2& drawRect, cint& getw, cint& geth, cint& texWidth, const color* texColors) const;
	void fillTextureCropped(crectangle2i& rect, cint texWidth, const color* texColors) const;
	void fillTexture(cint& getw, cint& geth, cint& texWidth, const mat3x3& transform, const color* texColors) const;
	void fillCircle(fp x, fp y, fp w, fp h, const brush& b) const;
	void fillCircleCentered(cvec2& pos, cvec2& size, const brush& b) const;
	void fillCircleCentered(fp x, fp y, fp w, fp h, const brush& b) const;
	void FloodFill(const POINT& p, const color& c) const;
	void DrawLine(const vec2& p0, const vec2& p1, const color& c) const;
	void DrawEllipse(int x, int y, int w, int h, color c) const;
	void DrawRotatedEllipse(cvec2 center, cvec2 size, fp rotation, const color c) const;
	void plotpoints(int xcenter, int ycenter, int x, int y, color c) const;
	void visualizeFormula(const rectangle2& screenRect, const rectangle2& spaceRect, fp(*func)(cfp& x), const brush& b);
	void Fade(const fp& weight, const color& fadeto) const;
	void Fade(fp multiplier) const;

	color GetPixel(cvec2i& pos) const;

	void SetDepth(vec2& pos, const fp& depth) const 
	{
		SetDepth((int)pos.x, (int)pos.y, depth);
	}
	void SetDepth(cint& x, cint& y, const fp& depth) const
	{
		if (InBounds(x, y))SetDepthUnsafe(x, y, depth);
	}

	void SetDepthUnsafe(cint& x, cint& y, const fp& depth) const
	{
		depthbuffer[x + y * width] = depth;
	}



	//fills pixels specified with the vertices to pixelcolors
	inline void fillPixels3D(const bufferobject<fp>* vertices, bufferobject<const color>* pixelcolors, const vec3& position, const mat4x4& view, const vec3& lookdirection) const
	{
		//ViewFrustum f = ViewFrustum();
		//f.update(view);
		vec3* const vertices2D = new vec3[vertices->size / 3];
		//cint Stride = 3;//no need for texture indicing or light leveling
		// vertices to 2D
		vec3* p2d = vertices2D;
		const fp* p3d = vertices->buffer;
		const fp* end = p3d + vertices->size;
		while (p3d < end)
		{
			*p2d++ = windowspace(view, *(vec3*)p3d);// - position);
			p3d += vertices->stride;
		}
		const color* colorPtr = pixelcolors->buffer;
		for (
			int i = 0;
			i < vertices->stepcount;
			i++, colorPtr += pixelcolors->stride
			)//iterate for each tri
		{
			// Get Values
			vec3* activepos = vertices2D + i;
			fillPixel3D(activepos->Get2d(), activepos->z, *colorPtr);

			//next:;//the next triangle will be drawn
		}
		delete[] vertices2D;
		//}
	}

	inline void fillPixel3D(vec2 pos, cfp depth, color color) const
	{
		fillPixel3D((int)pos.x, (int)pos.y, depth, color);
	}
	inline void fillPixel3D(cint x, cint y, cfp depth, color color) const
	{
		if (InBounds(x, y))fillPixel3DUnsafe(x, y, depth, color);
	}
	inline void fillPixel3DUnsafe(cint x, cint y, cfp depth, color color) const
	{
		cfp d = GetDepthUnsafe(x, y);//there are faster methods( calculating the position once)
		if (d > depth) 
		{
			SetDepthUnsafe(x, y, depth);
			fillPixelUnsafe(x, y, color);
		}
	}

	inline void fillPixel(vec2 pos, color color) const
	{
		fillPixel((int)pos.x, (int)pos.y, color);
	}
	inline void fillPixel(cint x, cint y, color color) const
	{
		if (x >= 0 && y >= 0 && x < this->width && y < this->height)fillPixelUnsafe(x, y, color);
	}
	inline void fillPixelUnsafe(cint& x, cint& y, const color& c) const
	{
		color* const ptr = colors + x + y * this->width;
		*ptr = c;
	}

	//get
	color GetPixel(cint x, cint y) const;
	color GetPixelUnSafe(cint x, cint y) const;
	fp GetDepth(cint& x, cint& y) const;
	fp GetDepthUnsafe(cint& x, cint& y) const;
	bool InBounds(cint x, cint y) const;
	inline vec2i Size() const 
	{
		return vec2i(width, height);
	}
	inline rectangle2i getClientRect() const
	{
		return rectangle2i(this->Size());
	}

	// Return points in window space
	//vec3(pixelx,pixely,distance)
	inline vec3 windowspace(const mat4x4& view, const vec3& in) const
	{
		vec3 out = view.multPointMatrix(in);
		out.x = (out.x * this->width + this->width) * .5;
		out.y = (-out.y * this->height + this->height) * .5;//-y for byte alignment ( inverted)
		return out;
	}
	
};

bool windedcorrect(const vec2 a, const vec2 b, const vec2 c);
