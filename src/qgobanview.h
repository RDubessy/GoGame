#ifndef QGOBANVIEW_H
#define QGOBANVIEW_H
#include <QtGui/QGraphicsView>
#include "stone.h"
class QGobanView : public QGraphicsView {
    Q_OBJECT
public:
    QGobanView(int size = 19,QWidget *parent = 0);
    void redraw(Stone **goban);
signals:
    void nodeSelected(int i, int j);
public slots:
protected:
    void drawBackground(QPainter *painter, const QRectF &rect);
    void mousePressEvent(QMouseEvent *event);
private:
    qreal _margin,_spacing;
    int _size;
};
#endif // QGOBANVIEW_H
/* qgobanview.h */