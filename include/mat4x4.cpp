#include "mat4x4.h"

//left handed coordinate system
/*Point-Matrix multiplication. The input point is assumed to have Cartesian coordinates but because we will multiply this point by a 4x4 matrix we actually assume it is a point with homogeneous coordinatess (x, y, z, w = 1). The point-matrix results in another point with homogeneous coordinates (x', y', z', w'). To get back to Cartesian coordinates we need to noramlized these coordinates: (x'/w', y'/w', z'/w').*/
//https://www.scratchapixel.com/code.php?id=4&origin=/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix&src=2
vec3 mat4x4::multPointMatrix(const vec3& in) const
{
	//out = in * Mproj;
	vec3 out = vec3(
	in.x * m00 + in.y * m10 + in.z * m20 + m30, //in.w = 1
	in.x * m01 + in.y * m11 + in.z * m21 + m31,
	in.x * m02 + in.y * m12 + in.z * m22 + m32
	);
	
	fp w = in.x * m03 + in.y * m13 + in.z * m23 + m33;
	if (w > 0)
	{
		// normalize if w is different than 0 (convert from homogeneous to Cartesian coordinates)
		out.x /= w;
		out.y /= w;
		//out.z /= w;
	}
	return out;
}

mat4x4 mat4x4::rotate3d(vec3 axis, const fp& angle)
{
	axis.Normalize();
	fp sinr = (fp)sin(angle);
	fp cosr = (fp)cos(angle);
	fp mincos = 1 - cosr;
	//https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	return mat4x4
	{
		cosr + axis.x * axis.x * mincos,
		axis.x * axis.y * mincos - axis.z * sinr,
		axis.x * axis.z * mincos + axis.y * sinr,
		0,
		axis.y * axis.x * mincos + axis.z * sinr,
		cosr + axis.y * axis.y * mincos,
		axis.y * axis.z * mincos - axis.x * sinr,
		0,
		axis.z * axis.x * mincos - axis.y * sinr,
		axis.z * axis.y * mincos + axis.x * sinr,
		cosr + axis.z * axis.z * mincos,
		0,0,0,0,1
	};
}

mat4x4 mat4x4::scale4d(fp scalar)
{
	mat4x4 m = mat4x4();
	m.m00 = scalar;
	m.m11 = scalar;
	m.m22 = scalar;
	m.m33 = 1;
	return m;
}
mat4x4 mat4x4::translate3d(vec3 add)
{
	return mat4x4(1, 0,0, add.x, 0, 1,0, add.y, 0, 0, 1,add.z,0,0,0,1);
}
mat4x4 mat4x4::scale4d(fp scaleX, fp scaleY, fp scaleZ, fp scaleW)
{
	mat4x4 m = mat4x4();
	m.m00 = scaleX;
	m.m11 = scaleY;
	m.m22 = scaleZ;
	m.m33 = scaleW;
	return m;
}
mat4x4 mat4x4::scale3d(vec3 scale)
{
	mat4x4 m = mat4x4();
	m.m00 = scale.x;
	m.m11 = scale.y;
	m.m22 = scale.z;
	return m;
}

//glm method
//https://codereview.stackexchange.com/questions/186770/4%C3%974-matrix-multiplication
mat4x4 mat4x4::cross(const mat4x4& last, const mat4x4& first)
{
	mat4x4 mult = mat4x4(0);//all values are 0
	int index, i, j;
	for (i = 0; i < 4; ++i) for (j = 0; j < 4; ++j) for (index = 0; index < 4; ++index){
			mult.arr2d[i][index] += last.arr2d[j][index] * first.arr2d[i][j];
			//mult.arr2d[i][j] += last.arr2d[i][index] * first.arr2d[index][j];
	}
	return mult;
}

//glm method
mat4x4 mat4x4::perspectiveFov
(
	fp const& fov,
	fp const& width,
	fp const& height,
	fp const& zNear,
	fp const& zFar
)
{
	fp const rad = fov;
	fp const h = cos(static_cast<fp>(0.5) * rad) / sin(static_cast<fp>(0.5) * rad);
	fp const w = h * height / width; ///todo max(width , Height) / min(width , Height)?

	mat4x4 Result(static_cast<fp>(0));
	Result.m00 = w;//the width multiplier of the screen
	Result.m11 = h;//the height multiplier of the screen
	Result.m22 = -(zFar + zNear) / (zFar - zNear);//the z multiplier
	//to bring it in range(0,1):
	Result.m23 = -static_cast<fp>(1);//the distance multiplier
	Result.m32 = -(static_cast<fp>(2) * zFar * zNear) / (zFar - zNear);//the distance substractor
	//row followed by column
	return Result;
}


//the view matrix
//glm method
//https://stackoverflow.com/questions/19740463/lookat-function-im-going-crazy
//https://stackoverflow.com/questions/21830340/understanding-glmlookat
mat4x4 mat4x4::lookat(vec3 eye, vec3 center, vec3 const& up)
{
	//Create a new coordinate system
	vec3 screenz((center - eye).normalized());//the look direction
	vec3 screenx(vec3::cross(screenz, up).normalized());//the sideways direction
	vec3 screeny(vec3::cross(screenz, screenx));//the upwards direction

	mat4x4 Result(1);
	Result.m00 = screenx.x;//the new x
	Result.m10 = screenx.y;
	Result.m20 = screenx.z;
	Result.m01 = screeny.x;//the new y
	Result.m11 = screeny.y;
	Result.m21 = screeny.z;
	Result.m02 = -screenz.x;//the new z
	Result.m12 = -screenz.y;
	Result.m22 = -screenz.z;
	Result.m30 = -vec3::dot(screenx, eye);
	Result.m31 = -vec3::dot(screeny, eye);
	Result.m32 = vec3::dot(screenz, eye);
	return Result;
}

bool operator==(mat4x4 m1, mat4x4 m2)
{
	return
		m1.m00 == m2.m00 && m1.m10 == m2.m10 && m1.m20 == m2.m20 && m1.m30 == m2.m30 &&
		m1.m01 == m2.m01 && m1.m11 == m2.m11 && m1.m21 == m2.m21 && m1.m31 == m2.m31 &&
		m1.m02 == m2.m02 && m1.m12 == m2.m12 && m1.m22 == m2.m22 && m1.m32 == m2.m32 &&
		m1.m03 == m2.m03 && m1.m13 == m2.m13 && m1.m23 == m2.m23 && m1.m33 == m2.m33;
}