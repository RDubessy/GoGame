#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QToolTip>
#include <QGraphicsWidget>

#include "qstone.h"
QRectF QStone::boundingRect() const {
    return QRectF(-_size,-_size,2*_size,2*_size);
}
QPainterPath QStone::shape() const {
    QPainterPath path;
    path.addEllipse(-_size,-_size,2*_size,2*_size);
    return path;
}
void QStone::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(Qt::lightGray);
    if(_color=='b') painter->setBrush(QBrush(QColor("black"),Qt::SolidPattern));
    else painter->setBrush(QBrush(QColor("white"),Qt::SolidPattern));
    painter->drawEllipse(-_size,-_size,2*_size,2*_size);
}
/* qstone.cpp */