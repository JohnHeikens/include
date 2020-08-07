#include "Texture.h"
#include "Texture.h"
bool rendersettings::multsize = false;
bool rendersettings::Remaindering = false;

color Texture::getColor(const vec2& pos) const
{
	throw("getcolor not implemented");
}
// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
void Texture::Barycentric(const vec2& p, const vec2& a, const vec2& b, const vec2& c, fp& u, fp& v, fp& w)
{
	const vec2 v0 = b - a, v1 = c - a, v2 = p - a;
	const fp d00 = vec2::dot(v0, v0);
	const fp d01 = vec2::dot(v0, v1);
	const fp d11 = vec2::dot(v1, v1);
	const fp d20 = vec2::dot(v2, v0);
	const fp d21 = vec2::dot(v2, v1);
	const fp denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;
}
// Compute barycentric set with respect to triangle (a, b, c)
//multiply a vec2 by the returned matrix and get a vec2(v, w)
mat3x3 Texture::GetBarycentricSet(const vec2& a, const vec2& b, const vec2& c)
{
	const vec2 v0 = b - a, v1 = c - a,
		v2a00 = -a, v2a10 = vec2(1, 0) - a, v2a01 = vec2(0, 1) - a;//calculate for (0,0),(1,0),(0,1)
	const fp d00 = vec2::dot(v0, v0);
	const fp d01 = vec2::dot(v0, v1);
	const fp d11 = vec2::dot(v1, v1);
	const fp d20a00 = vec2::dot(v2a00, v0);
	const fp d21a00 = vec2::dot(v2a00, v1);
	const fp d20a10 = vec2::dot(v2a10, v0);
	const fp d21a10 = vec2::dot(v2a10, v1);
	const fp d20a01 = vec2::dot(v2a01, v0);
	const fp d21a01 = vec2::dot(v2a01, v1);
	const fp denom = d00 * d11 - d01 * d01;
	const fp div = 1.0 / denom;
	const fp va00 = (d11 * d20a00 - d01 * d21a00) * div;//v at x 0 y 0
	const fp wa00 = (d00 * d21a00 - d01 * d20a00) * div;//w at x 0 y 0
	const fp va10 = (d11 * d20a10 - d01 * d21a10) * div;//v at x 1 y 0
	const fp wa10 = (d00 * d21a10 - d01 * d20a10) * div;//w at x 1 y 0
	const fp va01 = (d11 * d20a01 - d01 * d21a01) * div;//v at x 0 y 1
	const fp wa01 = (d00 * d21a01 - d01 * d20a01) * div;//w at x 0 y 1													
	//multx, multy, plus
	return mat3x3(
		va10 - va00, va01 - va00, va00,//v
		wa10 - wa00, wa01 - wa00, wa00,//w
		0, 0, 1//empty
	);
}

//color array WILL BE DELETED!
void Texture::destruct()
{
	delete colors;

}
