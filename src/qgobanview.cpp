#include <QGraphicsScene>
#include <QMouseEvent>
#include <QMessageBox>
#include <QString>
#include "qgobanview.h"
QGobanView::QGobanView(int size, QWidget *parent) : QGraphicsView(parent) {
    _size=size;
    _margin=9*25./_size;
    _spacing=9*50./_size;
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0,0,450,450);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
}
void QGobanView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);
    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
    // Lines
    for(int i=0;i<_size;i++) {
        painter->drawLine(sceneRect.left()+_margin+i*_spacing,sceneRect.top()+_margin,
                          sceneRect.left()+_margin+i*_spacing,sceneRect.height()-_margin);
        painter->drawLine(sceneRect.left()+_margin,sceneRect.top()+_margin+i*_spacing,
                          sceneRect.width()-_margin,sceneRect.top()+_margin+i*_spacing);
    }
}
void QGobanView::mousePressEvent(QMouseEvent *event) {
    if(event->button()==Qt::LeftButton) {
        QPointF point=mapToScene(event->pos());
        //Convert to coordinates.
        int i=(int)(point.x()/_spacing);
        int j=(int)(point.y()/_spacing);
        emit nodeSelected(i,j);
    }
}
void QGobanView::redraw(Stone **goban) {
    this->scene()->clear();
    qreal stonesize=9*25/_size;
    for(int i=0;i<_size;i++) {
        for(int j=0;j<_size;j++) {
            if(goban[i][j].colour()=='b') {
                QRectF rect(_margin+i*_spacing-stonesize,_margin+j*_spacing-stonesize,2*stonesize,2*stonesize);
                this->scene()->addEllipse(rect,QPen(QColor("black")),QBrush(Qt::SolidPattern));
            } else if(goban[i][j].colour()=='w') {
                QRectF rect(_margin+i*_spacing-stonesize,_margin+j*_spacing-stonesize,2*stonesize,2*stonesize);
                this->scene()->addEllipse(rect,QPen(QColor("lightGray")),QBrush(QColor("white"),Qt::SolidPattern));
            }
        }
    }
}
/* qgobanview.cpp */