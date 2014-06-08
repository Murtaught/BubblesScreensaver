#include "mainwidget.h"
#include "bubbleitem.h"
#include <QApplication>
#include <QTime>
#include <QScreen>
#include <QPixmap>

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsColorizeEffect>

#include <QList>
#include <QTimer>

// Количество пузырей на экране
const int BUBBLES_AMOUNT = 12;
const int EXPECTED_FPS   = 30;

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msec());
    QApplication app(argc, argv);

    BubbleItem::setBubblePixmap(QPixmap(":/images/bubble.png"));

    // Получаем скриншот
    QScreen *screen = QApplication::primaryScreen();
    if (!screen)
        return -1;

    QPixmap screenshot = screen->grabWindow(0);

    // QGraphicsScene - контейнер для создаваемых нами
    // объектов класса Bubble
    QGraphicsScene scene;
    scene.setSceneRect( screenshot.rect() );

    // Наполняем сцену непересекающимися элементами
    {
        QList<BubbleItem*> bubbles;

        for (int i = 0; i < BUBBLES_AMOUNT; ++i)
        {
            BubbleItem *bubble = new BubbleItem();
            scene.addItem(bubble);

            const int left   = scene.sceneRect().left()   + BubbleItem::RADIUS;
            const int top    = scene.sceneRect().top()    + BubbleItem::RADIUS;
            const int right  = scene.sceneRect().right()  - BubbleItem::RADIUS;
            const int bottom = scene.sceneRect().bottom() - BubbleItem::RADIUS;

            do
            {
                bubble->setPos(
                    left + qrand() % (right - left),
                    top  + qrand() % (bottom - top)
                );
            }
            while ( !scene.collidingItems(bubble).isEmpty() );
        }
    }

    // MainWidget - главный и единственный виджет этой программы,
    // он непосредственно рисует на экране элементы сцены, а также
    // обрабатывает нажатие клавиш и завершает выполнение программы при
    // нажатии клавиши ESC
    MainWidget view(&scene);
    view.setBackgroundPixmap(screenshot);
    view.setRenderHint(QPainter::Antialiasing);
    view.showFullScreen();

    // Используем QTimer для анимации движения пузырей. Сцена будет принудительно
    // обновляться EXPECTED_FPS раз в секунду
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    timer.start(1000 / EXPECTED_FPS);

    return app.exec();
}
