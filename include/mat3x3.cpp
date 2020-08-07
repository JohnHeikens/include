#include "mat3x3.h"
#include "mat3x3.h"
#include "mat3x3.h"
#include "mat3x3.h"
mat3x3 operator *(fp mult, mat3x3 matrix)
{
	return mat3x3
	(
		matrix.mxX * mult,
		matrix.myX * mult,
		matrix.mzX * mult,
		matrix.mxY * mult,
		matrix.myY * mult,
		matrix.mzY * mult,
		matrix.mxZ * mult,
		matrix.myZ * mult,
		matrix.mzZ * mult
	);
}
mat3x3 operator *(mat3x3 matrix, fp mult)
{
	return mat3x3
	(
		matrix.mxX * mult,
		matrix.myX * mult,
		matrix.mzX * mult,
		matrix.mxY * mult,
		matrix.myY * mult,
		matrix.mzY * mult,
		matrix.mxZ * mult,
		matrix.myZ * mult,
		matrix.mzZ * mult
	);
}

mat3x3 mat3x3::Inverse() const
{
	//https://en.wikipedia.org/wiki/Inverse_matrix
	//https://www.youtube.com/watch?v=pKZyszzmyeQ
	fp multiplier = 1 / Determinant();
	//N
	return
		mat3x3(//reflected/transposed=>10>01
	   //each part of the matrix finds himself 4 times
			multiplier * (m11 * m22 - m12 * m21),//00
			multiplier * (m12 * m20 - m10 * m22),//01 reverse value(-value)
			multiplier * (m10 * m21 - m11 * m20),//02
			multiplier * (m02 * m21 - m01 * m22),//10 reverse value(-value) so bl*tr - tl*br
			multiplier * (m00 * m22 - m02 * m20),//11
			multiplier * (m01 * m20 - m00 * m21),//12 reverse value(-value)
			multiplier * (m01 * m12 - m02 * m11),//20
			multiplier * (m02 * m10 - m00 * m12),//21 reverse value(-value)
			multiplier * (m00 * m11 - m01 * m10) //22
		);
}

mat3x3 mat3x3::transposed() const
{
	return mat3x3(mxX, mxY, mxZ, myX, myY, myZ, mzX, mzY, mzZ);
}

fp mat3x3::Determinant() const
{

	//a = the \ diagonal, b = the / diagonal
	return
		//the \ diagonals
		mxX * myY * mzZ
		+ myX * mzY * mxZ
		+ mzX * mxY * myZ
		//the / diagonals
		- mzX * myY * mxZ
		- mxX * mzY * myZ
		- myX * mxY * mzZ;

}

//if x++, output += getxstep
vec2 mat3x3::getxstep() const
{
	return vec2(this->mxX,this->mxY);
}

//if y++, output += getystep
vec2 mat3x3::getystep() const
{
	return vec2(this->myX, this->myY);
}
//output at x 0 y 0
vec2 mat3x3::getstart() const
{
	return vec2(mzX,mzY);
}

mat3x3 mat3x3::translate2d(vec2 add)
{
	return mat3x3(1, 0, add.x, 0, 1, add.y, 0, 0, 1);
}

mat3x3 mat3x3::rotate2d(cvec2& rotateAround, cfp& angle)
{
	return mat3x3::combine({
		//first set rotation point to 0,0
		mat3x3::translate2d(-rotateAround),

		//then rotate
		mat3x3::rotate2d(angle),
		//then move it back
		mat3x3::translate2d(rotateAround)
		}
	);
}

mat3x3 mat3x3::rotate2d(const fp& angle)
{
	fp sina = sin(angle);
	fp cosa = cos(angle);
	return mat3x3(cosa, sina, 0, -sina, cosa, 0, 0, 0, 1);;
}
mat3x3 mat3x3::rotate3d(vec3 axis,const fp& angle)
{
	axis.Normalize();
	fp sinr = (fp)sin(angle);
	fp cosr = (fp)cos(angle);
	fp mincos = 1 - cosr;
	//https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	return mat3x3
	{
		cosr + axis.x * axis.x * mincos,
		axis.x * axis.y * mincos - axis.z * sinr,
		axis.x * axis.z * mincos + axis.y * sinr,
		axis.y * axis.x * mincos + axis.z * sinr,
		cosr + axis.y * axis.y * mincos,
		axis.y * axis.z * mincos - axis.x * sinr,
		axis.z * axis.x * mincos - axis.y * sinr,
		axis.z * axis.y * mincos + axis.x * sinr,
		cosr + axis.z * axis.z * mincos
	};
	//axis.Normalize();
	//Matrix lhs = axis;
	//Matrix cross = Cross(lhs, matrix);
	//return cos * matrix + sin * cross;
}

mat3x3 mat3x3::scale2d(const fp& scalar)
{
	return mat3x3(scalar,0,0,0,scalar,0,0,0,1);
}
mat3x3 mat3x3::scale2d(const vec2& scalar)
{
	return mat3x3(scalar.x, 0, 0, 0, scalar.y, 0, 0, 0, 1);
}

mat3x3 mat3x3::scale3d(const fp& scalar)
{
	return mat3x3(scalar, 0, 0, 0, scalar, 0, 0, 0, scalar);
}

mat3x3 mat3x3::mult2d(const vec2& vector)
{
	return mat3x3(vector.x, 0, 0, 0, vector.y, 0, 0, 0, 1);
}

mat3x3 mat3x3::mult3d(const vec3& vector)
{
	return mat3x3(vector.x, 0, 0, 0, vector.y, 0, 0, 0, vector.z);
}

void mat3x3::MultiplyX(fp value)
{
	mxX *= value;
	myX *= value;
	mzX *= value;
}

void mat3x3::MultiplyY(fp value)
{
	mxY *= value;
	myY *= value;
	mzY *= value;
}

void mat3x3::MultiplyZ(fp value)
{
	mxZ *= value;
	myZ *= value;
	mzZ *= value;
}

vec3 mat3x3::multPointMatrix(const vec3& in) const
{
	return vec3
	(
		mxX * in.x + myX * in.y + mzX * in.z,
		mxY * in.x + myY * in.y + mzY * in.z,
		mxZ * in.x + myZ * in.y + mzZ * in.z
	);
}

rectangle2 mat3x3::multRectMatrix(const rectangle2& in) const
{
	return rectangle2(multPointMatrix(in.pos00), resizeSize(in.size));
}

vec2 mat3x3::multPointMatrix(const vec2& in) const
{
	return vec2
	(
		mxX * in.x + myX * in.y + mzX * 1,
		mxY * in.x + myY * in.y + mzY * 1
	);
}
vec2 mat3x3::resizeSize(const vec2& size) const
{
	return vec2(
		mxX * size.x + myX * size.y,
		mxY * size.x + myY * size.y);
}
//glm method
//https://codereview.stackexchange.com/questions/186770/4%C3%974-matrix-multiplication
mat3x3 mat3x3::cross(const mat3x3& last, const mat3x3& first)
{
	mat3x3 mult = mat3x3(0);//all values are 0
	int index, i, j;
	for (i = 0; i < 3; ++i) for (j = 0; j < 3; ++j) for (index = 0; index < 3; ++index) {
		mult.arr2d[i][index] += last.arr2d[j][index] * first.arr2d[i][j];
		//mult.arr2d[i][j] += a.arr2d[i][index] * b.arr2d[index][j];
	}
	return mult;
}