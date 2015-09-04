#include "ViewEdge.h"
#include <QGenericMatrix>
#include <vector>
#include "GraphViewer.h"

int ViewEdge::_id=0;
std::unordered_map<int, ViewEdge*> ViewEdge::edges;
const long double PI = 3.141592653589793238L;
ViewEdge::ViewEdge(QPointF a, QPointF b, QGraphicsItem *parent)
	: QGraphicsLineItem(QLineF(a,b),parent), node1(nullptr), node2(nullptr)
{
	id = _id++;
	textItem = nullptr;
	weight = 1;
	setZValue(-1);
	edges[id] = this;
	normal = pen();
	selected = QPen(QBrush(QColor(255, 60, 50, 255)), 4);
}

void ViewEdge::link(const QPointF& p1, const QPointF& p2, bool ignoreText)
{
	setLine(QLineF(p1, p2));
	if (!ignoreText)
	{
		if (!textItem)
		{
			textItem = new ViewText(std::to_string(1).c_str(), this);
			textItem->setTextInteractionFlags(Qt::TextEditable);
		}
		textItem->setTransform(QTransform(1, 0, 0, 1, 0,0));
		//get box top coordintates in scene space
		auto br =textItem->boundingRect();
		//get a vector in the direction of the line, in scene space
		QPointF vec = (p2 - p1);
		QPointF vec2 = (p1 - p2);
		QPointF add = p1;
		double theta = (std::atan2(vec.y(), vec.x())*180/PI);
		double theta_ = theta;
		if (theta < 0)
			theta += 180;
		bool flag = false;

		//2nd and 4th quads, horizontal included
		if (theta > 90 && theta <= 180)
		{
			if (theta_>=0)
			{
				vec = vec2;
				add = p2;
			}
				
		}
		else //1st and 3rd quads, vertical included
		{
			if (theta_ < 0)
			{
				vec = vec2;
				add = p2;
			}
			flag = true;
		}

		double mag = sqrt((vec.x()*vec.x()) + (vec.y()*vec.y()));
		double mul = flag ? -1.0 : 1.0;
		QPointF u = vec;
		u.setX(vec.x() / mag);
		u.setY(u.y() / mag);
		QPointF pos = p1 + p2;
		pos /= 2;
		const double r1 = sin(60 * (PI / 180));
		if (std::abs(u.y()) >= r1)
		{
			pos += mul*u*(br.height() / 2);
		}
		if (flag)
		{
			textItem->setTransform(QTransform(1, 0, 0, 1, -br.width(), 0));
		}

		textItem->setPos(pos.x(), pos.y());
	}

}

void ViewEdge::link(ViewNode* n1, ViewNode* n2, bool ignoreText, int weight)
{
	if (!ignoreText)
	{
		node1 = n1;
		node2 = n2;
		this->weight = weight;
	}
	link(n1->pos(), n2->pos(), ignoreText);
}

void ViewEdge::relink()
{
	link(node1->pos(), node2->pos());
}

void ViewEdge::enforce(bool yes)
{
	setPen(yes ? selected : normal);
}

ViewEdge* ViewEdge::get(int id)
{
	auto edge = edges.find(id);
	if (edge == edges.end())
		throw std::runtime_error(" ViewEdge::get(int id) -> Edge not found");

	return edge->second;
}



