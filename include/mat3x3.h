#include "vec3.h"
#include "vec2.h"
#include "rectangle2.h"
#pragma once
union mat3x3
{
public:
	//standard: input = output
	mat3x3() :
		mxX(1), myX(0), mzX(0),
		mxY(0), myY(1), mzY(0),
		mxZ(0), myZ(0), mzZ(1)
	{}
	//whatever u want
	mat3x3(
		fp mxx, fp myx, fp mzx,
		fp mxy, fp myy, fp mzy,
		fp mxz, fp myz, fp mzz) :
		mxX(mxx), myX(myx), mzX(mzx),
		mxY(mxy), myY(myy), mzY(mzy),
		mxZ(mxz), myZ(myz), mzZ(mzz)
	{}
	//multiply by the vector
	mat3x3(vec3 mult) :
		mxX(mult.x), myX(0), mzX(0),
		mxY(0), myY(mult.y), mzY(0),
		mxZ(0), myZ(0), mzZ(mult.z)
	{}
	mat3x3 Inverse() const;
	mat3x3 transposed() const;
	fp Determinant() const;
	vec2 getxstep() const;
	vec2 getystep() const;
	vec2 getstart() const;
	static mat3x3 translate2d(vec2 add);
	static mat3x3 rotate2d(const fp& angle);
	static mat3x3 rotate2d(cvec2& rotateAround, cfp& angle);
	static mat3x3 rotate3d(vec3 axis, const fp& angle);
	static mat3x3 scale2d(const fp& scalar);
	static mat3x3 scale2d(const vec2& scalar);
	static mat3x3 scale3d(const fp& scalar);
	static mat3x3 mult2d(const vec2& vector);
	static mat3x3 mult3d(const vec3& vector);
	static inline mat3x3 fromRectToRect(const rectangle2& rectFrom, const rectangle2& rectTo) 
	{
		const mat3x3 scale = scale2d(rectTo.size / rectFrom.size);
		const mat3x3 translateFrom = translate2d(-rectFrom.pos00);
		const mat3x3 translateTo = translate2d(rectTo.pos00);
		return mat3x3::cross(translateTo, mat3x3::cross(scale, translateFrom));
	}

	void MultiplyX(fp value);
	void MultiplyY(fp value);
	void MultiplyZ(fp value);
	
	vec3 multPointMatrix(const vec3& in) const;
	rectangle2 multRectMatrix(const rectangle2& in) const;
	//translation on z
	//vector.z = 1
	vec2 multPointMatrix(const vec2& vector) const;
	vec2 resizeSize(const vec2& size) const;
	static mat3x3 cross(const mat3x3& last, const mat3x3& first);
	/*vec3 Apply(fp x, fp y, fp z) const
	{
		return vec3
		{
			x = xX * x + yX * y + zX * z,
			y = xY * x + yY * y + zY * z,
			z = xZ * x + yZ * y + zZ * z,
		};
	}*/

	
	struct {
		fp m00; fp m01; fp m02;
		fp m10; fp m11; fp m12;
		fp m20; fp m21; fp m22;
	};
	struct {
		fp mxX; fp mxY; fp mxZ;
		fp myX; fp myY; fp myZ;
		fp mzX; fp mzY; fp mzZ;
	};
	fp arr1d[3 * 3];
	fp arr2d[3][3];

	//first to last
	inline static mat3x3 combine(const std::initializer_list< mat3x3> matrices)
	{
		auto i = matrices.begin();
		mat3x3 out = *i;
		while (++i != matrices.end())
		{
			out = mat3x3::cross(*i, out);
		}
		return out;
	}
	//from first to last : 0 to last
	inline static mat3x3 combine(std::vector<mat3x3> matrices)
	{
		auto i = matrices.begin();
		mat3x3 out = *i;
		while (++i != matrices.end())
		{
			out = mat3x3::cross(*i, out);
		}
		return out;
	}
};
static mat3x3 operator *(fp mult, mat3x3 matrix);
static mat3x3 operator *(mat3x3 matrix, fp mult);