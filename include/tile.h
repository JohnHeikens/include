#pragma once
#include "fastlist.h"
template<typename t, typename vecn>
struct tile:IDestructable
{
	vecn position = vec2();
	//the elements in this tile
	fastlist<t>* elements = nullptr;
	tile(vecn position, fastlist<t>* elements) 
	{
		this->position = position;
		this->elements = elements;
	}
	virtual void destruct() override 
	{
		if (elements) 
		{
			elements->destruct();
		}
		delete elements;
	}
};