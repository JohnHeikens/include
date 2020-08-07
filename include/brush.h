#include "mathfunctions.h"
#pragma once
struct brush
{
	virtual color getColor(cvec2& pos) const;
};