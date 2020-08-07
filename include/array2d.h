#pragma once
#include "mathfunctions.h"

template<typename t>
struct array2d 
{
	int w, l;
	t* basearray;
	array2d(cint& w, cint& l, t* basearray) :w(w), l(l), basearray(basearray) {}
	array2d(cint& w, cint& l, bool initializeToDefault):w(w), l(l),basearray(initializeToDefault? new t[w * l](): new t[w * l]) {}
	void fillCircleFast(cfp& x, cfp& y, cfp& w, cfp& h, const t& value) const;
	void fillCircle(cfp& x, cfp& y, cfp& w, cfp& h, const t& value) const;
	void fillCircle(cvec2& pos, cvec2& size, const t& value) const;
	inline void fill(cint& x, cint& y, const t& value) const
	{
		fill(cvec2i(x, y), value);
	}
	inline void fillUnsafe(cint& x, cint& y, const t& value) const
	{
		basearray[x + y * w] = value;
	}
	inline void fillUnsafe(cvec2i& pos, const t& value) const
	{
		fillUnsafe(pos.x, pos.y, value);
	}
	inline void fill(cvec2i& pos, const t& value) const
	{
		if (rectangle2i(0, 0, w, l).contains(pos))
		{
			basearray[pos.x + pos.y * w] = value;
		}
	}
	inline bool inBounds(cvec2i& pos) const
	{
		return inBounds(pos.x,pos.y);
	}
	inline bool inBounds(int x, int y) const
	{
		return x >= 0 && x < w && y >= 0 && y < l;
	}
	void floodFill(cvec2i& pos, const t& value);
	inline t getValue(cint x, cint y) const
	{
		return inBounds(x, y) ? getValueUnsafe(x, y) : t();
	}
	inline t getValueUnsafe(cint x, cint y) const
	{
		return basearray[x + y * w];
	}
	inline t getValue(cvec2i& pos) const
	{
		return getValue(pos.x, pos.y);
	}
	inline t getValueUnsafe(cvec2i& pos) const
	{
		return getValueUnsafe(pos.x, pos.y);
	}
	inline t& operator[](cint& index) const
	{
		return basearray[index];
	}
};
//x, y: pos00 position
//w, h: width, height
template<typename t>
inline void array2d<t>::fillCircle(cfp& x, cfp& y, cfp& w, cfp& h, const t& value) const
{
	int MinY = ceil(y);
	int MaxY = floor(y + h) + 1;//+1 for also filling the last pixel
	//crop
	if (MinY < 0)MinY = 0;

	if (MaxY > this->l)MaxY = this->l;
	
	fp midx = x + w * .5;
	fp midy = y + h * .5;
	fp multx = 2.0 / w;//1 / (midx - x);//multipliers
	fp multy = 2.0 / h;

	int r = 50; // radius

	for (int j = MinY; j < MaxY; j++)
	{
		//circle equation:
		//x * x + y * y = r * r
		//ellipse equation:
		//(dx * multx)^2 + (dy*multy)^2 = 1
		//(dx * multx)^2 = 1 - (dy * multy) ^2
		//dx * multx = sqrt(1 - (dy * multy) ^ 2)
		//dx = sqrt(1 - (dy * multy) ^2) / multx
		fp val = 1 - math::squared((midy - j) * multy);
		if (val > 0) {
			const fp dx = sqrt(val) / multx;
			
			//0.5 to 1.5:
			//only fill two pixels
			int minX = ceil(midx - dx);
			int maxX = floor(midx + dx) + 1;//+1 for also filling the last pixel
			if (minX < 0)minX = 0;
			if (maxX > this->w)maxX = this->w;

			if (maxX - minX > 10)
			{
				val = val + 1;
			}
			t* ptr = basearray + j * this->w + minX;
			for (int i = minX; i < maxX; i++, ptr++)
			{

				*ptr = value;
			}
		}
	}
}

template<typename t>
inline void array2d<t>::fillCircle(cvec2& pos, cvec2& size, const t& value) const
{
	fillCircle(pos.x, pos.y, size.x, size.y, value);
}



template<typename t>
inline void array2d<t>::floodFill(cvec2i& pos, const t& value)
{
	//	1. If target is equal to replacement, return.
	t target = getValue(pos);
	if (target == value)return;
	//3. Set the value of node to replacement.
	this->fill(pos, value);
	//4. Set Q to the empty queue.
	std::list<vec2i> Q = std::list<vec2i>();
	//5. Add node to the end of Q.
	Q.push_back(pos);
	//6. While Q is not empty:
	while (Q.size() > 0)
	{
		//7. Set n equal to the first element of Q.
		vec2i n = *Q.begin();
		//8.     Remove first element from Q.
		Q.pop_front();
		//9.     If the value of the node to the west of n is target,
		if (n.x > 0 && getValue(n.x - 1, n.y) == target) {
			//set the value of that node to replacement and add that node to the end of Q.
			fill(n.x - 1, n.y, value);
			Q.push_back(vec2i( n.x - 1,n.y ));
		}
		//10.     If the value of the node to the east of n is target,
		if (n.x < w - 1 && getValue(n.x + 1, n.y) == target) {
			//set the value of that node to replacement and add that node to the end of Q.
			fill(n.x + 1, n.y, value);
			Q.push_back(vec2i( n.x + 1,n.y ));
		}
		//11.     If the value of the node to the north of n is target,
		if (n.y > 0 && getValue(n.x, n.y - 1) == target) {
			//set the value of that node to replacement and add that node to the end of Q.
			fill(n.x, n.y - 1, value);
			Q.push_back(vec2i( n.x,n.y - 1 ));
		}
		//12.     If the value of the node to the south of n is target,
		if (n.y < l - 1 && getValue(n.x, n.y + 1) == target) {
			//set the value of that node to replacement and add that node to the end of Q.
			fill(n.x, n.y + 1, value);
			Q.push_back(vec2i( n.x ,n.y + 1 ));
		}
		//13. Continue looping until Q is exhausted.
	}
	//14. Return.
}


