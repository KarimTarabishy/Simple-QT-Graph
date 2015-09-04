#include "Graph.h"
#include <string>



bool Graph::Edge::PTR_COMPARE::operator()(const Edge* a, const Edge* b)
{
		return *a < *b;
}

bool Graph::Edge::PTR_COMPARE_GREATER::operator()(const Edge* a, const Edge* b)
{
	return !(*a < *b);
}

int Graph::Edge::other(int u) const
{
	if (u == vertex1) return vertex2;
	if (u == vertex2) return vertex1;
	throw std::runtime_error("Graph::Edge::other(int u) -> edge doesn't contain given vertex");
}

std::string Graph::Edge::toString()
{
	return  "Edge: {v1: " + std::to_string(getVertex1()+1) + ", v2: " + std::to_string(getVertex2()+1)
		+ ", weight: " + std::to_string(getWeight()) + "}";
}

Graph::Graph()
{
}

void Graph::addVertex(int v, bool forceCreate)
{
	//if u already exist of if it is new but id is not in order 
	if (v < adjacencyList.size())
		throw std::runtime_error("Graph::addVertex(int v, bool forceCreate) -> vertex already exist in graph");
	else if (v > adjacencyList.size())
	{
		if (!forceCreate)
			throw std::runtime_error("Graph::addVertex(int v, bool forceCreate) -> creating out of order vertex");

		//force create
		for (int i = adjacencyList.size(); i < v; i++)
		{
			adjacencyList.emplace_back();
		}
		
	}

	//add vertex
	adjacencyList.emplace_back();
}

void Graph::addEdge(int v, int u, int weight, bool createIfNotFound, bool forceCreate, int id)
{
	//get the smaller
	int first, second;
	getInOrder(v, u, first, second);
	//check if v and u dont exist
	if (first >= adjacencyList.size() || second >= adjacencyList.size())
	{
		//should we create them?
		if (!createIfNotFound)
			throw std::runtime_error("Graph::addEdge(int v, int u, int weight, bool createIfNotFound,"
			"bool forceCreate, int id) -> vertex not found in graph");
		
		//try to add
		addVertex(first, forceCreate);
		addVertex(second, forceCreate);
	}
	if (isAdjacent(first, second))
	{
		throw std::runtime_error("Graph::addEdge(int v, int u, int weight, bool createIfNotFound,"
			"bool forceCreate, int id) -> vertex already exist in graph");
	}
	
	//add
	adjacencyList[first].emplace_back(first,second, weight, id);
	adjacencyList[second].emplace_back(first, second, weight, id);

}

void Graph::addEdgeEx(int v, int u, int weight, bool createIfNotFound, bool forceCreate)
{
	return addEdge(v - 1, u - 1, weight, createIfNotFound, forceCreate);
}

bool Graph::isAdjacent(int v, int u)
{
	//get the smaller
	int first, second;
	getInOrder(v, u, first, second);
	for (auto& it : adjacencyList[first])
	{
		if (it.getVertex2() == second)
			return true;
	}
	return false;
}

const std::list<Graph::Edge>& Graph::getAdjacent(int u)
{
	if (u >= adjacencyList.size())
		throw std::runtime_error("Graph::getAdjacent(int u) -> vertex not found in graph");

	return adjacencyList[u];
}


Graph::Edge::edge_set_ptr Graph::getSortedEdges()
{
	Edge::edge_set_ptr edges;
	for (auto& list : adjacencyList)
	{
		for (auto& edge : list)
		{
			edges.insert(&edge);
		}
	}
	return edges;
}


void Graph::deleteVertex(int v)
{
	if (v < 0 || v >= adjacencyList.size())
		throw std::runtime_error("Graph::deleteVertex(int v) -> vertex not found in graph");

	adjacencyList.erase(adjacencyList.begin() + v);
	for (int k = 0; k < adjacencyList.size();k++)
	{
		auto& list = adjacencyList[k];
		std::list<Edge>::iterator i = list.begin();
		while (i != list.end())
		{
			if (i->other(k) == v)
				i = list.erase(i);
			else
				++i;
		}
	}
}

void Graph::getInOrder(int v, int u, int& first, int& second)
{
	if (u > v)
	{
		first = v;
		second = u;
	}
	else
	{
		first = u;
		second = v;
	}
}

bool operator<(const Graph::Edge& right, const Graph::Edge& left)
{
	bool eq_in_2 = right.getVertex2() == left.getVertex2(), eq_in_1 = right.getVertex1() == left.getVertex1();
	if (eq_in_1 && eq_in_2)
	{
		return false;
	}
	if (right.getWeight() == left.getWeight())
	{
		if (!eq_in_1)
			return right.getVertex1() < left.getVertex1();

		return right.getVertex2() < left.getVertex2();
	}

	return 	right.getWeight() < left.getWeight();
}

