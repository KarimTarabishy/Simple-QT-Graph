#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "Graph.h"
class GraphViewer;

class GraphController : public QObject
{
	Q_OBJECT

public:
	GraphController(QObject *parent = nullptr);
	~GraphController();

	void setGraphViewer(GraphViewer* viewer);
	bool alreadyConnected(int from, int to);
	std::vector<int> getAdjacentEdgesIds(int u);
public slots:
	void nodeAdded(int id);
	void edgeAdded(int id, int from, int to, int w);
	void primRequest();
	void kruskalRequest();
	void resetRequest();

private:
	GraphViewer* viewer;
	Graph* graph;

};

#endif // CONTROLLER_H
