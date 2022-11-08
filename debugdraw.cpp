#include "debugdraw.h"

#include <QPainter>

DebugDraw::DebugDraw(QObject *parent)
    : QObject{parent}
{

}

void DebugDraw::polygon(QImage *image, const RayCastResult &result)
{
    for (const QPolygon &fillPolygon : result.fillPolygons()) {
       QPainter paint(image);
       //paint.setRenderHints(QPainter::Antialiasing );
       paint.setPen(Qt::red);
       //paint.setBrush(Qt::red);
       paint.setBrush(QColor::fromHsv(50,220,220,120));
       paint.drawPolygon(fillPolygon, Qt::WindingFill);
       paint.end();
   }
}

void DebugDraw::startPoints(QImage *image, const RayCastResult &result)
{
    for (QPoint p: qAsConst(result.startPoints())) {
        QPainter paint(image);
        paint.setPen(Qt::green);
        paint.setBrush(Qt::green);
        paint.drawEllipse(p,5,5);
        paint.end();

    }
}

void DebugDraw::rays(QImage *image, const RayCastResult &result)
{
    for (int i = 0; i < result.startPoints().size(); ++i) {
        QPolygon fillPolygon = result.fillPolygons()[i];
        for (int j = 0; j < qAsConst(fillPolygon).size(); ++j) {
            QPainter paint(image);
            QColor color = QColor::fromHsv(i % 255,220,200);

            paint.setPen(Qt::red);
            paint.drawLine(result.startPoints()[i],fillPolygon[j]);
            paint.end();
        }

    }
}

void DebugDraw::gaps(QImage *image, const RayCastResult &result)
{
    for (QLine gapLine: qAsConst(result.gapLines())) {
        QPainter paint(image);
        paint.setPen(Qt::green);
        paint.setBrush(Qt::green);
        paint.drawLine(gapLine);
        //paint.drawEllipse(gapLine.p2(),5,5);
        paint.end();

    }
}
