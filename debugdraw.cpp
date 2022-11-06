#include "debugdraw.h"

#include <QPainter>

DebugDraw::DebugDraw(QObject *parent)
    : QObject{parent}
{

}

void DebugDraw::polygon(QImage *image, QPoint startPoint, const RayCastResult &result)
{
    for (const QPolygon &fillPolygon : result.fillPolygons()) {
       QPainter paint(image);
       paint.setRenderHints(QPainter::Antialiasing );
       paint.setPen(Qt::red);
       paint.setBrush(Qt::red);
       paint.drawPolygon(fillPolygon);
       paint.end();
   }
}

void DebugDraw::recursiveStartPoints(QImage *image, QPoint startPoint, const RayCastResult &result)
{
    for (QPoint p: qAsConst(result.recursionStartPoints())) {
        QPainter paint(image);
        paint.setPen(Qt::green);
        paint.setBrush(Qt::green);
        //paint.drawEllipse(p,5,5);
        paint.drawLine(p,startPoint);
        paint.end();

    }
}

void DebugDraw::rays(QImage *image, QPoint startPoint, const RayCastResult &result)
{
    for (QPolygon fillPolygon : result.fillPolygons()) {
        for (QPoint p: qAsConst(fillPolygon)) {
          //  QThread::msleep(20);

                QPainter paint(image);
                paint.setPen(Qt::red);
                paint.drawLine(startPoint,p);
                paint.end();
        }
    }

}
