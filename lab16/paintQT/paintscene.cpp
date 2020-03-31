#include "paintscene.h"

PaintScene::PaintScene()
{

}
void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    addEllipse(event->scenePos().x() - 10,
               event->scenePos().y() - 10,
               size,
               size,
               QPen(Qt::NoPen),
               QBrush(color));
    previousPoint = event->scenePos();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(color,size,style,Qt::RoundCap));
    previousPoint = event->scenePos();
}
