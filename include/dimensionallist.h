/*#include "dimensionalindex.h"
#pragma once

template<typename t, int Dimensioncount>
struct dimensionallist:Idimensional<t, Dimensioncount>
{
	fastlist<Idimensional<t, Dimensioncount - 1>> baselists;
	inline t GetElementAtIndex(dimensionalindex<Dimensioncount> index) const override
	{
		cint location = index.locations[Dimensioncount - 1];
		for (int i = 0; i < baselists.size;i++)
		{
			Idimensional<t, Dimensioncount - 1> l = baselists[i];
			if (l.index == location)
			{
				return l.GetElementAtIndex(dimensionalindex<Dimensioncount - 1>::fromPointer(index.locations));
			}
		}
	}
};
struct onedimensionallist 
{

};*/