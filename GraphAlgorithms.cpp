#include "GraphAlgorithms.h"
#include "DisjointSet.h"
#include <queue>
#include <stack>

std::vector<Graph::Edge> GraphAlgorithms::getKruskalMST(Graph& graph, DisjointSet* usedSet)
{
	std::vector<Graph::Edge> msts;
	DisjointSet disjointSet(graph.getVertexCount());
	Graph::Edge::edge_set_ptr sortedEdges = graph.getSortedEdges();

	for (auto it : sortedEdges)
	{
		//if the 2 vertices are not already connected
		if (disjointSet.find(it->getVertex1()) != disjointSet.find(it->getVertex2()))
		{
			//add edge to mst
			msts.push_back(*it);
			//adust the set
			disjointSet.makeUnion(it->getVertex1(), it->getVertex2());
		}
	}

	//move to output
	if (usedSet != nullptr)
	{
		*usedSet = std::move(disjointSet);
	}
	return msts;
}

std::vector<Graph::Edge> GraphAlgorithms::getPrimMST(Graph& graph)
{
	std::vector < Graph::Edge > mst;
	std::vector<unsigned char> marked(graph.getVertexCount(), 0);
	std::priority_queue<Graph::Edge*, std::vector<Graph::Edge*>, Graph::Edge::PTR_COMPARE_GREATER> queue;
	//start from 0
	int current_vertex = 0;
	//indicate it is in the mst
	marked[current_vertex] = 1;
	do
	{
		//append new  connection from mst graph cut to the other graph cut
		for (auto& edge : graph.getAdjacent(current_vertex))
		{
			//only append if not already exist in mst
			if (!marked[edge.other(current_vertex)])
				queue.push(const_cast<Graph::Edge*>(&edge));
		}

		//get minimum connection
		Graph::Edge* minimum_edge;
		do
		{
			if (queue.empty())
				return mst;
			minimum_edge = queue.top();
			//remove it
			queue.pop();
		} while (marked[minimum_edge->getVertex1()] && marked[minimum_edge->getVertex2()]);


		//add to mst
		mst.push_back(*minimum_edge);
		//one of those vertex must be marked
		int next_vertex = marked[minimum_edge->getVertex1()] ? minimum_edge->getVertex2() : minimum_edge->getVertex1();
		//indicate the new vertex is in mst
		marked[next_vertex] = 1;

		//add the new connection from this new vertex  to the other graph cut
		current_vertex = next_vertex;

	} while (mst.size() != graph.getVertexCount()-1);


	return mst;
}

bool GraphAlgorithms::isFullyConnected(Graph& graph)
{
	std::vector<unsigned char> reached(graph.getVertexCount(), false);
	int visitedCount = 0;
	std::stack<int> st;

	st.push(0);
	visitedCount++;
	reached[0] = true;

	while (!st.empty())
	{
		int current_vertex = st.top();
		st.pop();
		for (auto& edge:graph.getAdjacent(current_vertex))
		{
			int other_vertex = edge.other(current_vertex);
			if (!reached[other_vertex])
			{
				visitedCount++;
				st.push(other_vertex);
				reached[other_vertex] = true;
			}
		}
	}

	return visitedCount == graph.getVertexCount();
}