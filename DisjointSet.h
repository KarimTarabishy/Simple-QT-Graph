#pragma once
#include <vector>

class DisjointSet
{
	std::vector<int> id;		//hold set id
	std::vector<int> rank;		//hold set rank
	std::vector<int> flatten_holder;  //hold items that need to be flattened

public:
	/** Initialize a disjoint set with given number of componenets.

		Each set is represented by a number, so first we will have setCount
		sets having id's from 0 to setCount-1.

		@param setCount - number of sets available
	*/
	explicit DisjointSet(int setCount);

	/** Create an empty instance.
	
		Only use when you will assign another Disjointset object to this instance.
	*/
	DisjointSet(){}

	DisjointSet(DisjointSet&& rvalue);
	DisjointSet& operator=(DisjointSet&& rvalue);

	int  find(int v);
	void makeUnion(int v, int u);
	int getSetCount();

};

