#include "graphics.h"

//ideas:
//https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1126/materials/cppdoc/graphics.html

//enable to stop drawing transparent pixels as black pixels
//disabled for speed
bool rendersettings::checkopacity = true;
bool rendersettings::s3d::backfaceculling::enabled = true;
bool rendersettings::s3d::backfaceculling::clockwise = false;
fp rendersettings::s3d::mindistance = 0.1;
fp rendersettings::s3d::maxdistance = 0x100;

//set the screen to the background color
void graphicsObject::ClearColor(const color BackGroundColor) const
{
	std::fill(colors, colors + this->width * this->height, BackGroundColor);
}

void graphicsObject::drawRectangle(crectangle2i& rect, cint borderThickness, const color c) const
{
	fillRectangle(crectangle2i(rect.x, rect.y, rect.w, borderThickness), c);
	fillRectangle(crectangle2i(rect.x, rect.y, borderThickness, rect.h), c);
	fillRectangle(crectangle2i(rect.x + rect.w - borderThickness, rect.y, borderThickness, rect.h), c);
	fillRectangle(crectangle2i(rect.x, rect.y + rect.h - borderThickness, rect.w, borderThickness), c);
}

//fill the screen with this color
void graphicsObject::fill(const color c) const
{
	if (c.a == 0xff || !rendersettings::checkopacity) 
	{
		ClearColor(c);
	}
	else
	{
		if (c.a == 0)return;
		fillRectangleUnsafe(getClientRect(), c);
	}
}

color graphicsObject::getColor(const vec2& pos) const
{
	cint w = width;
	return *(colors + //index of current level
		(rendersettings::multsize ?
		((int)((rendersettings::Remaindering ? math::Remainder1(pos.x) : pos.x) * w) + //x
			(int)((rendersettings::Remaindering ? math::Remainder1(pos.y) : pos.y) * height) * w) : //y
			((int)pos.x + ((int)pos.y) * w)
			)
		);
}

//a faster, safer method
void graphicsObject::fillCircle(fp x, fp y, fp w, fp h, const brush& b) const
{
	int MinX = (int)x;
	int MinY = (int)y;
	int MaxX = (int)(x + w);
	int MaxY = (int)(y + h);
	//crop
	if (MinX < 0)MinX = 0;
	if (MinY < 0)MinY = 0;
	if (MaxX > this->width)MaxX = this->width;
	if (MaxY > this->height)MaxY = this->height;

	fp midx = x + w * .5;
	fp midy = y + h * .5;
	fp multx = 1 / (midx - x);//multipliers
	fp multy = 1 / (midy - y);
	color* ptr = colors + MinX + MinY * this->width;
	for (int j = MinY; j < MaxY; j++)
	{
		color* NextPtr = ptr + this->width;
		fp dy = (j - midy) * multy;
		fp dy2 = dy * dy;
		for (int i = MinX; i < MaxX; i++)
		{
			fp dx = (i - midx) * multx;
			if (dy2 + dx * dx < 1)
			{
				*ptr = b.getColor(vec2(i, j));
			}
			ptr++;
		}
		ptr = NextPtr;
	}
}
void graphicsObject::fillCircleCentered(cvec2& pos, cvec2& size, const brush& b) const
{
	fillCircleCentered(pos.x, pos.y, size.x, size.y, b);
}
//x and y specify the center coordinates of the ellipse.
void graphicsObject::fillCircleCentered(fp x, fp y, fp w, fp h, const brush& b) const
{
	fillCircle(x - w * 0.5, y - h * 0.5, w, h, b);
}
//Flood - fill(node, target - color, replacement - color) :
//https://en.wikipedia.org/wiki/Flood_fill
void graphicsObject::FloodFill(const POINT& p, const color& c) const
{
	color targetcolor = GetPixel(p.x, p.y);
	//	1. If target - color is equal to replacement - color, return.
	if (targetcolor == c)return;
	//3. Set the color of node to replacement - color.
	this->fillPixel(p.x, p.y, c);
	//4. Set Q to the empty queue.
	std::list<POINT> Q = std::list<POINT>();
	//5. Add node to the end of Q.
	Q.push_back(p);
	//6. While Q is not empty:
	while (Q.size() > 0)
	{
		//7. Set n equal to the first element of Q.
		POINT n = *Q.begin();
		//8.     Remove first element from Q.
		Q.pop_front();
		//9.     If the color of the node to the west of n is target - color,
		if (n.x > 0 && GetPixel(n.x - 1, n.y) == targetcolor) {
			//set the color of that node to replacement - color and add that node to the end of Q.
			fillPixel(n.x - 1, n.y, c);
			Q.push_back(POINT{ n.x - 1,n.y });
		}
		//10.     If the color of the node to the east of n is target - color,
		if (n.x < width - 1 && GetPixel(n.x + 1, n.y) == targetcolor) {
			//set the color of that node to replacement - color and add that node to the end of Q.
			fillPixel(n.x + 1, n.y, c);
			Q.push_back(POINT{ n.x + 1,n.y });
		}
		//11.     If the color of the node to the north of n is target - color,
		if (n.y > 0 && GetPixel(n.x, n.y - 1) == targetcolor) {
			//set the color of that node to replacement - color and add that node to the end of Q.
			fillPixel(n.x, n.y - 1, c);
			Q.push_back(POINT{ n.x,n.y - 1 });
		}
		//12.     If the color of the node to the south of n is target - color,
		if (n.y < height - 1 && GetPixel(n.x, n.y + 1) == targetcolor) {
			//set the color of that node to replacement - color and add that node to the end of Q.
			fillPixel(n.x, n.y + 1, c);
			Q.push_back(POINT{ n.x ,n.y + 1 });
		}
		//13. Continue looping until Q is exhausted.
	}
	//14. Return.
}
//draw a line from p0 to p1
//source:
//http://blog.ruofeidu.com/bresenhams-line-algorithm-in-c/
//also worth checking out:
//https://www.redblobgames.com/grids/line-drawing.html
//https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
void graphicsObject::DrawLine(const vec2& p0, const vec2& p1, const color& c) const
{
	int x0 = (int)p0.x, y0 = (int)p0.y, x1 = (int)p1.x, y1 = (int)p1.y;
	int dx = abs(x1 - x0), dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;


	while (true)
	{
		fillPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) return;
		int e2 = (err << 1);
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}
void graphicsObject::DrawEllipse(int x, int y, int w, int h, color c) const
{
	int rx = w / 2;
	int ry = h / 2;
	int xs1 = x + rx;
	int ys1 = y + ry;
	int activex, activey;
	fp d1, d2 = 0, dx, dy;
	activex = 0;                  // take start position as (0,ry)
	activey = ry;                 // finding decision parameter d1
	d1 = pow(ry, 2) - (pow(rx, 2) * ry) + (0.25 * pow(rx, 2));
	dx = 2 * pow(ry, 2) * activex;
	dy = 2 * pow(rx, 2) * activey;
	do                         // region one
	{
		plotpoints(xs1, ys1, activex, activey,c);
		if (d1 < 0)
		{
			activex++;
			dx = dx + (2 * (pow(ry, 2)));
			d1 = d1 + dx + (pow(ry, 2));
		}
		else
		{
			activex++;
			activey--;
			dx = dx + (2 * (pow(ry, 2)));
			dy = dy - (2 * (pow(rx, 2)));
			d1 = d1 + dx - dy + (pow(ry, 2));
		}
	} while (dx < dy);      // change over condition for region-2
	do                       // region two
	{
		plotpoints(xs1, ys1, activex, activey,c);
		if (d2 > 0)
		{
			activex = activex;
			activey--;
			dy = dy - (2 * (pow(rx, 2)));
			d2 = d2 - dy + pow(rx, 2);
		}
		else
		{
			activex++;
			activey--;
			dy = dy - (2 * (pow(rx, 2)));
			dx = dx + (2 * (pow(ry, 2)));
			d2 = d2 + dx - dy + pow(rx, 2);

		}
	} while (activey > 0);
}

//source:
//https://www.geeksforgeeks.org/draw-an-ellipse-rotating-over-a-circle-in-c-graphics/
void graphicsObject::DrawRotatedEllipse(cvec2 center, cvec2 size, fp rotation, const color c) const
{
	int stepcount = (size.x + size.y) * math::PI;//the length the border in pixels
	stepcount * 20;//improve prexision
	float t = math::PI2 / stepcount;
	//rotation = stepcount - rotation;
	int theta;

	mat3x3 basetransform = mat3x3::combine({
		mat3x3::scale2d(size), //scale the ellipse
		mat3x3::rotate2d(rotation),//rotate the ellipse
		mat3x3::translate2d(center) //scale the ellipse
	});


	// Filling each pixel corresponding 
	// to every angle from 0 to 360 
	for (int i = 0; i < stepcount; i += 1) {
		theta = i;
		cfp partofcircle = t * theta;
		mat3x3 localtransform = mat3x3::rotate2d(partofcircle);
		vec2 point = localtransform.multPointMatrix(vec2(-1, 0));//begin at the top
		point = basetransform.multPointMatrix(point);
		fillPixel(point.x, point.y, c);
	}
}

