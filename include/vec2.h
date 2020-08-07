#include "GlobalFunctions.h"
#pragma once
template<typename t>
union vec2t
{
public:

	//variables
	struct {
		t x, y;
	};
	t axis[2];
	//constructors
	inline vec2t()
	{
		x = y = 0;
	}
	inline vec2t(const t& x, const t& y)
	{
		this->x = x;
		this->y = y;
	}
	inline vec2t(const t& scale)
	{
		x = y = scale;
	}
	template<typename t2>
	inline vec2t(const vec2t<t2> in):x((t)in.x),y((t)in.y){}
	//functions
	inline t lengthsquared() const
	{
		return x * x + y * y;
	}
	//sum of all axes
	inline t diagonallength() const
	{
		return x + y;
	}
	inline t length() const
	{
		return sqrt(lengthsquared());
	}
	inline vec2t rotatez(const t& angle) const
	{
		t cosa = cos(angle);
		t sina = sin(angle);
		return vec2t(x * cosa - y * sina, x * sina + y * cosa);
	}
	inline vec2t normalized() const
	{
		t l = length();
		return vec2t(x / l, y / l);
	}
	inline void normalize()
	{
		t l = length();
		x /= l;
		y /= l;
	}
	inline vec2t absolute() const
	{
		return vec2t(abs(x), abs(y));
	}
	static inline vec2t getrotatedvector(const t& yaw)
	{
		//rotate vector over y axis(pitch)
		t siny = sin(-yaw), cosy = cos(-yaw);
		return vec2t(cosy, siny);
	}
	//this vector has to be normalized!
	inline fp getRotation()
	{
		fp rotation = asin(x);
		if (y < 0) { rotation = math::PI - rotation; }
		if (rotation < 0) { rotation += math::PI2; }
		return rotation;
	}
	static inline vec2t nearest(const vec2t& point, std::vector<vec2t> others)
	{
		vec2t nearest = others[0];
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
	static inline t dot(const vec2t& lhs, const vec2t& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}
	//operators
	inline bool operator !=(const vec2t& v2) const
	{
		return x != v2.x || y != v2.y;
	}
	inline bool operator ==(const vec2t& v2) const
	{
		return x == v2.x && y == v2.y;
	}
	inline vec2t operator *(const t& scale) const
	{
		return vec2t(x * scale, y * scale);
	}
	inline vec2t operator *(const vec2t& scale) const
	{
		return vec2t(x * scale.x, y * scale.y);
	}
	inline vec2t operator /(const vec2t& scale) const
	{
		return vec2t(x / scale.x, y / scale.y);
	}
	inline friend vec2t operator *(const t& scale, const vec2t& v)
	{
		return vec2t(v.x * scale, v.y * scale);
	}
	inline friend vec2t operator /(const t& scale, const vec2t& v)
	{
		return vec2t(scale / v.x, scale / v.y);
	}
	inline void operator*=(const t& scale)
	{
		x *= scale;
		y *= scale;
	}
	inline vec2t operator /(const t& scale) const
	{
		return vec2t(x / scale, y / scale);
	}
	inline void operator /=(const t& scale)
	{
		x /= scale;
		y /= scale;
	}
	inline vec2t operator +(const vec2t& v2) const
	{
		return vec2t(x + v2.x, y + v2.y);
	}
	inline void operator+=(const vec2t& v2)
	{
		x += v2.x;
		y += v2.y;
	}
	inline void operator-=(const vec2t& v2)
	{
		x -= v2.x;
		y -= v2.y;
	}
	inline vec2t operator +(const t& additional) const
	{
		return vec2t(x + additional, y + additional);
	}
	inline vec2t operator -(const vec2t& v2) const
	{
		return vec2t(x - v2.x, y - v2.y);
	}
	inline vec2t operator -() const
	{
		return vec2t(-x, -y);
	}

};

//typedefs
typedef vec2t<fp> vec2;
typedef vec2t<int> vec2i;
typedef vec2t<long long> vec2ll;
typedef const vec2 cvec2;
typedef const vec2i cvec2i;
typedef const vec2ll cvec2ll;

//returns wether a point is inside an array from (0,0) to (size.x, size.y)
static inline bool inbounds(const vec2i& pos, const vec2i& size)
{
	return pos.x >= 0 && pos.x < size.x && pos.y >= 0 && pos.y < size.y;
}
