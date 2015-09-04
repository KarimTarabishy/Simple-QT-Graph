#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include "ViewNode.h"
#include <chrono>
#include "ViewEdge.h"

class GraphScene : public QGraphicsScene
{
	Q_OBJECT

public:
	GraphScene(QObject *parent = 0);
	~GraphScene();

	void notifyNodeEntered();
	void notifyNodeLeft();
	void notifyNodeDrag(bool start, ViewNode* draggedNode = nullptr);

public slots:
	void entered();
	void left();
	void resetProbeColor();
	void resetRequest();
	void weightEntered(int weight);


protected:
	void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent) override;
	
signals:
	void addGNode(int id);
	void addGEdge(int id, int from, int to, int w);

private:
	ViewNode* probe;
	QPen safePen, errorPen, edgePen, normalPen;
	bool inNode, isDragging;
	bool isDrawingEdge;
	void notifyInvalidPlacementPosition();
	int recWeight;

	ViewNode* lastNode, *edgeNode, *lastConnected, *draggedNode, *tmpNode;
	ViewEdge* edge;

};

#endif // GRAPHSCENE_H