void graphicsObject::plotpoints(int xcenter, int ycenter, int x, int y, color c) const
{
	fillPixel(xcenter + x, ycenter + y, c);
	fillPixel(xcenter - x, ycenter + y, c);
	fillPixel(xcenter + x, ycenter - y, c);
	fillPixel(xcenter - x, ycenter - y, c);
}
void graphicsObject::visualizeFormula(const rectangle2& screenRect, const rectangle2& spaceRect, fp(*func)(cfp& x), const brush& b)
{
	mat3x3 spaceToScreen = mat3x3::fromRectToRect(spaceRect, screenRect);
	fp dotDistance = 0.01;
	for (fp spaceX = fastceil(spaceRect.pos00.x); spaceX < spaceRect.pos11().x; spaceX += dotDistance)
	{
		cvec2 pos = spaceToScreen.multPointMatrix(vec2(spaceX, func(spaceX)));
		fillCircleCentered(pos, vec2(4), b);
	}
}
void graphicsObject::Fade(const fp& weight, const color& fadeto) const
{
	cint size = width * height;
	color* const end = colors + size;
	for (color* c = colors; c < end; c++)
	{
		*c = color::lerpcolor(fadeto, *c, weight);
	}
}
//fade to transparency
void graphicsObject::Fade(fp multiplier) const
{
	int size = width * height;
	color* end = colors + size;
	for (color* c = colors; c < end; c++) 
	{
		*c = *c * multiplier;
	}
}

color graphicsObject::GetPixel(cvec2i& pos) const
{
	return GetPixel(pos.x,pos.y);
}

color graphicsObject::GetPixel(cint x, cint y) const
{
	if (InBounds(x, y)) 
	{
		return *(colors + x + y * width);
	}
	else 
	{
		return color();
	}
}
color graphicsObject::GetPixelUnSafe(cint x, cint y) const
{
	return *(colors + x + y * width);
}
fp graphicsObject::GetDepth(cint& x, cint& y) const
{
	return InBounds(x, y) ? GetDepthUnsafe(x, y) : -1;
}
fp graphicsObject::GetDepthUnsafe(cint& x, cint& y) const
{
	return depthbuffer[x + y * width];
}
bool graphicsObject::InBounds(cint x, cint y) const
{
	return x >= 0 && y >= 0 && x < width && y < height;
}
//fill the depthbuffer with the maximum distance where triangles are allowed to be drawn.
//this will be the maximum distance in 'rendersettings' by default.
//https://en.wikipedia.org/wiki/Z-buffering
void graphicsObject::ClearDepthBuffer(cfp MaxDistance) const
{
	std::fill(depthbuffer, depthbuffer + this->width * this->height, MaxDistance);
}


void graphicsObject::Fog(color FogColor, cfp& multiplier) const
{
	color* ptr = colors;
	color* endptr = colors + this->width * this->height;
	fp* DepthPtr = depthbuffer;
	while (ptr < endptr)
	{
		*ptr++ = color::lerpcolor(*ptr, FogColor, *DepthPtr * multiplier);
		DepthPtr++;
	}
}
//switch points so y0 <= y1 <= y2
inline void switchy(int (&switchindexes)[3], const fp (&screeny)[3])
{
	bool y10 = screeny[0] > screeny[1];//y0 more than y1? then y1 is higher than y0
	bool y21 = screeny[1] > screeny[2];//y1 more than y2? then y2 is higher than y1
	bool y20 = screeny[0] > screeny[2];//y0 more than y2? then y2 is higher than y0
	if (y10)
	{
		if (y21)//total opposite
		{//210
			switchindexes[0] = 2;
			switchindexes[1] = 1;
			switchindexes[2] = 0;
		}
		else
		{
			if (y20)
			{//120
				switchindexes[0] = 1;
				switchindexes[1] = 2;
				switchindexes[2] = 0;
			}
			else
			{//102
				switchindexes[0] = 1;
				switchindexes[1] = 0;
				switchindexes[2] = 2;
			}
		}
	}
	else if (y21)
	{
		if (y20)
		{//201
			switchindexes[0] = 2;
			switchindexes[1] = 0;
			switchindexes[2] = 1;
		}
		else
		{//021
			switchindexes[0] = 0;
			switchindexes[1] = 2;
			switchindexes[2] = 1;
		}
	}
	else
	{//012
		switchindexes[0] = 0;
		switchindexes[1] = 1;
		switchindexes[2] = 2;
	}
}

