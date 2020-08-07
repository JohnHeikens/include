#pragma once
#include "vec2.h"
template<typename t>
union rectangle2t
{
	struct {
		vec2t<t> pos00;
		vec2t<t> size;
	};
	struct {
		t x;
		t y;
		t w;
		t h;
	};
	inline rectangle2t():pos00(vec2()),size(vec2()){}
	inline rectangle2t(const vec2t<t> pos00, const vec2t<t> size) :pos00(pos00), size(size){}
	inline rectangle2t(const vec2t<t> size) : pos00(vec2()), size(size) {}
	inline rectangle2t(const t x, const t y, const t w, const t h) : x(x), y(y), w(w), h(h) {}

	template<typename t2>
	explicit inline rectangle2t(const rectangle2t<t2> in) : pos00((vec2t<t>)in.pos00), size((vec2t<t>)in.size) {}

	inline vec2t<t> pos11() const
	{
		return pos00 + size;
	}
	inline bool contains(const vec2t<t>& pos) const
	{
		if (pos.x >= pos00.x && pos.y >= pos00.y) 
		{
			vec2 br = pos11();
			return pos.x < br.x && pos.y < br.y;
		}
		else {
			return false;
		}

	}

	inline void expand(cfp& border) 
	{
		pos00 -= border;
		size += border * 2;
	}
	inline rectangle2t expanded(cfp& border)
	{
		rectangle2t r = *this;
		r.expand(border);
		return r;
	}
	//the rectangle will not exceed the borders; else it will have a negative size.
	inline void crop(const rectangle2t& borders)
	{
		if (pos00.x < borders.pos00.x)
		{
			size.x += pos00.x - borders.pos00.x;
			pos00.x = borders.pos00.x;
		}
		if (pos00.y < borders.pos00.y)
		{
			size.y += pos00.y - borders.pos00.y;
			pos00.y = borders.pos00.y;
		}
		if (pos00.x + size.x > borders.pos00.x + borders.size.x)
		{
			size.x = borders.pos00.x + borders.size.x - pos00.x;
		}
		if (pos00.y + size.y > borders.pos00.y + borders.size.y)
		{
			size.y = borders.pos00.y + borders.size.y - pos00.y;
		}
	}
	inline vec2t<t> centered(const vec2t<t>& innerRectSize) const
	{
		return vec2t<t>(
			x + ((w - innerRectSize.x) / 2), 
			y + ((h - innerRectSize.y) / 2)
		);
	}
	inline void moveToCenter(const rectangle2t& outerRect) 
	{
		pos00 = outerRect.centered(size);
	}
};
typedef rectangle2t<fp> rectangle2;
typedef rectangle2t<int> rectangle2i;
typedef const rectangle2 crectangle2;
typedef const rectangle2i crectangle2i;

inline rectangle2i floorRect(crectangle2& rect)
{
	cvec2i& pos00 = cvec2i(floor(rect.x), floor(rect.y));
	return rectangle2i(pos00, cvec2i(floor(rect.x + rect.w), floor(rect.y + rect.h)) - pos00);
}