#include "GraphScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "ViewNode.h"
#include <QTimer>
#include "GraphViewer.h"
#include <unordered_set>

GraphScene::GraphScene(QObject *parent)
	: QGraphicsScene(parent), safePen(QPen(QBrush(QColor(0, 0, 0, 80)), 1, Qt::DashLine)),
	errorPen(QPen(QBrush(QColor(255, 0, 0, 250)), 1, Qt::DashLine)), 
	edgePen(QPen(QBrush(QColor(0, 255, 0, 255)), 2))

{
	probe = new ViewNode(0, 0, 20, false);
	normalPen = probe->pen();
	probe->setZValue(-100);
	probe->setPen(QPen(QBrush(QColor(0,0, 0, 80)), 1,Qt::DashLine));
	probe->setVisible(false);
	addItem(probe);
	lastNode = nullptr;
	edgeNode = nullptr;
	edge = nullptr;
	inNode = false;
	isDrawingEdge = false;
	isDragging = false;
	lastConnected = nullptr;
	draggedNode = nullptr;
	isEnteringWeight = false;
}

GraphScene::~GraphScene()
{
}

void GraphScene::notifyNodeEntered()
{
	inNode = true;
	if (isDrawingEdge) return;
	probe->setVisible(false);
}

void GraphScene::notifyNodeLeft()
{
	inNode = false;
	if (isDrawingEdge) return;
	probe->setVisible(true);
	
}

void GraphScene::notifyNodeDrag(bool start, ViewNode* draggedNode)
{
	isDragging = start;
	if (start)
	{
		this->draggedNode = draggedNode;
	}
	else
		this->draggedNode = nullptr;
}

void GraphScene::entered()
{
	probe->setVisible(true);
	auto gv = GraphViewer::get()->getQGView();
	probe->setPos(gv->mapToScene(gv->mapFromGlobal(QCursor::pos())));
}

void GraphScene::left()
{
	if (isEnteringWeight)
		return;

	probe->setVisible(false);

	//start resetting all states
	inNode = false;
	lastNode = nullptr;
	if (isDrawingEdge)
	{
		removeItem(edge);
		edge = nullptr;
		edgeNode->setPen(normalPen);
		edgeNode = nullptr;
		isDrawingEdge = false;
	}
}

void GraphScene::resetProbeColor()
{
	probe->setPen(safePen);
}

void GraphScene::resetRequest()
{
	auto edges = ViewEdge::getEdges();
	for (auto& edge : edges)
	{
		removeItem(edge.second);
	}
	auto nodes = ViewNode::getNodes();
	for (auto& node : nodes)
	{
		removeItem(node.second);
	}

	ViewNode::resetIds();
	ViewEdge::resetIds();
	lastNode = nullptr;
	edgeNode = nullptr;
	edge = nullptr;
	inNode = false;
	isDrawingEdge = false;
	isDragging = false;
	lastConnected = nullptr;
	draggedNode = nullptr;
}

