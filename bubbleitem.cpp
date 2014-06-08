#include "bubbleitem.h"

#include <QDebug>
#include <QBrush>
#include <QGraphicsScene>
#include <QList>

#define _USE_MATH_DEFINES
#include <cmath>

static double RADS_IN_DEG = M_PI / 180.0;

const qreal  BubbleItem::RADIUS = 90; // радиус шаров в пикселях
const qreal  BubbleItem::COLORIZER_STRENGTH = 0.27; // чем больше этот параметр - тем ярче цвет пузырей
QPixmap     *BubbleItem::BUBBLE_PIXMAP = NULL; // картинка с мыльным пузырем

BubbleItem::BubbleItem()
    : speed( (qrand() % 300) / 10.0 ),
      color(qrand() % 256, qrand() % 256, qrand() % 256),
      can_bounce(true)
{
    setRotation(qrand() % 360);

    effect = new QGraphicsColorizeEffect();
    effect->setColor(color);
    effect->setStrength(COLORIZER_STRENGTH);
    QGraphicsItem::setGraphicsEffect(effect);
}

QRectF BubbleItem::boundingRect() const
{
    return QRectF(-RADIUS, -RADIUS, 2 * RADIUS, 2 * RADIUS);
}

QPainterPath BubbleItem::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void BubbleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->rotate( -rotation() );

    if (BUBBLE_PIXMAP != NULL)
        painter->drawPixmap(boundingRect(), *BUBBLE_PIXMAP, BUBBLE_PIXMAP->rect());

    // Пример дополнительной декорации
    //painter->setBrush(Qt::Dense7Pattern);
    //painter->drawEllipse(boundingRect());

    painter->restore();
}

void BubbleItem::setBubblePixmap(const QPixmap &pixmap)
{
    BUBBLE_PIXMAP = new QPixmap(pixmap);
}

void BubbleItem::advance(int step)
{
    if (step == 0)
        return;

    QList<QGraphicsItem*> collisions = scene()->collidingItems(this);

    if ( can_bounce )
    {
        // Обрабатаем столновения с другими шарами
        if ( !collisions.isEmpty() )
        {
            BubbleItem *other = static_cast<BubbleItem*>( collisions.first() );

            // Мы считаем массы шаров одинаковыми, а столкновение
            // абсолютно упругим. Решение очень простое:
            // нужно поменять векторы скоростей местами
            qreal other_rot = other->rotation();
            other->setRotation(this->rotation());
            setRotation(other_rot);

            std::swap( this->speed, other->speed );

            // Предотвращаем "склеивание" шаров
            can_bounce = false;
        }
    }
    else
        if ( collisions.isEmpty() )
            can_bounce = true;

    // Проверим столкновения с краями экрана
    const QRectF scene_rect = scene()->sceneRect();
    const qreal x = QGraphicsItem::pos().x();
    const qreal y = QGraphicsItem::pos().y();

    QPointF vec( std::cos(RADS_IN_DEG * rotation()),
                 std::sin(RADS_IN_DEG * rotation()));

    if (x - RADIUS < scene_rect.left())
        vec.setX( qAbs(vec.x()) );

    if (x + RADIUS > scene_rect.right())
        vec.setX( -qAbs(vec.x()) );

    if (y - RADIUS < scene_rect.top())
        vec.setY( qAbs(vec.y()) );

    if (y + RADIUS > scene_rect.bottom())
        vec.setY( -qAbs(vec.y()) );

    qreal new_angle = std::acos(vec.x()) / RADS_IN_DEG;
    if ( vec.y() < 0 )
        new_angle = 360.0 - new_angle;

    setRotation(new_angle);

    // Сделаем шаг
    setPos( mapToScene(speed, 0) );
}


