#include <QGraphicsScene>
#include <QMouseEvent>
#include <QMessageBox>
#include <QString>
#include "qgobanview.h"
#include "qstone.h"
#include "goban.h"
QGobanView::QGobanView(QWidget *parent) : QGraphicsView(parent) {
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0,0,450,450);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
}
void QGobanView::init(int size) {
    _size=size;
    _margin=9*25./_size;
    _spacing=9*50./_size;
    resetCachedContent();
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
    QGraphicsView::mousePressEvent(event);
}
void QGobanView::redraw(Stone **goban) {
    scene()->clear();
    qreal stonesize=9*25/_size;
    for(int i=0;i<_size;i++) {
        for(int j=0;j<_size;j++) {
            if(goban[i][j].colour()!='.') {
                QStone *stone=new QStone(goban[i][j].colour(),stonesize);
                scene()->addItem(stone);
                stone->setPos(_margin+i*_spacing,_margin+j*_spacing);
                stone->setToolTip(tr("#stones : %1\n#freedom : %2\n")
                                  .arg(goban[i][j].group()->stones()->size())
                                  .arg(goban[i][j].group()->freedom()->size()));
            }
        }
    }
}
/* qgobanview.cpp */