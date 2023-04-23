#include "item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

Item::Item(QRectF rect, QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
    path.addRoundedRect(rect, 10, 10);
    background = Qt::transparent;
}

QRectF Item::boundingRect() const
{
    return path.boundingRect();
}

QPainterPath Item::shape() const
{
    return path;
}

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillPath(path, background);
}

void Item::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsObject::hoverEnterEvent(event);
    background = Qt::yellow;
    emit hovered(true);
    update();
}

void Item::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsObject::hoverLeaveEvent(event);
    background = Qt::transparent;
    emit hovered(false);
    update();
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //    addItem("cpu socket", QRectF(196.84, 126.924, 66, 66));
    //    addItem("ram1", QRectF(334.510, 87.1261, 10, 183));
    //    addItem("ram2", QRectF(375.395, 87.1261, 10, 183));
    //    addItem("gpu", QRectF(124.773, 321.613, 186, 8));
    //    addItem("pci1", QRectF(154.891, 378.622, 186, 8));
    //    addItem("pci1", QRectF(154.891, 419.955, 186, 8));

    QRectF rectCpuSocket(196.84, 126.924, 66, 66);
    QRectF rectRam1(334.510, 87.1261, 10, 183);
    QRectF rectRam2(375.395, 87.1261, 10, 183);
    QRectF rectGpuSlot(124.773, 321.613, 186, 8);
    QRectF rectPCI1(154.891, 378.622, 186, 8);
    QRectF rectPCI2(154.891, 419.955, 186, 8);

    if (rectCpuSocket.contains(mapToScene(event->pos()))) {
        emit cpuSocketClicked();
    } else if (rectRam1.contains(mapToScene(event->pos()))) {
        qDebug() << "RAM1 clicked()";
        emit ram1Clicked();
    } else if (rectRam2.contains(mapToScene(event->pos()))) {
        qDebug() << "RAM2 clicked()";
        emit ram2Clicked();
    } else if (rectGpuSlot.contains(mapToScene(event->pos()))) {
         qDebug() << "GPU clicked()";
         emit gpuSlotClicked();
    } else if (rectPCI1.contains(mapToScene(event->pos()))) {
        qDebug() << "PCI1 clicked()";
        emit pci1Clicked();
    } else if (rectPCI2.contains(mapToScene(event->pos()))) {
        qDebug() << "PCI2 clicked()";
        emit pci2Clicked();
    }

    //    if (path.contains(mapToScene(event->pos()))) {
    //        emit cpuSocketClicked();
    //    }
}
