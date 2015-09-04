#ifndef VIEWNODE_H
#define VIEWNODE_H

#include <QGraphicsItem>
#include <QPen>
#include <unordered_map>
class ViewEdge;
class ViewNode : public QGraphicsEllipseItem
{
	static int _id;
	static std::unordered_map<int, ViewNode*> nodes;
public:
	enum {Type = UserType + 1};
	ViewNode(qreal x, qreal y, qreal radius, bool defaultIncrease = true, QGraphicsEllipseItem *parent = 0);
	~ViewNode();

	int type() const override { return Type; }
	void setIncreased(bool increased, bool force = false);
	void setText(QString text){ this->text = text; }
	int getId() const{ return id; }
	void updateEdge();
	void setInPath(bool yes);

	static ViewNode* get(int id);
	static std::unordered_map<int, ViewNode*> getNodes(){ return nodes; }
	static void resetIds(){ _id = 0; nodes.clear(); }

	
protected:
	void paint(QPainter *painter,
		const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
	

private:
	QPointF startDragPos;
	QPen normal, large, inPathPen;
	bool isIncreased, inPath;
	QString text;
	int id;
	double radius;

	
};

#endif // VIEWNODE_H
