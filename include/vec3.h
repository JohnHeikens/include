#pragma once
#include "vec2.h"
template<typename t>
union vec3t
{
public:
	//variables
	struct {
		t x, y, z;
	};
	struct {
		t b, g, r;
	};
	struct {
		t h;
		t s;
		t v;//the value of the brightest channel
	};
	t axis[3];
	//constructors
	inline vec3t()
	{
		x = y = z = 0;
	}
	inline vec3t(const t& x, const t& y, const t& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	inline vec3t(const vec2& xy, const t& z)
	{
		this->x = xy.x;
		this->y = xy.y;
		this->z = z;
	}
	inline vec3t(const t& x, const t& y)
	{
		this->x = x;
		this->y = y;
		this->z = 0;
	}
	inline vec3t(const t& scale)
	{
		x = y = z = scale;
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
	inline vec2t<t> Get2d() const
	{
		return vec2t<t>(x, y);
	}
	inline vec3t rotatez(const t& angle) const
	{
		t cosa = cos(angle);
		t sina = sin(angle);
		return vec3t(x * cosa - y * sina, x * sina + y * cosa, z);
	}
	inline vec3t normalized() const
	{
		t l = length();
		return vec3t(x / l, y / l, z / l);
	}
	inline void Normalize()
	{
		t l = length();
		x /= l;
		y /= l;
		z /= l;
	}
	inline vec3t absolute() const
	{
		return vec3t(abs(x), abs(y), abs(z));
	}
	static inline vec3t getrotatedvector(const t& pitch, const t& yaw)
	{
		//rotate vector over y axis(pitch)
		t cosp = cos(-pitch), sinp = sin(-pitch), siny = sin(-yaw), cosy = cos(-yaw);
		return vec3t(cosy * cosp, siny * cosp, sinp);
	}
	//right handed, so cross(forward, up) will give right
	static inline vec3t cross(const vec3t& lhs, const vec3t& rhs)
	{
		return vec3t(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);

	}
	static inline vec3t Vector_IntersectPlane(const vec3t& plane_p, const vec3t& plane_n, const vec3t& lineStart, const vec3t& lineEnd, t& w)
	{
		//plane_n.Normalize();
		t plane_d = -dot(plane_n, plane_p);
		t ad = dot(lineStart, plane_n);
		t bd = dot(lineEnd, plane_n);
		w = (-plane_d - ad) / (bd - ad);
		return lerp<vec3t>(lineStart, lineEnd, w);
	}
	static inline vec3t nearest(const vec3t& point, std::vector<vec3t> others)
	{
		vec3t nearest = others[0];
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
	static inline vec3t rotate(const vec3t vec, const vec3t axis, const t angle)
	{
		//axis.Normalize();
		const vec3t crossProduct = cross(axis, vec);
		const t sina = sin(angle);
		const t cosa = cos(angle);
		return cosa * vec + sina * crossProduct;
	}
	static inline t dot(const vec3t& lhs, const vec3t& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	//operators
	inline bool operator !=(const vec3t& v2) const
	{
		return x != v2.x || y != v2.y || z != v2.z;
	}
	inline bool operator ==(const vec3t& v2) const
	{
		return x == v2.x && y == v2.y && z == v2.z;
	}
	inline vec3t operator *(const vec3t& scale) const
	{
		return vec3t(x * scale.x, y * scale.y, z * scale.z);
	}
	inline vec3t operator *(const t& scale) const
	{
		return vec3t(x * scale, y * scale, z * scale);
	}
	inline vec3t operator /(const vec3t& scale) const
	{
		return vec3t(x / scale.x, y / scale.y, z / scale.z);
	}

	inline friend vec3t operator *(const t& scale, const vec3t& v)
	{
		return vec3t(v.x * scale, v.y * scale, v.z * scale);
	}
	inline friend vec3t operator /(const t& scale, const vec3t& v)
	{
		return vec3t(scale / v.x, scale / v.y, scale / v.z);
	}
	inline void operator*=(const t& scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
	}
	inline void operator*=(const vec3t<t>& scale)
	{
		x *= scale.x;
		y *= scale.y;
		z *= scale.z;
	}
	inline void operator /=(const vec3t<t>& scale)
	{
		x /= scale.x;
		y /= scale.y;
		z /= scale.z;
	}
	inline vec3t operator /(const t& scale) const
	{
		return vec3t(x / scale, y / scale, z / scale);
	}
	inline void operator /=(const t& scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
	}
	inline vec3t operator +(const vec3t& v2) const
	{
		return vec3t(x + v2.x, y + v2.y, z + v2.z);
	}
	inline void operator+=(const vec3t& v2)
	{
		x += v2.x;
		y += v2.y;
		z += v2.z;
	}
	inline void operator-=(const vec3t& v2)
	{
		x -= v2.x;
		y -= v2.y;
		z -= v2.z;
	}
	inline vec3t operator +(const t& additional) const
	{
		return vec3t(x + additional, y + additional, z + additional);
	}
	inline vec3t operator -(const vec3t& v2) const
	{
		return vec3t(x - v2.x, y - v2.y, z - v2.z);
	}
	inline vec3t operator -() const
	{
		return vec3t(-x, -y, -z);
	}
};

//typedefs
typedef vec3t<fp> vec3;
typedef vec3t<int> vec3i;
typedef const vec3 cvec3;

inline color vectocolor(const vec3 v) { 
	return color(v.r, v.g, v.b); 
}
inline vec3 colortovec(const color c) { 
	vec3 v;
	v.r = c.r / (fp)0xff;
	v.g = c.g / (fp)0xff;
	v.b = c.b / (fp)0xff;
	return v;
}

namespace zup
{
	const vec3 up = vec3(0, 0, 1), 
		down = vec3(0, 0, -1), 
		left = vec3(-1, 0, 0), 
		right = vec3(1, 0, 0), 
		forward = vec3(0, 1, 0), 
		back = vec3(0, -1, 0);
	const vec3 directions[6]
	{
		up,down,left,right,forward,back
	};
}
//returns "x, y, z"
inline std::wstring towstring(const vec3 value, cint digits)
{
	return
		towstring(value.x, digits) + L", " +
		towstring(value.y, digits) + L", " +
		towstring(value.z, digits);
};