#pragma once
#include "vec3.h"
#include <cstdio> 
#include <cstdlib> 
#include <fstream> 

//row major
//https://en.wikipedia.org/wiki/Row-_and_column-major_order
union mat4x4
{
public:
	mat4x4() :
		m00(1), m10(0), m20(0), m30(0),
		m01(0), m11(1), m21(0), m31(0),
		m02(0), m12(0), m22(1), m32(0),
		m03(0), m13(0), m23(0), m33(1)
	{};
	mat4x4(fp fill) :
		m00(fill), m10(0), m20(0), m30(0),
		m01(0), m11(fill), m21(0), m31(0),
		m02(0), m12(0), m22(fill), m32(0),
		m03(0), m13(0), m23(0), m33(fill)
	{};
	//whatever u want
	mat4x4(
		fp mxx, fp myx, fp mzx, fp mwx,
		fp mxy, fp myy, fp mzy, fp mwy,
		fp mxz, fp myz, fp mzz, fp mwz,
		fp mxw, fp myw, fp mzw, fp mww) :
		mxX(mxx), myX(myx), mzX(mzx), mwX(mwx),
		mxY(mxy), myY(myy), mzY(mzy), mwY(mwy),
		mxZ(mxz), myZ(myz), mzZ(mzz), mwZ(mwz),
		mxW(mxw), myW(myw), mzW(mzw), mwW(mww)
	{}
	//mxy
	//m[from][to]
	struct {
		fp m00; fp m01; fp m02; fp m03;
		fp m10; fp m11; fp m12; fp m13;
		fp m20; fp m21; fp m22; fp m23;
		fp m30; fp m31; fp m32; fp m33;
	};
	struct {
		fp mxX; fp mxY; fp mxZ; fp mxW;
		fp myX; fp myY; fp myZ; fp myW;
		fp mzX; fp mzY; fp mzZ; fp mzW;
		fp mwX; fp mwY; fp mwZ; fp mwW;
	};
	fp arr1d[4 * 4];
	fp arr2d[4][4];
	vec3 multPointMatrix(const vec3& in) const;
	static mat4x4 rotate3d(vec3 axis, const fp& angle);
 	static mat4x4 scale4d(fp scalar);
	static mat4x4 translate3d(vec3 add);
	static mat4x4 scale4d(fp scaleX, fp scaleY, fp scaleZ, fp scaleW);
	static mat4x4 scale3d(vec3 scale);
	static mat4x4 cross(const mat4x4& last, const mat4x4& first);
	static mat4x4 perspectiveFov(fp const& fov, fp const& width, fp const& height, fp const& zNear, fp const& zFar);
	static mat4x4 lookat(vec3 eye, vec3 center, vec3 const& up);
	inline static mat4x4 combine(const std::initializer_list< mat4x4> matrices)
	{
		auto i = matrices.begin();
		mat4x4 out = *i;
		while (++i != matrices.end())
		{
			out = mat4x4::cross(*i, out);
		}
		return out;
	}
	inline static mat4x4 combine(std::vector<mat4x4> matrices)
	{
		auto i = matrices.begin();
		mat4x4 out = *i;
		while (++i != matrices.end())
		{
			out = mat4x4::cross(*i, out);
		}
		return out;
	}
};
bool operator ==(mat4x4 m1, mat4x4 m2);
#pragma once
static const mat4x4 YtoZ = mat4x4(
	1, 0, 0, 0, 
	0, 0, 1, 0,//switched y with z 
	0, 1, 0, 0, 
	0, 0, 0, 1);