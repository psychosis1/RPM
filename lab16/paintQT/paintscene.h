#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class PaintScene : public QGraphicsScene
{
public:
    PaintScene();
private:
    QPointF previousPoint;
public:
    int size;
    Qt::PenStyle style;
    QColor color;
private:
    /**
     * @brief mousePressEvent Назначение 1 точки (эллипса)
     * @param event Нажатие
     */
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    /**
     * @brief mouseMoveEvent Линия
     * @param event Движение мышки
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PAINTSCENE_H
