#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsObject>

class Item : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit Item(QRectF rect, QGraphicsItem *parent = nullptr);
    // QGraphicsItem interface
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

signals:
    void hovered(bool);
    void cpuSocketClicked();
    void ram1Clicked();
    void ram2Clicked();
    void gpuSlotClicked();
    void pci1Clicked();
    void pci2Clicked();

    // QGraphicsItem interface
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPainterPath path;
    QColor background;
};

#endif // ITEM_H
