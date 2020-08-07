#include "tile.h"
#pragma once

template<typename t, typename vecn>
struct tileList:IDestructable
{
	typedef tile<t, vecn> listTile;
	//the tiles are sorted
	fastlist<listTile*>* tiles;

	//override
	listTile* (*addTile)(vecn position, fastlist<t>* elements);

	//WATCH OUT: shouldswap cant convert all listTiles into your derived ones!
	tileList(bool (*shouldSwap)(listTile* const& first, listTile* const& last), listTile* (*addTile)(vecn position, fastlist<t>* elements))
	{
		this->tiles = new fastlist<listTile*>();
		this->tiles->shouldSwap = shouldSwap;
		this->addTile = addTile;
	}
	//returns the tile this element is added to
	inline tile<t, vecn>* addElement(t value, vecn position, bool update = true)
	{
		//use a temporary tile to find out if there is already a tile or not
		tile<t, vecn>* currentTile = new listTile(position, nullptr);
		int index = tiles->findSortedUpperNeighbor(currentTile);
		currentTile->destruct();
		delete currentTile;
		tile<t, vecn>* closestTile;
		if (index >= 0)
		{
			closestTile = (*tiles)[index];
			if (closestTile->position == position)
			{
				goto addToTile;
			}
		}
		//add a tile
		currentTile = addTile(position, nullptr);
		tiles->insertAndUpdate(currentTile, index + 1);
		currentTile->elements = new fastlist<t>();
		closestTile = currentTile;
	addToTile:
		closestTile->elements->push_back(value);
		if (update) { 
			closestTile->elements->update(); 
		}
		return closestTile;
	}
	inline virtual void destruct() 
	{
		for (listTile* currentTile : *tiles) 
		{
			currentTile->destruct();
			delete currentTile;
		}
		tiles->destruct();
		delete tiles;
	}
};
