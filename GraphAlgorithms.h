#pragma once
#include <vector>
#include "Graph.h"
#include "DisjointSet.h"

namespace GraphAlgorithms
{
	/** Get Forest of MST (works on non fully connected graphs)

		Returned MSTs are represented by their edges. The given edges may form several MSTS when
		the given graph is not fully connected. 

		O(E logE)

		One way to know which edges belong to which tree is to use the extra optional parameter, that
		when passed return the disjoint set used, so you can call find(int v) from it passing either vertex 
		from an edge, and it will return the id of the MST it is in.

		@param graph - The graph to apply the algorithm on, it can be fully connected or not.
		@param usedSet - output parameter that will get the disjoint set used in the algorithm.
	*/
	std::vector<Graph::Edge> getKruskalMST(Graph& graph, DisjointSet* usedSet = nullptr);

	std::vector<Graph::Edge> getPrimMST(Graph& graph);

	bool isFullyConnected(Graph& graph);
}