//y is greater at the top
bool windedcorrect(const vec2 a, const vec2 b, const vec2 c) 
{
	//counter-clockwise
	const vec2 dab = b - a;
	const vec2 dac = c - a;
	const fp winding = dab.x * dac.y - dab.y * dac.x;
	return rendersettings::s3d::backfaceculling::clockwise ? winding < 0: winding > 0;
}
//with textures
//void graphicsObject::DrawTrianglesTex(fp* vertices, uint* indices, const uint& TriangleCount, const uint& VerticeCount, const vec3& position, mat4x4& view,const Texture& tex, const vec3& lookdirection) const
//vertices:
//stepcount:the amount of vectors
//stride:the stride of a vector
//
void graphicsObject::DrawTrianglesTex(const bufferobject<fp>* vertices, const bufferobject<fp>* texturecoords, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const Texture& tex, const vec3& lookdirection) const
{
	vec3* const vertices2D = new vec3[vertices->stepcount];
	//cint Stride = 3;//no need for texture indicing or light leveling
	// vertices to 2D
	vec3* p2d = vertices2D;
	const fp* p3d = vertices->buffer;
	const fp* end = p3d + vertices->stride * vertices->stepcount;
	while (p3d < end)
	{
		*p2d++ = windowspace(view, *(vec3*)p3d);// - position);
		p3d += vertices->stride;
	}
	const uint* indPtr = indices->buffer;
	for (
		int i = 0;
		i < indices->stepcount;
		i++, indPtr += indices->stride
		)//iterate for each tri
	{
		// Get Values
		cint //get indices(they will be used multiple times)
			indicevert0 = *indPtr,
			indicevert1 = *(indPtr + 1),
			indicevert2 = *(indPtr + 2);
		triangle tris[2];//max count of clipped triangles is 2
		//set first triangle
		tris[0].p[0] = *(vec3*)(vertices->buffer + indicevert0 * vertices->stride);
		tris[0].p[1] = *(vec3*)(vertices->buffer + indicevert1 * vertices->stride);
		tris[0].p[2] = *(vec3*)(vertices->buffer + indicevert2 * vertices->stride);
		tris[0].t[0] = *(vec2*)(texturecoords->buffer + indicevert0 * texturecoords->stride);
		tris[0].t[1] = *(vec2*)(texturecoords->buffer + indicevert1 * texturecoords->stride);
		tris[0].t[2] = *(vec2*)(texturecoords->buffer + indicevert2 * texturecoords->stride);
		tris[0].screen[0] = *(vertices2D + indicevert0);
		tris[0].screen[1] = *(vertices2D + indicevert1);
		tris[0].screen[2] = *(vertices2D + indicevert2);
		triangle t = tris[0];//copy
		int ClippedTriangleCount = Triangle_ClipAgainstScreen(view, t, tris[0], tris[1]);

		//clippedtrianglecount can be zero, it will just skip this section.
		for (int ClippedTriangleIndex = 0; ClippedTriangleIndex < ClippedTriangleCount; ClippedTriangleIndex++)
		{
			const triangle* activetri = (&tris[ClippedTriangleIndex]);
			const fp screenx[3] = { activetri->screen[0].x, activetri->screen[1].x, activetri->screen[2].x };//screen coordinates
			const fp screeny[3] = { activetri->screen[0].y, activetri->screen[1].y, activetri->screen[2].y };
			const fp distance[3] = { activetri->screen[0].z, activetri->screen[1].z, activetri->screen[2].z };
			// Calculate triangle screen bounds
			fp minx, maxx;
			if (screenx[0] < screenx[1])
			{
				minx = min(screenx[0], screenx[2]);
				maxx = max(screenx[1], screenx[2]);
			}
			else
			{
				minx = min(screenx[1], screenx[2]);
				maxx = max(screenx[0], screenx[2]);
			}
			//if the triangle is less than a pixel broad or out of reach then dont draw it.
			if ((int)minx == (int)maxx || minx > this->width || maxx < 0) continue;
			//https://stackoverflow.com/questions/9120032/determine-winding-of-a-2d-triangles-after-triangulation
			if (rendersettings::s3d::backfaceculling::enabled)
			{
				if (!windedcorrect(activetri->screen[0].Get2d(), activetri->screen[1].Get2d(), activetri->screen[2].Get2d()))
				{
					continue;
				}
			}
			int switchind[3];
			switchy(switchind, screeny);
			//if the triangle is less than a pixel heigh or is out of reach then dont fill it.
			if ((int)screeny[switchind[0]] == (int)screeny[switchind[2]] || screeny[switchind[0]] > this->height || screeny[switchind[2]] < 0) continue;

			fillTriangle3D(screenx[switchind[0]], screeny[switchind[0]], distance[switchind[0]],  activetri->t[switchind[0]], screenx[switchind[1]], screeny[switchind[1]], distance[switchind[1]], activetri->t[switchind[1]], screenx[switchind[2]], screeny[switchind[2]], distance[switchind[2]], activetri->t[switchind[2]], tex);
		}
	next:;//the next triangle will be drawn
	}
	delete[] vertices2D;
	//}
}
//with textures and light levels
//void graphicsObject::DrawTrianglesTex(fp* vertices, uint* indices, const uint& TriangleCount, const uint& VerticeCount, const vec3& position, mat4x4& view,const Texture& tex, const vec3& lookdirection) const
//vertices:
//stepcount:the amount of vectors
//stride:the stride of a vector
//
void graphicsObject::DrawTrianglesTexLight(const bufferobject<fp>* vertices, const bufferobject<fp>* texturecoords, const bufferobject<fp>* multiplylight, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const Texture& tex, const vec3& lookdirection) const
{
	vec3* const vertices2D = new vec3[vertices->stepcount];
	//cint Stride = 3;//no need for texture indicing or light leveling
	// vertices to 2D
	vec3* p2d = vertices2D;
	const fp* p3d = vertices->buffer;
	const fp* end = p3d + vertices->stride * vertices->stepcount;
	while (p3d < end)
	{
		*p2d++ = windowspace(view, *(vec3*)p3d);// - position);
		p3d += vertices->stride;
	}
	cuint* indPtr = indices->buffer;
	cfp* lightPtr = multiplylight->buffer;
	for (
		int i = 0;
		i < indices->stepcount;
		i++, indPtr += indices->stride, lightPtr += multiplylight->stride
		)//iterate for each tri
	{
		// Get Values
		cuint //get indices(they will be used multiple times)
			indicevert0 = *indPtr,
			indicevert1 = *(indPtr + 1),
			indicevert2 = *(indPtr + 2);
		triangle tris[2];//max count of clipped triangles is 2
		//set first triangle
		tris[0].p[0] = *(vec3*)(vertices->buffer + indicevert0 * vertices->stride);
		tris[0].p[1] = *(vec3*)(vertices->buffer + indicevert1 * vertices->stride);
		tris[0].p[2] = *(vec3*)(vertices->buffer + indicevert2 * vertices->stride);
		tris[0].t[0] = *(vec2*)(texturecoords->buffer + indicevert0 * texturecoords->stride);
		tris[0].t[1] = *(vec2*)(texturecoords->buffer + indicevert1 * texturecoords->stride);
		tris[0].t[2] = *(vec2*)(texturecoords->buffer + indicevert2 * texturecoords->stride);
		tris[0].light[0] = *(vec3*)(lightPtr);
		tris[0].light[1] = *(vec3*)(lightPtr + 3);
		tris[0].light[2] = *(vec3*)(lightPtr + 6);
		tris[0].screen[0] = *(vertices2D + indicevert0);
		tris[0].screen[1] = *(vertices2D + indicevert1);
		tris[0].screen[2] = *(vertices2D + indicevert2);
		triangle t = tris[0];//copy
		int ClippedTriangleCount = Triangle_ClipAgainstScreen(view, t, tris[0], tris[1]);

		//clippedtrianglecount can be zero, it will just skip this section.
		for (int ClippedTriangleIndex = 0; ClippedTriangleIndex < ClippedTriangleCount; ClippedTriangleIndex++)
		{
			const triangle* activetri = (&tris[ClippedTriangleIndex]);
			const fp screenx[3] = { activetri->screen[0].x, activetri->screen[1].x, activetri->screen[2].x };//screen coordinates
			const fp screeny[3] = { activetri->screen[0].y, activetri->screen[1].y, activetri->screen[2].y };
			const fp distance[3] = { activetri->screen[0].z, activetri->screen[1].z, activetri->screen[2].z };
			// Calculate triangle screen bounds
			fp minx, maxx;
			if (screenx[0] < screenx[1])
			{
				minx = min(screenx[0], screenx[2]);
				maxx = max(screenx[1], screenx[2]);
			}
			else
			{
				minx = min(screenx[1], screenx[2]);
				maxx = max(screenx[0], screenx[2]);
			}
			//if the triangle is less than a pixel broad or out of reach then dont draw it.
			if ((int)minx == (int)maxx || minx > this->width || maxx < 0) continue;
			//https://stackoverflow.com/questions/9120032/determine-winding-of-a-2d-triangles-after-triangulation
			if (rendersettings::s3d::backfaceculling::enabled)
			{
				if (!windedcorrect(activetri->screen[0].Get2d(), activetri->screen[1].Get2d(), activetri->screen[2].Get2d()))
				{
					continue;
				}
			}
			int switchind[3];
			switchy(switchind, screeny);
			//if the triangle is less than a pixel heigh or is out of reach then dont fill it.
			if ((int)screeny[switchind[0]] == (int)screeny[switchind[2]] || screeny[switchind[0]] > this->height || screeny[switchind[2]] < 0) continue;

			fillTriangle3DLight(
				screenx[switchind[0]], screeny[switchind[0]], distance[switchind[0]], activetri->t[switchind[0]],activetri->light[switchind[0]], 
				screenx[switchind[1]], screeny[switchind[1]], distance[switchind[1]], activetri->t[switchind[1]], activetri->light[switchind[1]], 
				screenx[switchind[2]], screeny[switchind[2]], distance[switchind[2]], activetri->t[switchind[2]],activetri->light[switchind[2]], tex);
		}
	next:;//the next triangle will be drawn
	}
	delete[] vertices2D;
}
//without textures
void graphicsObject::DrawTrianglesPlain(const bufferobject<fp>* vertices, bufferobject<color>* tricolors, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const vec3& lookdirection) const
{
	vec3* const vertices2D = new vec3[vertices->stepcount];
	//cint Stride = 3;//no need for texture indicing or light leveling
	// vertices to 2D
	vec3* p2d = vertices2D;
	const fp* p3d = vertices->buffer;
	const fp* end = p3d + vertices->stride * vertices->stepcount;
	while (p3d < end)
	{
		*p2d++ = windowspace(view, *(vec3*)p3d);// - position);
		p3d += vertices->stride;
	}
	const uint* indPtr = indices->buffer;
	const color* colorPtr = tricolors->buffer;
	for (
		int i = 0;
		i<indices->stepcount;
		i++,indPtr += indices->stride, colorPtr+=tricolors->stride
		)//iterate for each tri
	{
		// Get Values
		cint //get indices(they will be used multiple times)
			indicevert0 = *indPtr,
			indicevert1 = *(indPtr + 1),
			indicevert2 = *(indPtr + 2);
		triangle tris[2];//max count of clipped triangles is 2
		//set first triangle
		tris[0].p[0] = *(vec3*)(vertices->buffer + indicevert0 * vertices->stride);
		tris[0].p[1] = *(vec3*)(vertices->buffer + indicevert1 * vertices->stride);
		tris[0].p[2] = *(vec3*)(vertices->buffer + indicevert2 * vertices->stride);
		tris[0].screen[0] = *(vertices2D + indicevert0);
		tris[0].screen[1] = *(vertices2D + indicevert1);
		tris[0].screen[2] = *(vertices2D + indicevert2);
		triangle t = tris[0];//copy
		int ClippedTriangleCount = Triangle_ClipAgainstScreen(view, t, tris[0], tris[1]);
			
		//clippedtrianglecount can be zero, it will just skip this section.
		for (int ClippedTriangleIndex = 0; ClippedTriangleIndex < ClippedTriangleCount; ClippedTriangleIndex++)
		{
			const triangle* activetri = (&tris[ClippedTriangleIndex]);
			const fp screenx[3] = { activetri->screen[0].x, activetri->screen[1].x, activetri->screen[2].x };//screen coordinates
			const fp screeny[3] = { activetri->screen[0].y, activetri->screen[1].y, activetri->screen[2].y };
			const fp distance[3] = { activetri->screen[0].z, activetri->screen[1].z, activetri->screen[2].z };
			// Calculate triangle screen bounds
			fp minx, maxx;
			if (screenx[0] < screenx[1])
			{
				minx = min(screenx[0],screenx[2]);
				maxx = max(screenx[1],screenx[2]);
			}
			else
			{
				minx = min(screenx[1], screenx[2]);
				maxx = max(screenx[0], screenx[2]);
			}
			//if the triangle is less than a pixel broad or out of reach then dont draw it.
			if ((int)minx == (int)maxx || minx > this->width || maxx < 0) continue;
			//https://stackoverflow.com/questions/9120032/determine-winding-of-a-2d-triangles-after-triangulation
			if (rendersettings::s3d::backfaceculling::enabled)
			{
				if (!windedcorrect(activetri->screen[0].Get2d(), activetri->screen[1].Get2d(), activetri->screen[2].Get2d()))
				{
					continue;
				}
			}
			int switchind[3];
			switchy(switchind,screeny);
			//if the triangle is less than a pixel heigh or is out of reach then dont fill it.
			if ((int)screeny[switchind[0]] == (int)screeny[switchind[2]] || screeny[switchind[0]] > this->height || screeny[switchind[2]] < 0) continue;

			fillTriangle3D(screenx[switchind[0]], screeny[switchind[0]], distance[switchind[0]], screenx[switchind[1]], screeny[switchind[1]], distance[switchind[1]], screenx[switchind[2]], screeny[switchind[2]], distance[switchind[2]], *colorPtr);
		}
	next:;//the next triangle will be drawn
	}
	delete[] vertices2D;
	//}
}

