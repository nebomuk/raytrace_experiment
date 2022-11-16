#include "calculate.h"
#include <QVariant>

#include <QPolygon>


extern "C" Q_DECL_EXPORT QObject * calc(void * obj)
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
    obj1->setProperty("test","tstvalue");
    obj1->setProperty("vartest",*p);

    return obj1;


}

