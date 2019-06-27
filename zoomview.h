#ifndef ZOOMVIEW_H
#define ZOOMVIEW_H

#include <QDebug>
#include <QRect>
#include <QTimer>
#include <QPixmap>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>

class ZoomView : public QGraphicsView
{
    QGraphicsPixmapItem * pixmapItem;
    QGraphicsTextItem * textItem;
    QRectF * rect;
    QGraphicsRectItem * rectItem;
    QGraphicsScene scene;
    QTimer refreshTimer;

    void redrawScene();
public:
    ZoomView();
    ~ZoomView();
    ZoomView(QWidget * parent);

    QPoint originPos;

    void setImage(QPixmap image);
    void setRectangle(int x, int y, int width, int height, QString text);

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
};

#endif // ZOOMVIEW_H
