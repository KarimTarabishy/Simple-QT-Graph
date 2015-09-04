#ifndef VIEWEDGE_H
#define VIEWEDGE_H

#include <QGraphicsLineItem>
#include "ViewNode.h"
#include <unordered_map>
#include "ViewText.h"

class ViewEdge : public QGraphicsLineItem
{
	static int _id;
	static std::unordered_map<int, ViewEdge*> edges;

public:
	ViewEdge(QPointF a, QPointF b, QGraphicsItem *parent = 0);

	void link(const QPointF& p1, const QPointF& p2, bool ignoreText = false);
	void link(ViewNode* n1, ViewNode* n2, bool ignoreText = false, int weight = 1);
	int getId() const{ return id; }
	int getWeight() const { return weight; }
	void relink();
	void enforce(bool yes);
	void getNodes(ViewNode* n1, ViewNode* n2){ n1 = node1; n2 = node2; }

	static ViewEdge* get(int id);
	static void resetIds(){ _id = 0; edges.clear(); }
	static std::unordered_map<int, ViewEdge*>& getEdges(){ return edges; }
	
private:
	ViewNode *node1, *node2;
	ViewText* textItem;

	int id;
	int weight;

	QPen normal, selected;

};

#endif // VIEWEDGE_H
