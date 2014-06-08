#ifndef BUBBLEITEM_H
#define BUBBLEITEM_H

#include <QGraphicsItem>
#include <QGraphicsColorizeEffect>
#include <QPixmap>
#include <QColor>
#include <QRectF>
#include <QPainterPath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class BubbleItem : public QGraphicsItem
{
public:
    static const qreal  RADIUS;
    static const qreal  COLORIZER_STRENGTH;

public:
    BubbleItem();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    static void setBubblePixmap(const QPixmap &pixmap);

protected:
    void advance(int step);

private:
    static QPixmap *BUBBLE_PIXMAP;

    qreal speed;
    QColor color;
    QGraphicsColorizeEffect *effect;
    bool can_bounce;
};

#endif // BUBBLEITEM_H