//tricolors stride: 9 -> 3 colors per triangle and 3 channels per color
void graphicsObject::DrawTrianglesPlainLight(const bufferobject<fp>* vertices, bufferobject<color>* tricolors, const bufferobject<fp>* multiplylight, const bufferobject<uint>* indices, const vec3& position, const mat4x4& view, const vec3& lookdirection) const
{
	vec3* const vertices2D = new vec3[vertices->stepcount];
	//cint Stride = 3;//no need for texture indicing or light leveling
	// vertices to 2D
	vec3* p2d = vertices2D;
	const fp* p3d = vertices->buffer;
	const fp* end = p3d + vertices->stride * vertices->stepcount;
	while (p3d < end)
	{
		*p2d++ = windowspace(view, *(vec3*)p3d);// - position);
		p3d += vertices->stride;
	}
	const uint* indPtr = indices->buffer;
	const color* colorPtr = tricolors->buffer;
	const fp* lightPtr = multiplylight->buffer;
	for (
		int i = 0;
		i < indices->stepcount;
		i++, indPtr += indices->stride, colorPtr += tricolors->stride, lightPtr += multiplylight->stride
		)//iterate for each tri
	{
		// Get Values
		cint //get indices(they will be used multiple times)
			indicevert0 = *indPtr,
			indicevert1 = *(indPtr + 1),
			indicevert2 = *(indPtr + 2);
		triangle tris[2];//max count of clipped triangles is 2
		//set first triangle
		tris[0].p[0] = *(vec3*)(vertices->buffer + indicevert0 * vertices->stride);
		tris[0].p[1] = *(vec3*)(vertices->buffer + indicevert1 * vertices->stride);
		tris[0].p[2] = *(vec3*)(vertices->buffer + indicevert2 * vertices->stride);
		tris[0].light[0] = *(vec3*)(lightPtr);
		tris[0].light[1] = *(vec3*)(lightPtr + 3);
		tris[0].light[2] = *(vec3*)(lightPtr + 6);
		tris[0].screen[0] = *(vertices2D + indicevert0);
		tris[0].screen[1] = *(vertices2D + indicevert1);
		tris[0].screen[2] = *(vertices2D + indicevert2);
		triangle t = tris[0];//copy
		int ClippedTriangleCount = Triangle_ClipAgainstScreen(view, t, tris[0], tris[1]);

		//clippedtrianglecount can be zero, it will just skip this section.
		for (int ClippedTriangleIndex = 0; ClippedTriangleIndex < ClippedTriangleCount; ClippedTriangleIndex++)
		{
			const triangle* activetri = (&tris[ClippedTriangleIndex]);
			const fp screenx[3] = { activetri->screen[0].x, activetri->screen[1].x, activetri->screen[2].x };//screen coordinates
			const fp screeny[3] = { activetri->screen[0].y, activetri->screen[1].y, activetri->screen[2].y };
			const fp distance[3] = { activetri->screen[0].z, activetri->screen[1].z, activetri->screen[2].z };

			// Calculate triangle screen bounds
			fp minx, maxx;
			if (screenx[0] < screenx[1])
			{
				minx = min(screenx[0], screenx[2]);
				maxx = max(screenx[1], screenx[2]);
			}
			else
			{
				minx = min(screenx[1], screenx[2]);
				maxx = max(screenx[0], screenx[2]);
			}
			//if the triangle is less than a pixel broad or out of reach then dont draw it.
			if ((int)minx == (int)maxx || minx > this->width || maxx < 0) continue;
			//https://stackoverflow.com/questions/9120032/determine-winding-of-a-2d-triangles-after-triangulation
			if (rendersettings::s3d::backfaceculling::enabled)
			{
				if (!windedcorrect(activetri->screen[0].Get2d(), activetri->screen[1].Get2d(), activetri->screen[2].Get2d()))
				{
					continue;
				}
			}
			int switchind[3];
			switchy(switchind, screeny);
			//if the triangle is less than a pixel heigh or is out of reach then dont fill it.
			if ((int)screeny[switchind[0]] == (int)screeny[switchind[2]] || screeny[switchind[0]] > this->height || screeny[switchind[2]] < 0) continue;

			fillTriangle3DLight(
				screenx[switchind[0]], screeny[switchind[0]], distance[switchind[0]], activetri->light[switchind[0]],
				screenx[switchind[1]], screeny[switchind[1]], distance[switchind[1]], activetri->light[switchind[1]],
				screenx[switchind[2]], screeny[switchind[2]], distance[switchind[2]], activetri->light[switchind[2]],
				*colorPtr);
		}
	next:;//the next triangle will be drawn
	}
	delete[] vertices2D;
	//}
}

void graphicsObject::fillRectangle(cfp x, cfp y, cfp w, cfp h, const color c) const
{
	int ix = (int)x;
	int iy = (int)y;
	fillRectangle(ix, iy, (int)(x + w) - ix, (int)(y + w) - iy, c);
}
void graphicsObject::fillRectangle(crectangle2& rect, const color c) const
{
	fillRectangle(floorRect(rect), c);
}
void graphicsObject::fillRectangle(rectangle2i rect, const color c) const
{
	rect.crop(getClientRect());
	fillRectangleUnsafe(rect, c);
}
void graphicsObject::fillRectangle(rectangle2i rect, const brush* b) const
{
	rect.crop(getClientRect());
	fillRectangleUnsafe(rect, b);
}
void graphicsObject::fillRectangleUnsafe(crectangle2i& rect, const brush* b) const
{
	color* ptr = colors + rect.x + rect.y * width;
	color* EndPtrY = ptr + rect.h * width;
	vec2 pos;
	pos.y = rect.y;
	while (ptr < EndPtrY)
	{
		pos.x = rect.x;
		color* NextY = ptr + width;
		color* EndPtrX = ptr + rect.w;
		while (ptr < EndPtrX)
		{
			*ptr = b->getColor(pos);
			ptr++;
			pos.x++;
		}
		pos.y++;
		ptr = NextY;//ptr+=width of screen
	}
}
void graphicsObject::fillRectangleUnsafe(crectangle2i& rect, const color c) const
{
	color* ptr = colors + rect.x + rect.y * width;
	color* EndPtrY = ptr + rect.h * width;
	while (ptr < EndPtrY)
	{
		color* NextY = ptr + width;
		color* EndPtrX = ptr + rect.w;
		while (ptr < EndPtrX)
		{
			*ptr = c;
			ptr++;
		}

		ptr = NextY;//ptr+=width of screen
	}
}
void graphicsObject::fillTexture(cvec2i& position, const Texture& tex) const
{
	//crop rectangle
	rectangle2i rect = rectangle2i(position.x, position.y, tex.width, tex.height);
	rect.crop(getClientRect());
	fillTextureCropped(rect, tex.width, tex.colors);
}
//fill a scaled image
void graphicsObject::fillTexture(crectangle2& rect, const Texture& tex) const
{
	fillTexture(rect, tex.width, tex.height, tex.width, tex.colors);
}
void graphicsObject::fillTexture(crectangle2i& textureRect, mat3x3 transform, const Texture& tex) const
{
	fillTexture(textureRect.w, textureRect.h, tex.width, transform, tex.colors + textureRect.x + textureRect.y * tex.width);
}
void graphicsObject::fillTexture(crectangle2i& textureRect, crectangle2& drawRect, const Texture& tex) const
{
	fillTexture(drawRect, textureRect.w, textureRect.h, tex.width, tex.colors + textureRect.x + textureRect.y * tex.width);
}
//fill a part of a scaled image
void graphicsObject::fillTexture(crectangle2& rect, cint& getw, cint& geth, cint& texWidth, const color* texColors) const
{
	mat3x3 transform = mat3x3::cross(
		mat3x3::translate2d(vec2(rect.x, rect.y)),
		mat3x3::mult2d(vec2((fp)rect.w / getw, (fp)rect.h / geth))
	);
	fillTexture(getw, geth, texWidth, transform, texColors);
}
void graphicsObject::fillTextureCropped(crectangle2i& rect, cint imgwidth, const color* img) const
{
	color* ptr = colors + rect.x + rect.y * width;
	color* EndPtrY = ptr + rect.h * width;
	const color* imgptr = img;
	while (ptr < EndPtrY)
	{
		color* NextY = ptr + width;
		const color* imgNextY = imgptr + imgwidth;
		color* EndPtrX = ptr + rect.w;
		while (ptr < EndPtrX)
		{
			if (!rendersettings::checkopacity || imgptr->a > 0)
			{
				*ptr = *imgptr;
			}
			ptr++;
			imgptr++;
		}
		imgptr = imgNextY;
		ptr = NextY;//ptr+=width of screen
	}
}

