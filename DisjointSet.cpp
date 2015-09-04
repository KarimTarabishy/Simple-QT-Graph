#include "DisjointSet.h"
#include <cmath>

DisjointSet::DisjointSet(int setCount):
rank(setCount, 0),
flatten_holder(static_cast<int>(log2(setCount+1)), 0)
{
	
	//initialize set_id
	for (int i = 0; i < setCount; i++)
	{
		id.push_back(i);
	}
}

DisjointSet::DisjointSet(DisjointSet&& rvalue)
{
	id = std::move(rvalue.id);
	rank = std::move(rvalue.rank);
	flatten_holder = std::move(rvalue.flatten_holder);
}

DisjointSet& DisjointSet::operator=(DisjointSet&& rvalue)
{
	id = std::move(rvalue.id);
	rank = std::move(rvalue.rank);
	flatten_holder = std::move(rvalue.flatten_holder);

	return *this;
}

int DisjointSet::find(int v)
{
	int flatten_index = 0;
	int current_id = v;
	//loop untill we are at a root node which is the id of the set
	while (id[current_id] != current_id)
	{
		//add to flatten
		flatten_holder[flatten_index++] = current_id;
		//jump to parent
		current_id = id[current_id];
	}

	//flatten 
	for (int i = 0; i < flatten_index; i++)
	{
		id[flatten_holder[i]] = current_id;
	}
	return current_id;
}

void DisjointSet::makeUnion(int v, int u)
{
	//get the sets id first
	int v_set_id = find(v), u_set_id = find(u);

	//if in same set return
	if (v_set_id == u_set_id)
		return;

	//merge the smaller tree to the larger one
	if (rank[v_set_id] > rank[u_set_id])
	{
		//merge smaller set u to larger set v
		id[u_set_id] = v_set_id;
	}
	else if (rank[v_set_id] < rank[u_set_id])
	{
		//merge smaller set v to larger set u
		id[v_set_id] = u_set_id;
	}
	else 
	{
		//they are equal so merge u to v
		id[u_set_id] = v_set_id;
		//increment v's rank
		rank[v_set_id]++;
	}
}

int DisjointSet::getSetCount()
{
	int found = 0;
	for (int i = 0; i < id.size(); i++)
	{
		if (i == id[i]) found++;
	}
	return found;
}

