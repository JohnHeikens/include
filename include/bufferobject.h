#include "GlobalFunctions.h"
#pragma once
template<typename t>
struct bufferobject
{
	const t* buffer;
	int size;//the length of the buffer in t's
	int stride;//the step to take in t's
	int stepcount;//the steps to make
	/*void Resize(int newsize, const bool keep = false)
	{
		t* newbuffer = new t[newsize];
		if (keep)
		{
			memcpy(newbuffer, buffer, sizeof(t) * size);
		}
		size = newsize;
		delete[] buffer;
	}*/
	//bufferobject(const std::initializer_list<const t> elements, cint stride = 1, cint stepcount = 0);
	bufferobject(const t* elements, cint elementcount, cint stride = 1, cint stepcount = 0);
	const t& operator[](cint& index) const
	{
		return *(buffer + index * stride);
	}
};

/*template<typename t>
inline bufferobject<t>::bufferobject(const std::initializer_list<const t> elements, cint stride, cint stepcount)
{
	this->stepcount = stepcount;
	this->stride = stride;
	buffer = new t[this->size = elements.size()];
	std::copy(elements.begin(), elements.end(), buffer);
}*/

template<typename t>
inline bufferobject<t>::bufferobject(const t* elements, cint elementcount, cint stride, cint stepcount)
{
	this->stepcount = stepcount;
	this->stride = stride;
	this->size = elementcount;
	buffer = elements;
	/*buffer = new t[this->size = elementcount];
	std::copy(elements, elements + elementcount, buffer);*/
}