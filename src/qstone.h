#ifndef QSTONE_H
#define QSTONE_H

#include <QGraphicsItem>

class QGraphicsSceneMouseEvent;

class QStone : public QGraphicsItem {
public:
    QStone(char color, qreal size) {
        _color=color;
        _size=size;
    };
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    char _color;
    qreal _size;
};
#endif // QSTONE_H
/* qstone.h */