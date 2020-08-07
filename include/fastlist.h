#pragma once
#include "idestructable.h"
//super fast solution for iterating through a list while deleting certain items and adding items
constexpr bool keepindicator = true;
constexpr bool eraseindicator = !keepindicator;
template<typename t>
struct fastlist:IDestructable
{
	bool* mask;
	t* basearray;
	int size;
	int newsize;
	std::vector<t> addlist;
	
	
	inline fastlist(const int& size = 0)
		:size(size),
		mask(nullptr),
		basearray(new t[size]),
		addlist(std::vector<t>()),
		newsize(size)
	{
		
	}

	
	//for iterative functions
	inline t* begin() 
	{
		return basearray;
	}
	inline t* end() 
	{
		return basearray + size;
	}

	//refresh the list (add and remove selected items)
	inline void update() 
	{
		//delete according to the mask: false = do not delete, true = delete
		t* newarray = new t[newsize];
		int oldindex = 0, newindex = 0;
		//iterate through old array
		for (int oldindex = 0; oldindex < size; oldindex++) 
		{
			if (!mask || mask[oldindex]) 
			{
				newarray[newindex] = basearray[oldindex];
				newindex++;
			}//else skip
		}
		if (addlist.size()) {
			//add the 'addlist' array
			memcpy(&newarray[newindex], &addlist[0], addlist.size() * sizeof(t));
		}
		//update variables
		size = newsize;
		if (mask) 
		{
			delete[] mask;
		}
		delete[] basearray;
		addlist.clear();
		basearray = newarray;
		mask = nullptr;
	}
	inline void generateMask() 
	{
		mask = new bool[size];
		std::fill(mask, mask + size, keepindicator);
	}
	//DO NOT ERASE THE SAME INDEX TWICE
	inline void erase(const int& index) 
	{
		if (!mask) 
		{
			generateMask();
		}
		mask[index] = eraseindicator;
		newsize--;
	}
	//find the index of an element
	inline int find(const t& element) const
	{
		for (int index = 0; index < size; index++)
		{
			if (basearray[index] == element)return index;
		}
		return -1;
	}

	bool (*shouldSwap)(const t& first, const t& last);
	//for example if the list contains 3,5,6 and you search for 4 it will return 0(position of 3)
	inline int findSortedUpperNeighbor(const t& value)
	{
		//play the guess game
		int minNeighbor = 0;
		int maxNeighbor = size - 1;

		if (size == 0 || shouldSwap(basearray[0], value))
		{
			return -1;
		}
		while (minNeighbor < maxNeighbor) 
		{
			int mid = (maxNeighbor - (minNeighbor + 1)) / 2 + (minNeighbor + 1);
			const t midValue = basearray[mid];
			if (shouldSwap(value, midValue))
			{
				//element > mid
				minNeighbor = mid;
			}
			else if (!shouldSwap(midValue, value))
			{
				//mid == element
				return mid;
			}
			else
			{
				//mid > element
				maxNeighbor = mid - 1;
			}
		}
		return minNeighbor;
	}
	//this is slow
	inline void insertAndUpdate(const t& element, int index) 
	{
		//mask is null, that will continue to be so
		update();

		newsize = size + 1;
		t* newarray = new t[newsize];
		//0 to index: old 0 to index
		std::copy(basearray, basearray + index, newarray);
		//index: new value
		newarray[index] = element;
		//index + 1 to size + 1: old index to size
		std::copy(basearray + index, basearray + size, newarray + index + 1);
		delete[] basearray;
		basearray = newarray;
		size = newsize;
	}
	inline void insertSorted(const t& value)
	{
		int neighborIndex = findSortedUpperNeighbor(value);
		insertAndUpdate(value, neighborIndex + 1);
	}
	inline void push_back(const t& element)
	{
		addlist.push_back(element);
		newsize++;
	}
	inline t& operator[](cint& index) const
	{
		return basearray[index];
	}
	//source:
	//https://www.geeksforgeeks.org/cpp-program-for-quicksort/
	/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
	array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
	//so low to high
	//should swap:
	//from low to high: return first > last

	int partition(cint& low, cint& high)
	{
		const t pivot = basearray[high];    // pivot 
		int i = (low - 1);  // Index of smaller element 

		for (int j = low; j <= high - 1; j++)
		{
			// If current element is smaller than or 
			// equal to pivot 
			if (shouldSwap(pivot, basearray[j]))//basearray[j] <= pivot -> pivot > basearray[j]  
			{
				i++;    // increment index of smaller element 
				std::swap(basearray[i], basearray[j]);
			}
		}
		std::swap(basearray[i + 1], basearray[high]);
		return (i + 1);
	}

	/* The main function that implements QuickSort
	  low  --> Starting index,
	  high  --> Ending index */
	void quickSort(cint& low, cint& high)
	{
		if (low < high)
		{
			/* pi is partitioning index, basearray[p] is now
			   at right place */
			cint pi = partition(low, high);

			// Separately sort elements before 
			// partition and after partition 
			quickSort(low, pi - 1);
			quickSort( pi + 1, high);
		}
	}
	inline void destruct() override
	{
		delete[] mask;
		delete[] basearray;
		//dont delete the elements!
	}
};