//advanced images
void graphicsObject::fillTexture(cint& getw, cint& geth, cint& texWidth, const mat3x3& transform, const color* texColors) const
{
	const vec2 pos00 = transform.multPointMatrix(vec2(0, 0));//upper left corner
	const vec2 pos10 = transform.multPointMatrix(vec2(getw, 0));
	const vec2 pos01 = transform.multPointMatrix(vec2(0, geth));
	const vec2 pos11 = transform.multPointMatrix(vec2(getw, geth));
	int minX = (int)max(min(min(pos00.x, pos10.x), min(pos01.x, pos11.x)), 0),
		maxX = (int)min(max(max(pos00.x, pos10.x), max(pos01.x, pos11.x)), width - 1),
		minY = (int)max(min(min(pos00.y, pos10.y), min(pos01.y, pos11.y)), 0),
		maxY = (int)min(max(max(pos00.y, pos10.y), max(pos01.y, pos11.y)), height - 1);
	int dMinMaxX = maxX - minX;
	int dMinMaxY = maxY - minY;
	mat3x3 inverse = transform.Inverse();
	vec2 dx = vec2(inverse.mxX, inverse.mxY);
	vec2 dy = vec2(inverse.myX, inverse.myY);
	vec2 posj = inverse.multPointMatrix(vec2(minX, minY));
	color* colorptr = colors + minX + minY * width;
	for (int j = minY; j < maxY; j++,posj += dy)
	{
		vec2 posi = posj;
		color* nextptr = colorptr + width;
		
		for (color* endptr = colorptr + dMinMaxX;colorptr < endptr; posi += dx,colorptr++)
		{
			if (posi.x >= 0 && posi.x < getw && posi.y >= 0 && posi.y < geth) 
			{
				color c = *(texColors + (int)posi.x + ((int)posi.y) * texWidth);
				if (!rendersettings::checkopacity || c.a > 0)
				{
					*colorptr = c;
				}
			}
		}
		colorptr = nextptr;
	}
}

//y1 < y2 < y3
void fillTriangle(fp x1, fp y1, fp x2, fp y2, fp x3, fp y3, color c)
{



}

graphicsObject::graphicsObject()
{
}

graphicsObject::graphicsObject(cint& width, cint& height, color* colors, fp* depthbuffer)
{
	this->width = width;
	this->height = height;
	this->colors = colors;
	this->depthbuffer = depthbuffer;
}
graphicsObject::graphicsObject(cint& width, cint& height, bool generateDepthBuffer)
{
	this->width = width;
	this->height = height;
	createColorBuffer();
	if (generateDepthBuffer) {
		createDepthBuffer();
	}
	else 
	{
		depthbuffer = nullptr;
	}
}

graphicsObject* graphicsObject::FromImage(const Image& img)
{
	return new graphicsObject(img.width, img.height, img.colors, nullptr);
}

//copy colors and depthbuffer
graphicsObject* graphicsObject::CopyObj(const graphicsObject& other)
{
	graphicsObject* g = new graphicsObject(other.width, other.height, new color[other.width * other.height], nullptr);
	std::memcpy(g->colors, other.colors, other.width * other.height * sizeof(color));
	if (other.depthbuffer != nullptr) {
		g->depthbuffer = new fp[other.width, other.height];
		std::memcpy(g->depthbuffer, other.depthbuffer, other.width * other.height * sizeof(fp));
	}
	return g;
}

void graphicsObject::createColorBuffer()
{
	colors = new color[width * height]();
}

void graphicsObject::createDepthBuffer()
{
	depthbuffer = new fp[width * height]();
}

void graphicsObject::DeleteColors() const
{
	delete[] colors;
}

void graphicsObject::DeleteDepthBuffer() const
{
	delete[] depthbuffer;
}
//textured
//conditions:
//y0 < y1 < y2
//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
void graphicsObject::fillTriangle3D(const fp& x0, const fp& y0, const fp& d0, const vec2& tex0, const fp& x1, const fp& y1, const fp& d1, const vec2& tex1, const fp& x2, const fp& y2, const fp& d2, const vec2& tex2, const Texture& c) const
{
	cint y0Row = y0 > 0 ? fastceil(y0) : 0, maxy = y2 < height ? fastceil(y2) : height, maxsegment0y = y1 < height ? fastceil(y1) : height;

	const fp x2x0 = x2 - x0;//dx from 0 to 2
	const fp x1x0 = x1 - x0;//dx from 0 to 1
	const fp x2x1 = x2 - x1;//dx from 1 to 2

	const mat3x3 barcoords = Texture::GetBarycentricSet(vec2(x0, y0), vec2(x1, y1), vec2(x2, y2));
	const fp total_height = y2 - y0;
	fp segment_height = y1 - y0;
	fp astep = x2x0 / total_height;//line 0 to 2
	fp bstep = x1x0 / segment_height;//line 0 to 1
	const fp minyy0 = (y0Row - y0);
	fp A = x0 + astep * minyy0;//intersection from the left line with current y
	fp B = x0 + bstep * minyy0;//intersection from the right line with current y


	if (A > B) {
		std::swap(A, B);//make a the left line, b the right line.
		std::swap(astep, bstep);
	}


	int y = y0Row;
	//calc values at (0,y0Row)
	fp* ydepthptr = depthbuffer + y * width;
	color* ycolorptr = colors + y * width;
	fp depth00, depthxstep, depthystep;
	Texture::getcoordfunction<fp>(d0, d1, d2, barcoords, depth00, depthxstep, depthystep);
	vec2 tex00, texxstep, texystep;
	Texture::getcoordfunction<vec2>(tex0, tex1, tex2, barcoords, tex00, texxstep, texystep);
	fp depth0y = depth00 + depthystep * y0Row;
	vec2 tex0y = tex00 + texystep * y0Row;
	//fill top triangle(segment 0)
	if (y < maxsegment0y)
	{
	segment0loop:
		cint activeminx = A > 0 ? ceil(A) : 0;//crop
		cint activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp depthxy = depth0y + activeminx * depthxstep;
		vec2 texxy = tex0y + activeminx * texxstep;
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle

			if (depthxy < *activedepthptr)
			{
				if (depthxy < 0) {
					depthxy++;
				}
				const color clr = c.getColor(texxy);
				if (clr.a > 0 || !rendersettings::checkopacity)
				{
					*activedepthptr = depthxy;
					*activecolorptr = clr;
				}
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
			texxy += texxstep;
		}
		//increment y
		//always increment
		ydepthptr += width;
		ycolorptr += width;
		depth0y += depthystep;
		tex0y += texystep;
		if (++y < maxsegment0y)
		{
			A += astep;//only increment if the loop continues
			B += bstep;
			goto segment0loop;
		}
	}
	
	//we expect that y = maxsegment0y
	segment_height = y2 - y1;
	astep = x2x0 / total_height;//line 0 to 2
	bstep = x2x1 / segment_height;//line 1 to 2
	A = x0 + astep * (y - y0);
	B = x1 + bstep * (y - y1);
	if (A > B) {//make a the left line, b the right line.
		std::swap(A, B);
		std::swap(astep, bstep);
	}
	//fill bottom triangle(segment 1)
	if (y < maxy)
	{
	segment1loop:
		int activeminx = A > 0 ? ceil(A) : 0;//crop
		int activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp depthxy = depth0y + activeminx * depthxstep;
		vec2 texxy = tex0y + activeminx * texxstep;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle
			if (depthxy < *activedepthptr)
			{
				const color clr = c.getColor(texxy);
				if (clr.a > 0 || !rendersettings::checkopacity)
				{
					*activedepthptr = depthxy;
					*activecolorptr = clr;
				}
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
			texxy += texxstep;
		}
		if (++y < maxy)
		{//increment y
			A += astep;//only increment a and b if the loop continues.
			B += bstep;
			ydepthptr += width;//no need to increment y if y = maxy because this is the last segment
			ycolorptr += width;
			depth0y += depthystep;
			tex0y += texystep;
			goto segment1loop;
		}
	}
}

