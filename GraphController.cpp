#include "GraphController.h"
#include "GraphViewer.h"
#include "GraphAlgorithms.h"
#include <unordered_set>

GraphController::GraphController(QObject *parent)
	: QObject(parent)
{
	graph = new Graph();
	viewer = nullptr;
}

GraphController::~GraphController()
{
	delete graph;
	graph = nullptr;
}

void GraphController::setGraphViewer(GraphViewer* viewer)
{
	this->viewer = viewer;
}

bool GraphController::alreadyConnected(int from, int to)
{
	return graph->isAdjacent(from, to);
}

std::vector<int> GraphController::getAdjacentEdgesIds(int u)
{
	std::vector<int> ids;
	auto edges = graph->getAdjacent(u);
	for (auto& edge : edges)
	{
		if (edge.getId() >= 0)
		{
			ids.push_back(edge.getId());
		}
		
	}
	return ids;
}



void GraphController::nodeAdded(int id)
{
	graph->addVertex(id);
}

void GraphController::edgeAdded(int id, int from, int to, int w)
{
	graph->addEdge(from, to, w, false, false, id);
}

void GraphController::primRequest()
{
	viewer->unselectEdges();
	bool fullyConnected = GraphAlgorithms::isFullyConnected(*graph);
	if (!fullyConnected)
	{
		viewer->showResult("Prim Algorithm works only on fully connected graphs.");
		return;
	}

	auto result = GraphAlgorithms::getPrimMST(*graph);
	int sum = 0;
	std::vector<int> edgesIds;
	for (auto& edge : result)
	{
		sum += edge.getWeight();
		edgesIds.push_back(edge.getId());
	}
	viewer->selectEdges(edgesIds, std::vector<int>());
	viewer->showResult(("MST found with total weight = " + std::to_string(sum)).c_str());
}

void GraphController::kruskalRequest()
{
	viewer->unselectEdges();
	DisjointSet set;
	auto result = GraphAlgorithms::getKruskalMST(*graph, &set);

	if (result.empty())
	{
		viewer->showResult("No MST found.");
		return;
	}
	std::unordered_map<int, int> sums;
	std::vector<int> edgesIds;
	for (auto& edge: result)
	{
		int id = set.find(edge.getVertex1());
		auto it = sums.find(id);
		if (it != sums.end())
			it->second += edge.getWeight();
		else
		{
			sums[id] = edge.getWeight();
		}
		edgesIds.push_back(edge.getId());
	}
	std::vector<int> nodes;
	std::string st;
	for (auto& it:sums)
	{
		st += "(" + std::to_string(it.first) +") = " + std::to_string(it.second) + ", ";
		nodes.push_back(it.first);
	}
	st.erase(st.end() - 2, st.end());
	viewer->selectEdges(edgesIds, nodes);
	std::string s = "";
	if (nodes.size() > 1)
	{
		s = "s";
	}
	viewer->showResult((std::to_string(sums.size()) +" MST found with total weight"+s+":  " + st).c_str());
}

void GraphController::resetRequest()
{

	delete graph;
	graph = new Graph;
}