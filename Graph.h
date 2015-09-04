#pragma once
#include <vector>
#include <set>
#include <list>

class Graph
{
public:
	class Edge
	{
		int vertex1, vertex2, weight, id;
	public:
		struct PTR_COMPARE
		{
			bool operator()(const Edge* a, const Edge* b);
		};
		struct PTR_COMPARE_GREATER
		{
			bool operator()(const Edge* a, const Edge* b);
		};
		typedef std::set<Edge*, PTR_COMPARE> edge_set_ptr;
		typedef std::set<Edge> edge_set;

		Edge(int vertex1, int vertex2, int weight, int id = -1) :
			vertex1(vertex1), vertex2(vertex2), weight(weight), id(id){}

		int getVertex1() const
		{
			return vertex1;
		}

		int getVertex2() const
		{
			return vertex2;
		}
		int getId() const {
			return id;
		}
		int getWeight() const
		{
			return weight;
		}
		void setWeight(int weight){ this->weight = weight; }
		int either() const{ return vertex1; }
		int other(int u) const;
		std::string toString();
	};
private:
	std::vector<std::list<Edge>> adjacencyList;


public:

	Graph();
	void addVertex(int v, bool forceCreate = false);
	void addEdge(int v, int u, int weight, bool createIfNotFound = false, bool forceCreate = false, int id = -1);
	void addEdgeEx(int v, int u, int weight, bool createIfNotFound = true, bool forceCreate = true);
	bool isAdjacent(int v, int u);
	const std::list<Edge>& getAdjacent(int u);
	Edge::edge_set_ptr getSortedEdges();
	int getVertexCount(){ return adjacencyList.size(); }
	void deleteVertex(int v);


	
private:
	void getInOrder(int v, int u, int& first, int& second);
};

bool operator<(const Graph::Edge& right, const Graph::Edge& left);

