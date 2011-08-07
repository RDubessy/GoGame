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
    //Stars
    int size=(int)(_spacing*0.1);
    //Always mark the center
    painter->setBrush(Qt::SolidPattern);
    painter->drawEllipse(QPoint(sceneRect.left()+_margin+(_size-1)/2*_spacing,
                                 sceneRect.top()+_margin+(_size-1)/2*_spacing),size,size);
    if(_size>=13) {
        //Mark the corners
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+3*_spacing,
                                     sceneRect.top()+_margin+3*_spacing),size,size);
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+(_size-4)*_spacing,
                                     sceneRect.top()+_margin+3*_spacing),size,size);
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+(_size-4)*_spacing,
                                     sceneRect.top()+_margin+(_size-4)*_spacing),size,size);
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+3*_spacing,
                                     sceneRect.top()+_margin+(_size-4)*_spacing),size,size);
    }
    if(_size==19) {
        //Mark the middle
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+(_size-1)/2*_spacing,
                                     sceneRect.top()+_margin+3*_spacing),size,size);
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+(_size-1)/2*_spacing,
                                     sceneRect.top()+_margin+(_size-4)*_spacing),size,size);
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+(_size-4)*_spacing,
                                     sceneRect.top()+_margin+(_size-1)/2*_spacing),size,size);
        painter->drawEllipse(QPoint(sceneRect.left()+_margin+3*_spacing,
                                     sceneRect.top()+_margin+(_size-1)/2*_spacing),size,size);
    }
}
void QGobanView::mousePressEvent(QMouseEvent *event) {
    QPointF point=mapToScene(event->pos());
    //Convert to coordinates.
    int i=(int)(point.x()/_spacing);
    int j=(int)(point.y()/_spacing);
    switch(event->button()) {
        case Qt::LeftButton :
            emit nodeSelected(i,j);
            break;
        case Qt::RightButton :
            emit stoneSelected(i,j,event->globalPos());
            break;
        default:
            break;
    }
    //QGraphicsView::mousePressEvent(event);
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
            }
        }
    }
}
/* qgobanview.cpp */