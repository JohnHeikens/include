#pragma once
#include "GlobalFunctions.h"
//CAUTION! WILL NOT DELETE ITS ARRAY UPON DESTRUCTION!
template<typename t, cint width, cint height>
struct matnxn
{
	cint width, height;
	t* values;
	inline matnxn(cint width, cint height) :width(width), height(height)
	{
		values = new t[width * height];
	}
	inline matnxn(cint width, cint height, const t fillWith) : width(width), height(height)
	{
		values = new t[width * height];
		std::fill(values, values + width * height, fillWith);
	}
	//index: x + y * width

};