void GraphScene::weightEntered(int weight)
{
	setFocus();
	edge->link(edgeNode, tmpNode, false, weight);
	
	emit addGEdge(edge->getId(), edgeNode->getId(), tmpNode->getId(), edge->getWeight());

	edge = nullptr;
	if (!edgeNode->isInPath())
		edgeNode->setPen(normalPen);
	edgeNode = nullptr;
	isDrawingEdge = false;
	isEnteringWeight = false;

	
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{

	if (isDrawingEdge)
	{
		auto item = itemAt(mouseEvent->scenePos(), GraphViewer::get()->getQGView()->transform());
		ViewNode* node = qgraphicsitem_cast<ViewNode*>(item);
		if (!node || node == edgeNode)
		{
			removeItem(edge);
			edge = nullptr;
			if (!edgeNode->isInPath())
				edgeNode->setPen(normalPen);
			edgeNode = nullptr;
			isDrawingEdge = false;
			if (!node)
				probe->setVisible(true);
			if (node)
				node->setIncreased(true, true);
		}
		else
		{
			isEnteringWeight = true;
			tmpNode = node;
			edge->setPen(normalPen);
			if (!tmpNode->isInPath())
				tmpNode->setPen(normalPen);
			GraphViewer::get()->showWeightDialog();
		}

		return;
	}

	if (inNode || isDragging)
	{
		QGraphicsScene::mousePressEvent(mouseEvent);
		return;
	}

	//creating node
	
	auto item = probe->collidingItems();
	
	if (!item.isEmpty())
	{
		notifyInvalidPlacementPosition();
		QGraphicsScene::mousePressEvent(mouseEvent);
		return;
	}
	lastNode = new ViewNode(0,0, 20);
	lastNode->setPos(probe->pos());
	addItem(lastNode);
	probe->setVisible(false);
	emit addGNode(lastNode->getId());

}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	
	QGraphicsView* view = GraphViewer::get()->getQGView();

	if (!inNode || isDragging || isDrawingEdge)
	{
		QPointF min, max;
		GraphViewer::get()->getQGVPoints(min, max);
		auto spos = mouseEvent->screenPos();
		QPoint np = spos;
		if (spos.x() - min.x() < 11)
			np.setX(min.x() + 11);
		if (max.x() - spos.x() < 11)
			np.setX(max.x() - 11);
		if (spos.y() - min.y() < 11)
			np.setY(min.y() + 11);
		if (max.y() - spos.y() < 11)
			np.setY(max.y() - 11);
		auto gv = GraphViewer::get()->getQGView();
		QPointF ss = gv->mapToScene(gv->mapFromGlobal(np));
		probe->setPos(ss);
		if (isDragging)
		{
			QCursor::setPos(np);
		}
	}
	if (isDrawingEdge)
	{

		auto item = itemAt(mouseEvent->scenePos(), view->transform());
		ViewNode* node = qgraphicsitem_cast<ViewNode*>(item);
		if (!node)
		{
			edge->link(edgeNode->pos(), mouseEvent->scenePos(), true);
			if (lastConnected)
			{
				lastConnected->setPen(normalPen);
				lastConnected = nullptr;
			}
			edge->setPen(normalPen);
			
		}
		else if (node != edgeNode && node != probe) 
		{
			//we found a new connected node
			if (lastConnected)
				lastConnected->setPen(normalPen);
			lastConnected = node;
			if (!lastConnected->isInPath())
				lastConnected->setPen(edgePen);
			edge->link(edgeNode->pos(),node->pos(), true);
			edge->setPen(edgePen);
		}
		
		
		return;
	}
	if (isDragging)
	{
		draggedNode->updateEdge();
	}
	
	
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (lastNode)
	{
		auto item = itemAt(event->scenePos(), GraphViewer::get()->getQGView()->transform());
		if (item != lastNode)
			lastNode->setIncreased(false);
		else//created item don't recieve first press and release
			inNode = true;

		lastNode = nullptr;
		probe->setVisible(!item);
	}

	QGraphicsScene::mouseReleaseEvent(event);
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	if (isDrawingEdge)
	{
		mousePressEvent(mouseEvent);
		return;
	}
	auto item = itemAt(mouseEvent->scenePos(), GraphViewer::get()->getQGView()->transform());
	ViewNode* node = qgraphicsitem_cast<ViewNode*>(item);
	if (node)
	{
		isDrawingEdge = true;
		edgeNode = node;
		edge = new ViewEdge(node->pos(), node->pos());
		if (!edgeNode->isInPath())
			edgeNode->setPen(edgePen);
		addItem(edge);
	}
	


}

void GraphScene::notifyInvalidPlacementPosition()
{
	if (probe->isVisible())
	{
		probe->setPen(errorPen);
		QTimer::singleShot(1000, this, &GraphScene::resetProbeColor);
	}
}