//textured
//conditions:
//y0 < y1 < y2
//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
void graphicsObject::fillTriangle3DLight(const fp& x0, const fp& y0, const fp& d0, const vec2& tex0, const vec3& l0, const fp& x1, const fp& y1, const fp& d1, const vec2& tex1, const vec3& l1, const fp& x2, const fp& y2, const fp& d2, const vec2& tex2, const vec3& l2, const Texture& c) const
{
	cint y0Row = y0 > 0 ? (int)ceil(y0) : 0, maxy = y2 < height ? (int)ceil(y2) : height, maxsegment0y = y1 < height ? (int)ceil(y1) : height;

	const fp x2x0 = x2 - x0;//dx from 0 to 2
	const fp x1x0 = x1 - x0;//dx from 0 to 1
	const fp x2x1 = x2 - x1;//dx from 1 to 2

	const mat3x3 barcoords = Texture::GetBarycentricSet(vec2(x0, y0), vec2(x1, y1), vec2(x2, y2));
	const fp total_height = y2 - y0;
	fp segment_height = y1 - y0;
	fp astep = x2x0 / total_height;//line 0 to 2
	fp bstep = x1x0 / segment_height;//line 0 to 1
	const fp minyy0 = (y0Row - y0);
	fp A = x0 + astep * minyy0;//intersection from the left line with current y
	fp B = x0 + bstep * minyy0;//intersection from the right line with current y


	if (A > B) {//x0 + x2x0 / 2 > x0 + x1x0 / 2 = x2x0 / 2 > x1x0 / 2 = x2x0 > x1x0
		std::swap(A, B);//make a the left line, b the right line.
		std::swap(astep, bstep);
	}


	int y = y0Row;
	//calc values at (0,miny)
	fp* ydepthptr = depthbuffer + y * width;
	color* ycolorptr = colors + y * width;
	fp depth00, depthxstep, depthystep;
	Texture::getcoordfunction<fp>(d0, d1, d2, barcoords, depth00, depthxstep, depthystep);
	vec2 tex00, texxstep, texystep;
	Texture::getcoordfunction<vec2>(tex0, tex1, tex2, barcoords, tex00, texxstep, texystep);
	vec3 light00, lightxstep, lightystep;
	Texture::getcoordfunction<vec3>(l0, l1, l2, barcoords, light00, lightxstep, lightystep);
	fp depth0y = depth00 + depthystep * y0Row;
	vec2 tex0y = tex00 + texystep * y0Row;
	vec3 light0y = light00 + lightystep * y0Row;
	//fill top triangle(segment 0)
	if (y < maxsegment0y)
	{
	segment0loop:
		cint activeminx = A > 0 ? ceil(A) : 0;//crop
		cint activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp depthxy = depth0y + activeminx * depthxstep;
		vec2 texxy = tex0y + activeminx * texxstep;
		vec3 lightxy = light0y + activeminx * lightxstep;
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle
			if (depthxy < *activedepthptr)
			{
				const color clrwithoutlight = c.getColor(texxy);
				if (clrwithoutlight.a > 0 || !rendersettings::checkopacity)
				{
					const color clr = clrwithoutlight * lightxy;
					*activedepthptr = depthxy;
					*activecolorptr = clr;
				}
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
			texxy += texxstep;
			lightxy += lightxstep;
		}
		//increment y
		//always increment
		ydepthptr += width;
		ycolorptr += width;
		depth0y += depthystep;
		tex0y += texystep;
		light0y += lightystep;
		if (++y < maxsegment0y)
		{
			A += astep;//only increment if the loop continues
			B += bstep;
			goto segment0loop;
		}
	}

	//we expect that y = maxsegment0y
	segment_height = y2 - y1;
	astep = x2x0 / total_height;//line 0 to 2
	bstep = x2x1 / segment_height;//line 1 to 2
	A = x0 + astep * (y - y0);
	B = x1 + bstep * (y - y1);
	//x0 + x2x0 * 0.5 > 
	//x1 + x2x1 * 0.5

	//

	//x2x1 > x2x0

	//A > B
	if (A > B) {//make a the left line, b the right line.
		std::swap(A, B);
		std::swap(astep, bstep);
	}
	//fill bottom triangle(segment 1)
	if (y < maxy)
	{
	segment1loop:
		int activeminx = A > 0 ? ceil(A) : 0;//crop
		int activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp depthxy = depth0y + activeminx * depthxstep;
		vec2 texxy = tex0y + activeminx * texxstep;
		vec3 lightxy = light0y + activeminx * lightxstep;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle
			if (depthxy < *activedepthptr)
			{
				const color clrwithoutlight = c.getColor(texxy);
				if (clrwithoutlight.a > 0 || !rendersettings::checkopacity)
				{
					const color clr = clrwithoutlight * lightxy;
					*activedepthptr = depthxy;
					*activecolorptr = clr;
				}
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
			texxy += texxstep;
			lightxy += lightxstep;
		}
		if (++y < maxy)
		{//increment y
			A += astep;//only increment a and b if the loop continues.
			B += bstep;
			ydepthptr += width;//no need to increment y if y = maxy because this is the last segment
			ycolorptr += width;
			depth0y += depthystep;
			tex0y += texystep;
			light0y += lightystep;
			goto segment1loop;
		}
	}
}



