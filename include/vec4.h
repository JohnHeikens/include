#pragma once
#include "vec3.h"
template<typename t>
union vec4t
{
public:
	//variables
	struct {
		t x, y, z, w;
	};
	struct {
		t b, g, r, a;
	};
	t axis[4];
	//constructors
	inline vec4t()
	{
		x = y = z = 0;
	}
	inline vec4t(const t& x, const t& y, const t& z, const t& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	inline vec4t(const t& x, const t& y, const t& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 1;
	}
	inline vec4t(const vec3t<t>& xyz)
	{
		this->x = xyz.x;
		this->y = xyz.y;
		this->z = xyz.z;
		this->w = 1;
	}
	inline vec4t(const vec3t<t>& xyz, const t& w)
	{
		this->x = xyz.x;
		this->y = xyz.y;
		this->z = xyz.z;
		this->w = 1;
	}
	inline vec4t(const vec2t<t>& xy, const t& z)
	{
		this->x = xy.x;
		this->y = xy.y;
		this->z = z;
		this->w = 1;
	}
	inline vec4t(const t& x, const t& y)
	{
		this->x = x;
		this->y = y;
		this->z = 0;
		this->w = 1;
	}
	inline vec4t(const t& scale)
	{
		x = y = z = scale;
		w = 1;
	}
	//functions
	inline t lengthsquared() const
	{
		return x * x + y * y + z * z;
	}
	//sum of all axes
	inline t diagonallength() const
	{
		return x + y + z;
	}
	inline t length() const
	{
		return sqrt(lengthsquared());
	}
	inline vec2 Get2d() const
	{
		return vec2(x, y);
	}
	inline vec3 Get3d() const
	{
		return vec3(x, y, z);
	}
	inline vec4t rotatez(const t& angle) const
	{
		t cosa = cos(angle);
		t sina = sin(angle);
		return vec4t(x * cosa - y * sina, x * sina + y * cosa, z);
	}
	inline vec4t normalized() const
	{
		t l = length();
		return vec4t(x / l, y / l, z / l);
	}
	inline void Normalize()
	{
		t l = length();
		x /= l;
		y /= l;
		z /= l;
	}
	inline vec4t absolute() const
	{
		return vec4t(abs(x), abs(y), abs(z));
	}
	static inline vec4t getrotatedvector(const t& pitch, const t& yaw)
	{
		//rotate vector over y axis(pitch)
		t cosp = cos(-pitch), sinp = sin(-pitch), siny = sin(-yaw), cosy = cos(-yaw);
		return vec4t(cosy * cosp, siny * cosp, sinp);
	}
	static inline vec4t cross(const vec4t& lhs, const vec4t& rhs)
	{
		return vec4t(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);

	}
	static inline vec4t Vector_IntersectPlane(const vec4t& plane_p, const vec4t& plane_n, const vec4t& lineStart, const vec4t& lineEnd, t& w)
	{
		//plane_n.Normalize();
		t plane_d = -dot(plane_n, plane_p);
		t ad = dot(lineStart, plane_n);
		t bd = dot(lineEnd, plane_n);
		w = (-plane_d - ad) / (bd - ad);
		return lerp<vec4t>(lineStart, lineEnd, w);
	}
	static inline vec4t nearest(const vec4t& point, std::vector<vec4t> others)
	{
		vec4t nearest = others[0];
		t d = (point - nearest).lengthsquared();
		cint size = others.size();
		for (int i = 0; i < size; i++)
		{
			t currentdistance = (point - others[i]).lengthsquared();
			if (currentdistance < d)
			{
				d = currentdistance;
				nearest = others[i];
			}
		}
		return nearest;
	}
	//axis has to be normalized
	static inline vec4t rotate(const vec4t vec, const vec4t axis, const t angle)
	{
		//axis.Normalize();
		const vec4t cross = cross(axis, vec);
		const t sina = sin(angle);
		const t cosa = cos(angle);
		return cosa * vec + sina * cross;
	}
	static inline t dot(const vec4t& lhs, const vec4t& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	//operators
	inline vec4t operator !=(const vec4t& v2) const
	{
		return x != v2.x || y != v2.y || z != v2.z;
	}
	inline bool operator ==(const vec4t& v2) const
	{
		return x == v2.x && y == v2.y && z == v2.z;
	}
	inline vec4t operator *(const t& scale) const
	{
		return vec4t(x * scale, y * scale, z * scale);
	}
	inline friend vec4t operator *(const t& scale, const vec4t& v)
	{
		return vec4t(v.x * scale, v.y * scale, v.z * scale);
	}
	inline friend vec4t operator /(const t& scale, const vec4t& v)
	{
		return vec4t(scale / v.x, scale / v.y, scale / v.z);
	}
	inline void operator*=(const t& scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
	}
	inline vec4t operator /(const t& scale) const
	{
		return vec4t(x / scale, y / scale, z / scale);
	}
	inline void operator /=(const t& scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
	}
	inline vec4t operator +(const vec4t& v2) const
	{
		return vec4t(x + v2.x, y + v2.y, z + v2.z);
	}
	inline void operator+=(const vec4t& v2)
	{
		x += v2.x;
		y += v2.y;
		z += v2.z;
	}
	inline void operator-=(const vec4t& v2)
	{
		x -= v2.x;
		y -= v2.y;
		z -= v2.z;
	}
	inline vec4t operator +(const t& additional) const
	{
		return vec4t(x + additional, y + additional, z + additional);
	}
	inline vec4t operator -(const vec4t& v2) const
	{
		return vec4t(x - v2.x, y - v2.y, z - v2.z);
	}
	inline vec4t operator -() const
	{
		return vec4t(-x, -y, -z);
	}

};

//typedefs
typedef vec4t<fp> vec4;
typedef vec4t<int> vec4i;

namespace zup4d
{
	const vec4 up = vec4(0, 0, 1),
		down = vec4(0, 0, -1),
		left = vec4(-1, 0, 0),
		right = vec4(1, 0, 0),
		forward = vec4(0, 1, 0),
		back = vec4(0, -1, 0);
	const vec4 directions[6]
	{
		up,down,left,right,forward,back
	};
}