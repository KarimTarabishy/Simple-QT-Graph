#include "ViewNode.h"
#include <QGraphicsSceneDragDropEvent>
#include "GraphViewer.h"
#include "ViewEdge.h"
#include "GraphController.h"
int ViewNode::_id = 0;
std::unordered_map<int, ViewNode*> ViewNode::nodes;
ViewNode::ViewNode(qreal x, qreal y, qreal radius, bool defaultIncrease, QGraphicsEllipseItem *parent)
	: QGraphicsEllipseItem(x, y, radius, radius, parent), normal(pen()), large(pen()), radius(radius),
	inPath(false), inPathPen(QBrush(QColor(0,0,255,255)), 3)
{
	if (defaultIncrease)
	{
		id = _id++;
		text = std::to_string(id).c_str();
		nodes[id] = this;
	}
	setTransform(QTransform(1, 0, 0, 1, -radius/2, -radius/2));
	setFlags(ItemIsMovable);
	setAcceptHoverEvents(true);
	large.setWidth(3);
	setIncreased(defaultIncrease);
	setZValue(0);
	setBrush(QBrush(QColor(255, 255, 255)));

	
}

ViewNode::~ViewNode()
{

}

void ViewNode::updateEdge()
{
	auto edges = GraphViewer::get()->getController()->getAdjacentEdgesIds(id);
	for (auto& edge:edges)
	{
		ViewEdge::get(edge)->relink();
	}
}

void ViewNode::setInPath(bool yes)
{
	inPath = yes;
	if (yes)
	{
		isIncreased = false;
		setPen(inPathPen);
	}
	else
	{
		setPen(normal);
	}
}

ViewNode* ViewNode::get(int id)
{
	auto node = nodes.find(id);
	if (node == nodes.end())
		throw std::runtime_error(" ViewEdge::get(int id) -> Edge not found");

	return node->second;
}

void ViewNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

	QGraphicsEllipseItem::paint(painter, option, widget);
	if (!text.isEmpty())
	{
		auto r = boundingRect();
		QRectF s(r.x(), r.y() - 0.3, r.width(), r.height() - 0.3);
		painter->drawText(s, Qt::AlignCenter, text);
	}

}

void ViewNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (zValue() == -100)
	{
		event->ignore();
		return;
	}
	GraphViewer::get()->getScene()->notifyNodeDrag(true, this);
	startDragPos = pos();
	QGraphicsEllipseItem::mousePressEvent(event);
}

void ViewNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (zValue() == -100)
	{
		event->ignore();
		return;
	}
	GraphViewer::get()->getScene()->notifyNodeDrag(false);
	if (startDragPos != pos())
	{
		auto item = collidingItems();
		bool reset = false;
		for (auto i : item)
		{
			if (qgraphicsitem_cast<ViewNode*>(i))
			{
				reset = true;
				break;
			}
		}
		if (reset)
		{
			setPos(startDragPos);
		}
	}
	QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void ViewNode::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	if (zValue()>= 0)
	{
		setIncreased(true);
		GraphViewer::get()->getScene()->notifyNodeEntered();
	}
	
	QGraphicsEllipseItem::hoverEnterEvent(event);
}

void ViewNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	if (zValue() >= 0)
	{
		setIncreased(false);
		GraphViewer::get()->getScene()->notifyNodeLeft();
	}
	QGraphicsEllipseItem::hoverLeaveEvent(event);
}



void ViewNode::setIncreased(bool increased, bool force)
{
	if (inPath)
		return;

	if (increased == isIncreased && !force)
		return;

	isIncreased = increased;
	if (isIncreased)
	{
		setPen(large);
	}
	else
	{
		setPen(normal);
	}
}