#include "zoomview.h"

void ZoomView::redrawScene()
{
    QPoint currentPos = this->cursor().pos();
    translate(currentPos.x() - originPos.x(),originPos.y()- currentPos.y());

    originPos = currentPos;
}

ZoomView::ZoomView()
{

}

ZoomView::~ZoomView()
{
    delete textItem;
    delete rectItem;
    delete  pixmapItem;
}

ZoomView::ZoomView(QWidget *parent)
{
    this->setScene(&scene);

    //sceneBrush.setColor(Qt::GlobalColor::red);

    textItem = new QGraphicsTextItem();
    textItem->setDefaultTextColor(QColor(Qt::GlobalColor::green));

    rectItem = new QGraphicsRectItem();
    pixmapItem = new QGraphicsPixmapItem();

    this->scene.setBackgroundBrush(sceneBrush);
    this->scene.addItem(textItem);
    this->scene.addItem(rectItem);
    this->scene.addItem(pixmapItem);

    pixmapItem->setZValue(0);
    rectItem->setZValue(3);
    textItem->setZValue(3);

    connect(&refreshTimer, &QTimer::timeout,this, &ZoomView::redrawScene);
}

void ZoomView::setImage(QPixmap image)
{
    pixmapItem->setPixmap(image);
}

void ZoomView::setRectangle(int x, int y, int width, int height, QString text)
{
    scale(1.0,1.0);
    textItem->setScale(1.0);

    rectItem->setRect(x,y,width,height);
    textItem->setPlainText(text);
    textItem->setPos(x,y+height);
}

void ZoomView::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    if (!numDegrees.isNull())
    {
        if(numDegrees.y() > 0)
        {
            scale(2.0,2.0);
            textItem->setScale(textItem->scale() / 1.1);
            this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            //this->scene.addItem(textItem);
        }
        else
        {
           textItem->setScale(textItem->scale() * 1.1);
           this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
           scale(0.5,0.5);
        }
    }
}

void ZoomView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        originPos = event->pos();

        refreshTimer.start(1);
    }
}

void ZoomView::mouseReleaseEvent(QMouseEvent *event)
{
    refreshTimer.stop();
}