//single color
//conditions:
//y0 < y1 < y2
//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
void graphicsObject::fillTriangle3D(const fp& x0, const fp& y0, const fp& d0, const fp& x1, const fp& y1, const fp& d1, const fp& x2, const fp& y2, const fp& d2, const color& c) const
{
	if(c.a < 0xff)
	{
		fillTriangle3DOpacity(x0, y0, d0, x1, y1, d1, x2, y2, d2, c);
		return;
	}
	cint y0Row = y0 > 0 ? (int)ceil(y0) : 0, maxy = y2 < height ? (int)ceil(y2) : height, maxsegment0y = y1 < height ? (int)ceil(y1) : height;

	const fp x2x0 = x2 - x0;//dx from 0 to 2
	const fp x1x0 = x1 - x0;//dx from 0 to 1
	const fp x2x1 = x2 - x1;//dx from 1 to 2

	const mat3x3 barcoords = Texture::GetBarycentricSet(vec2(x0, y0), vec2(x1, y1), vec2(x2, y2));
	const fp total_height = y2 - y0;
	fp segment_height = y1 - y0;
	fp astep = x2x0 / total_height;//line 0 to 2
	fp bstep = x1x0 / segment_height;//line 0 to 1
	const fp minyy0 = (y0Row - y0);
	fp A = x0 + astep * minyy0;//intersection from the left line with current y
	fp B = x0 + bstep * minyy0;//intersection from the right line with current y


	if (A > B) {
		std::swap(A, B);//make a the left line, b the right line.
		std::swap(astep, bstep);
	}


	int y = y0Row;
	//calc values at (0,miny)
	fp* ydepthptr = depthbuffer + y * width;
	color* ycolorptr = colors + y * width;
	fp depth00, depthxstep, depthystep;
	Texture::getcoordfunction<fp>(d0, d1, d2, barcoords, depth00, depthxstep, depthystep);
	fp depth0y = depth00 + depthystep * y0Row;
	//fill top triangle(segment 0)
	if (y < maxsegment0y)
	{
	segment0loop:
		cint activeminx = A > 0 ? ceil(A) : 0;//crop
		cint activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp depthxy = depth0y + activeminx * depthxstep;
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle

			if (depthxy < *activedepthptr)
			{
				*activedepthptr = depthxy;
				*activecolorptr = c;
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
		}
		//increment y
		//always increment
		ydepthptr += width;
		ycolorptr += width;
		depth0y += depthystep;
		if (++y < maxsegment0y)
		{
			A += astep;//only increment if the loop continues
			B += bstep;
			goto segment0loop;
		}
	}

	//we expect that y = maxsegment0y
	segment_height = y2 - y1;
	astep = x2x0 / total_height;//line 0 to 2
	bstep = x2x1 / segment_height;//line 1 to 2
	A = x0 + astep * (y - y0);
	B = x1 + bstep * (y - y1);
	if (A > B) {//make a the left line, b the right line.
		std::swap(A, B);
		std::swap(astep, bstep);
	}
	//fill bottom triangle(segment 1)
	if (y < maxy)
	{
	segment1loop:
		int activeminx = A > 0 ? ceil(A) : 0;//crop
		int activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp depthxy = depth0y + activeminx * depthxstep;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle
			if (depthxy < *activedepthptr)
			{
				*activedepthptr = depthxy;
				*activecolorptr = c;
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
		}
		if (++y < maxy)
		{//increment y
			A += astep;//only increment a and b if the loop continues.
			B += bstep;
			ydepthptr += width;//no need to increment y if y = maxy because this is the last segment
			ycolorptr += width;
			depth0y += depthystep;
			goto segment1loop;
		}
	}
}
void graphicsObject::fillTriangle3DLight(
	const fp& x0, const fp& y0, const fp& d0, const vec3& l0, 
	const fp& x1, const fp& y1, const fp& d1, const vec3& l1, 
	const fp& x2, const fp& y2, const fp& d2, const vec3& l2, 
	const color& c) const
{
	if (c.a < 0xff)
	{
		fillTriangle3DOpacity(x0, y0, d0, x1, y1, d1, x2, y2, d2, c);
		return;
	}
	cint y0Row = y0 > 0 ? (int)ceil(y0) : 0, maxy = y2 < height ? (int)ceil(y2) : height, maxsegment0y = y1 < height ? (int)ceil(y1) : height;

	const fp x2x0 = x2 - x0;//dx from 0 to 2
	const fp x1x0 = x1 - x0;//dx from 0 to 1
	const fp x2x1 = x2 - x1;//dx from 1 to 2

	const mat3x3 barcoords = Texture::GetBarycentricSet(vec2(x0, y0), vec2(x1, y1), vec2(x2, y2));
	const fp total_height = y2 - y0;
	fp segment_height = y1 - y0;
	fp astep = x2x0 / total_height;//line 0 to 2
	fp bstep = x1x0 / segment_height;//line 0 to 1
	const fp minyy0 = (y0Row - y0);
	fp A = x0 + astep * minyy0;//intersection from the left line with current y
	fp B = x0 + bstep * minyy0;//intersection from the right line with current y


	if (A > B) {
		std::swap(A, B);//make a the left line, b the right line.
		std::swap(astep, bstep);
	}


	int y = y0Row;
	//calc values at (0,miny)
	fp* ydepthptr = depthbuffer + y * width;
	color* ycolorptr = colors + y * width;

	fp depth00, depthxstep, depthystep;
	Texture::getcoordfunction<fp>(d0, d1, d2, barcoords, depth00, depthxstep, depthystep);
	fp depth0y = depth00 + depthystep * y0Row;

	vec3 light00, lightxstep, lightystep;
	Texture::getcoordfunction<vec3>(l0, l1, l2, barcoords, light00, lightxstep, lightystep);
	vec3 light0y = light00 + lightystep * y0Row;

	//fill top triangle(segment 0)
	if (y < maxsegment0y)
	{
	segment0loop:
		cint activeminx = A > 0 ? ceil(A) : 0;//crop
		cint activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp depthxy = depth0y + activeminx * depthxstep;
		vec3 lightxy = light0y + activeminx * lightxstep;
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle

			if (depthxy < *activedepthptr)
			{
				*activedepthptr = depthxy;
				*activecolorptr = c * lightxy;
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
			lightxy += lightxstep;
		}
		//increment y
		//always increment
		ydepthptr += width;
		ycolorptr += width;
		depth0y += depthystep;
		light0y += lightystep;
		if (++y < maxsegment0y)
		{
			A += astep;//only increment if the loop continues
			B += bstep;
			goto segment0loop;
		}
	}

	//we expect that y = maxsegment0y
	segment_height = y2 - y1;
	astep = x2x0 / total_height;//line 0 to 2
	bstep = x2x1 / segment_height;//line 1 to 2
	A = x0 + astep * (y - y0);
	B = x1 + bstep * (y - y1);
	if (A > B) {//make a the left line, b the right line.
		std::swap(A, B);
		std::swap(astep, bstep);
	}
	//fill bottom triangle(segment 1)
	if (y < maxy)
	{
	segment1loop:
		int activeminx = A > 0 ? ceil(A) : 0;//crop
		int activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp depthxy = depth0y + activeminx * depthxstep;
		vec3 lightxy = light0y + activeminx * lightxstep;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle
			if (depthxy < *activedepthptr)
			{
				*activedepthptr = depthxy;
				*activecolorptr = c * lightxy;
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
			lightxy += lightxstep;
		}
		if (++y < maxy)
		{//increment y
			A += astep;//only increment a and b if the loop continues.
			B += bstep;
			ydepthptr += width;//no need to increment y if y = maxy because this is the last segment
			ycolorptr += width;
			depth0y += depthystep;
			light0y += lightystep;
			goto segment1loop;
		}
	}
}
//single transparent color
//conditions:
//y0 < y1 < y2
//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
void graphicsObject::fillTriangle3DOpacity(const fp& x0, const fp& y0, const fp& d0, const fp& x1, const fp& y1, const fp& d1, const fp& x2, const fp& y2, const fp& d2, const color& c) const
{
	cint y0Row = y0 > 0 ? (int)ceil(y0) : 0, maxy = y2 < height ? (int)ceil(y2) : height, maxsegment0y = y1 < height ? (int)ceil(y1) : height;

	const fp weight = c.a * bytemult0to1;//opacity

	const fp x2x0 = x2 - x0;//dx from 0 to 2
	const fp x1x0 = x1 - x0;//dx from 0 to 1
	const fp x2x1 = x2 - x1;//dx from 1 to 2

	const mat3x3 barcoords = Texture::GetBarycentricSet(vec2(x0, y0), vec2(x1, y1), vec2(x2, y2));
	const fp total_height = y2 - y0;
	fp segment_height = y1 - y0;
	fp astep = x2x0 / total_height;//line 0 to 2
	fp bstep = x1x0 / segment_height;//line 0 to 1
	const fp minyy0 = (y0Row - y0);
	fp A = x0 + astep * minyy0;//intersection from the left line with current y
	fp B = x0 + bstep * minyy0;//intersection from the right line with current y


	if (A > B) {
		std::swap(A, B);//make a the left line, b the right line.
		std::swap(astep, bstep);
	}


	int y = y0Row;
	//calc values at (0,miny)
	fp* ydepthptr = depthbuffer + y * width;
	color* ycolorptr = colors + y * width;
	fp depth00, depthxstep, depthystep;
	Texture::getcoordfunction<fp>(d0, d1, d2, barcoords, depth00, depthxstep, depthystep);
	fp depth0y = depth00 + depthystep * y0Row;
	//fill top triangle(segment 0)
	if (y < maxsegment0y)
	{
	segment0loop:
		cint activeminx = A > 0 ? ceil(A) : 0;//crop
		cint activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp depthxy = depth0y + activeminx * depthxstep;
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle

			if (depthxy < *activedepthptr)
			{
				*activedepthptr = depthxy;
				*activecolorptr = color::lerpcolor(*activecolorptr, c, weight);
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
		}
		//increment y
		//always increment
		ydepthptr += width;
		ycolorptr += width;
		depth0y += depthystep;
		if (++y < maxsegment0y)
		{
			A += astep;//only increment if the loop continues
			B += bstep;
			goto segment0loop;
		}
	}

	//we expect that y = maxsegment0y
	segment_height = y2 - y1;
	astep = x2x0 / total_height;//line 0 to 2
	bstep = x2x1 / segment_height;//line 1 to 2
	A = x0 + astep * (y - y0);
	B = x1 + bstep * (y - y1);
	if (A > B) {//make a the left line, b the right line.
		std::swap(A, B);
		std::swap(astep, bstep);
	}
	//fill bottom triangle(segment 1)
	if (y < maxy)
	{
	segment1loop:
		int activeminx = A > 0 ? ceil(A) : 0;//crop
		int activemaxx = B < width ? ceil(B) : width;
		//calc values at(minx, miny)
		fp* activedepthptr = ydepthptr + activeminx;
		color* activecolorptr = ycolorptr + activeminx;
		fp depthxy = depth0y + activeminx * depthxstep;
		fp* endxptr = ydepthptr + activemaxx;
		while (activedepthptr < endxptr) {//fill horizontal line of triangle
			if (depthxy < *activedepthptr)
			{
				*activedepthptr = depthxy;
				*activecolorptr = color::lerpcolor(*activecolorptr, c, weight);
			}
			activedepthptr++;
			activecolorptr++;
			depthxy += depthxstep;
		}
		if (++y < maxy)
		{//increment y
			A += astep;//only increment a and b if the loop continues.
			B += bstep;
			ydepthptr += width;//no need to increment y if y = maxy because this is the last segment
			ycolorptr += width;
			depth0y += depthystep;
			goto segment1loop;
		}
	}
}
//returns clipped triangles against the screen
int graphicsObject::Triangle_ClipAgainstScreen(const mat4x4& view, triangle& in_tri, triangle& out_tri0, triangle& out_tri1) const
{
	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	int insidecount = 0;
	int outsidecount = 0;
	vec3* inside_screen[3];
	vec3* outside_screen[3];
	vec3* inside_points[3];
	vec3* outside_points[3];
	vec2* inside_tex[3];
	vec2* outside_tex[3];
	vec3* inside_light[3];
	vec3* outside_light[3];

	//const bool PointInBack[3] = { in_tri.screen[0].z <= rendersettings::s3d::mindistance, in_tri.screen[1].z <= rendersettings::s3d::mindistance,in_tri.screen[2].z <= rendersettings::s3d::mindistance };//determines wether this point is behind the frustum
	const bool PointInBack[3] = { in_tri.screen[0].z <= 0, in_tri.screen[1].z <= 0,in_tri.screen[2].z <= 0 };//determines wether this point is behind the frustum
	if (PointInBack[0] || PointInBack[1] || PointInBack[2])
	{
		if (PointInBack[0])//outside
		{
			if (PointInBack[1] && PointInBack[2])return 0;//too near
			outside_points[outsidecount] = &in_tri.p[0]; outside_tex[outsidecount] = &in_tri.t[0]; outside_screen[outsidecount] = &in_tri.screen[0]; outside_light[outsidecount] = &in_tri.light[0];
			outsidecount++;
		}
		else
		{
			inside_points[insidecount] = &in_tri.p[0]; inside_tex[insidecount] = &in_tri.t[0]; inside_screen[insidecount] = &in_tri.screen[0]; inside_light[insidecount] = &in_tri.light[0];
			insidecount++;
		}
		if (PointInBack[1])
		{
			outside_points[outsidecount] = &in_tri.p[1];  outside_tex[outsidecount] = &in_tri.t[1]; outside_screen[outsidecount] = &in_tri.screen[1]; outside_light[outsidecount] = &in_tri.light[1];
			outsidecount++;
		}
		else
		{
			inside_points[insidecount] = &in_tri.p[1]; inside_tex[insidecount] = &in_tri.t[1]; inside_screen[insidecount] = &in_tri.screen[1]; inside_light[insidecount] = &in_tri.light[1];
			insidecount++;
		}
		if (PointInBack[2]) {
			outside_points[outsidecount] = &in_tri.p[2];  outside_tex[outsidecount] = &in_tri.t[2]; outside_screen[outsidecount] = &in_tri.screen[2]; outside_light[outsidecount] = &in_tri.light[2];
			outsidecount++;
		}
		else {
			inside_points[insidecount] = &in_tri.p[2]; inside_tex[insidecount] = &in_tri.t[2]; inside_screen[insidecount] = &in_tri.screen[2]; inside_light[insidecount] = &in_tri.light[2];
			insidecount++;
		}
		if (PointInBack[0] == PointInBack[2] && PointInBack[0] != PointInBack[1]) 
		{
			//swap
			if (PointInBack[0]) 
			{
				std::swap(outside_points[0], outside_points[1]);
				std::swap(outside_tex[0], outside_tex[1]);
				std::swap(outside_screen[0], outside_screen[1]);
				std::swap(outside_light[0], outside_light[1]);
			}
			else 
			{
				std::swap(inside_points[0], inside_points[1]);
				std::swap(inside_tex[0], inside_tex[1]);
				std::swap(inside_screen[0], inside_screen[1]);
				std::swap(inside_light[0], inside_light[1]);
			}
			
		}
		// Now classify triangle points, and break the input triangle into 
		// smaller output triangles if required. There are four possible
		// outcomes...
		//insidepointcount has to be 1 or 2

		if (insidecount == 1)//&& outsidecount == 2
		{
			//form clipped triangle
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

			// The inside point is valid, so keep that...
			out_tri0.screen[0] = *inside_screen[0];
			out_tri0.t[0] = *inside_tex[0];
			out_tri0.light[0] = *inside_light[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the screen
			fp t = getw<fp>(inside_screen[0]->z, outside_screen[0]->z, 0);
			out_tri0.p[1] = lerp<vec3>(*inside_points[0], *outside_points[0], t);
			out_tri0.t[1] = lerp<vec2>(*inside_tex[0], *outside_tex[0], t);
			out_tri0.light[1] = lerp<vec3>(*inside_light [0] , *outside_light[0], t);

			t = getw<fp>(inside_screen[0]->z, outside_screen[1]->z, 0);
			out_tri0.p[2] = lerp<vec3>(*inside_points[0], *outside_points[1], t);
			out_tri0.t[2] = lerp<vec2>(*inside_tex[0], *outside_tex[1], t);
			out_tri0.light[2] = lerp<vec3>(*inside_light [0] , *outside_light[1], t);
			//recalculate screen pos
			out_tri0.screen[1] = windowspace(view, out_tri0.p[1]);
			out_tri0.screen[2] = windowspace(view, out_tri0.p[2]);
			if (out_tri0.screen[0].z < 0) 
			{
				int a = 4;
			}

			return 1; // Return the newly formed single triangle
		}
		if (insidecount == 2)// && outsidecount == 1
		{
			//form quad
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles

			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri0.screen[0] = *inside_screen[0];
			out_tri0.screen[1] = *inside_screen[1];
			out_tri0.t[0] = *inside_tex[0];
			out_tri0.t[1] = *inside_tex[1];
			out_tri0.light[0] = *inside_light[0];
			out_tri0.light[1] = *inside_light[1];

			fp t = getw<fp>(inside_screen[0]->z, outside_screen[0]->z, 0);
			out_tri0.p[2] = lerp<vec3>(*inside_points[0], *outside_points[0], t);
			out_tri0.t[2] = lerp<vec2>(*inside_tex[0], *outside_tex[0], t);
			out_tri0.light[2] = lerp<vec3>(*inside_light[0], *outside_light[0], t);

			//recalculate screen pos
			out_tri0.screen[2] = windowspace(view, out_tri0.p[2]);

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri1.screen[0] = *inside_screen[1];//0 = inside point
			out_tri1.t[0] = *inside_tex[1];
			out_tri1.light[0] = *inside_light[1];
			out_tri1.screen[2] = out_tri0.screen[2];//1 = i0 * o0
			out_tri1.t[2] = out_tri0.t[2];
			out_tri1.light[2] = out_tri0.light[2];

			t = getw<fp>(inside_screen[1]->z, outside_screen[0]->z, 0);
			out_tri1.p[1] = lerp<vec3>(*inside_points[1], *outside_points[0], t);//2 = i1 * o0
			out_tri1.t[1] = lerp<vec2>(*inside_tex[1], *outside_tex[0], t);
			out_tri1.light[1] = lerp<vec3>(*inside_light[1], *outside_light[0], t);
			//recalculate screen pos
			out_tri1.screen[1] = windowspace(view, out_tri1.p[1]);

			if (out_tri0.screen[0].z < 0)
			{
				int a = 4;
			}
			return 2; // Return two newly formed triangles which form a quad
		}
	}
	else
	{
		if (in_tri.screen[0].z >= rendersettings::s3d::maxdistance && in_tri.screen[1].z >= rendersettings::s3d::maxdistance && in_tri.screen[2].z >= rendersettings::s3d::maxdistance)
		{
			return 0;//too far
		}
		out_tri0 = in_tri;
		return 1;
	}
	return -1;
}
