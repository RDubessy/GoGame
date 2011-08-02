#ifndef QGOBANVIEW_H
#define QGOBANVIEW_H
#include <QtGui/QGraphicsView>
#include "stone.h"
class QGobanView : public QGraphicsView {
    Q_OBJECT
public:
    QGobanView(QWidget *parent = 0);
    void redraw(Stone **goban);
    void init(int size);
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