#include "ViewText.h"
#include <QPainter>

ViewText::ViewText(const QString& text, QGraphicsItem *parent)
	: QGraphicsTextItem(text,parent)
{

}

ViewText::~ViewText()
{

}

void ViewText::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	
	painter->setRenderHints(QPainter::HighQualityAntialiasing);
	QGraphicsTextItem::paint(painter, option, widget);
}