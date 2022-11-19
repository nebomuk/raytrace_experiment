#include "pythoninterop.h"
#include <QVariant>

#include <QPolygon>
#include "debugdraw.h"
#include "mainwindow.h"
#include "raytrace.h"

extern "C" Q_DECL_EXPORT QObject * push_qobject(void * obj)
{
    QPolygon * p = new QPolygon();
    QPolygon p2;
    p2 << QPoint(1,1) << QPoint(-1,-3) << QPoint(2,5);
    p->swap(p2);

    if(obj == nullptr)
    {
        return nullptr;
    }
    QObject * obj1 = (QObject*)obj;
    obj1->setProperty("test","beta");
    obj1->setProperty("vartest",*p);
    QVariant imageVariant = obj1->property("image");
    QVariant startPointVariant = obj1->property("startPoint");

//    MainWindow *window = new MainWindow();

//    if(imageVariant.isValid())
//    {
//        QImage image = imageVariant.value<QImage>();
//        window->openQImage(image);

//    }
//    window->show();

    RayTrace raytrace;

    QImage image = imageVariant.value<QImage>();
    QPoint startPoint = startPointVariant.toPoint();
    RayCastResult res = raytrace.start(&image,startPoint,RayTraceConfig());
    DebugDraw debugDraw(Qt::red,Qt::red);
    debugDraw.polygon(&image,res);
    obj1->setProperty("filledImage",image); // detaches from origianl QImage, and copies by value ?

    return obj1;


}

