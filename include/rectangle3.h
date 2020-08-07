#pragma once
#include "rectangle2.h"
#include "vec3.h"
template<typename t>
union rectangle3t
{
	struct {
		vec3t<t> pos000;
		vec3t<t> size;
	};
	struct {
		t x;
		t y;
		t z;
		t w;//width
		t h;//height
		t d;//depth
	};
	inline rectangle3t() :pos000(vec3()), size(vec3()) {}
	inline rectangle3t(const vec3t<t> pos000, const vec3t<t> size) : pos000(pos000), size(size) {}
	inline rectangle3t(const vec3t<t> size) : pos000(vec3()), size(size) {}
	inline rectangle3t(const t x, const t y, const t z, const t w, const t h, const t d) : x(x), y(y), z(z), w(w), h(h), d(d) {}

	template<typename t2>
	explicit inline rectangle3t(const rectangle3t<t2> in) : pos000((vec3t<t>)in.pos000), size((vec3t<t>)in.size) {}

	inline vec3t<t> pos111() const
	{
		return pos000 + size;
	}
	inline bool contains(const vec3t<t>& pos) const
	{
		if (pos.x >= pos000.x && pos.y >= pos000.y && pos.z >= pos000.z)
		{
			vec3 br = pos111();
			return pos.x < br.x&& pos.y < br.y && pos.z < br.z;
		}
		else {
			return false;
		}

	}

	inline void expand(cfp& border)
	{
		pos000 -= border;
		size += border * 2;
	}
	inline rectangle3t expanded(cfp& border)
	{
		rectangle3t r = *this;
		r.expand(border);
		return r;
	}
	//the rectangle will not exceed the borders; else it will have a negative size.
	inline void crop(const rectangle3t& borders)
	{
		if (pos000.x < borders.pos000.x)
		{
			size.x += pos000.x - borders.pos000.x;
			pos000.x = borders.pos000.x;
		}
		if (pos000.y < borders.pos000.y)
		{
			size.y += pos000.y - borders.pos000.y;
			pos000.y = borders.pos000.y;
		}
		if (pos000.z < borders.pos000.z)
		{
			size.z += pos000.z - borders.pos000.z;
			pos000.z = borders.pos000.z;
		}
		if (pos000.x + size.x > borders.pos000.x + borders.size.x)
		{
			size.x = borders.pos000.x + borders.size.x - pos000.x;
		}
		if (pos000.y + size.y > borders.pos000.y + borders.size.y)
		{
			size.y = borders.pos000.y + borders.size.y - pos000.y;
		}
		if (pos000.z + size.z > borders.pos000.z + borders.size.z)
		{
			size.z = borders.pos000.z + borders.size.z - pos000.z;
		}
	}
	inline vec3t<t> centered(const vec3t<t>& innerRectSize) const
	{
		return vec3t<t>(
			x + ((w - innerRectSize.x) / 2),
			y + ((h - innerRectSize.x) / 2),
			z + ((d - innerRectSize.y) / 2)
			);
	}
	inline void moveToCenter(const rectangle3t& outerRect)
	{
		pos000 = outerRect.centered(size);
	}
};
typedef rectangle3t<fp> rectangle3;
typedef rectangle3t<int> rectangle3i;
typedef const rectangle3 crectangle3;
typedef const rectangle3i crectangle3i;