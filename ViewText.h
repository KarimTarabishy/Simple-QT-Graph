#ifndef VIEWTEXT_H
#define VIEWTEXT_H

#include <QGraphicsTextItem>

class ViewText : public QGraphicsTextItem
{
public:
	ViewText(const QString& text,QGraphicsItem *parent = 0);
	~ViewText();

protected:
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;
	
};

#endif // VIEWTEXT_H
