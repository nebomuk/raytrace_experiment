#include "debugdraw.h"
#include "qdebug.h"

#include <QLinkedList>
#include <QPainter>
#include <QSet>
#include <QVector>
#include <deque>

DebugDraw::DebugDraw(const QColor &penColor, const QColor &brushColor, QObject *parent)
    : QObject{parent}, penColor_(penColor), brushColor_(brushColor)
{

}

void DebugDraw::polygon(QImage *image, const RayCastResult &result)
{
    QPolygon united;
      for (const QPolygon &fillPolygon : result.fillPolygons()) {
          united = united.united(fillPolygon);

         }
             QPainter paint(image);
             //paint.setRenderHints(QPainter::Antialiasing );
             paint.setPen(penColor_);
             //paint.setBrush(Qt::red);
             paint.setBrush(brushColor_);
             paint.drawPolygon(united, Qt::WindingFill);

             paint.end();
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

            paint.setPen(penColor_);
            paint.drawLine(result.startPoints()[i],fillPolygon[j]);
            paint.end();
        }

    }
}

void DebugDraw::gaps(QImage *image, const RayCastResult &result)
{
    for (QPoint gapPoint: qAsConst(result.gapPoints())) {
        QPainter paint(image);
        paint.setPen(Qt::green);
        paint.setBrush(Qt::green);
        //paint.drawPoint(gapLine);
        paint.drawEllipse(gapPoint,2,2);
        paint.end();

    }
}

void DebugDraw::floodFillPoints(QImage *image, const RayCastResult &result)
{
    QPolygon united;
      for (const QPolygon &fillPolygon : result.fillPolygons()) {
          united = united.united(fillPolygon);

      }

       for (QPoint p: qAsConst(united)) {

       }


      QPainter paint(image);
      //paint.setRenderHints(QPainter::Antialiasing );
      paint.setPen(penColor_);
      //paint.setBrush(Qt::red);
      paint.setBrush(brushColor_);
      paint.drawPolygon(united, Qt::WindingFill);
      paint.end();
}

void DebugDraw::floodFill(const QImage * referenceImage,QImage * imageToDrawOn, QPoint seed, QColor color)
{
    const int width = referenceImage->width();
        const int height = referenceImage->height();

        // const cast to remove warning
        QRgb* bits = reinterpret_cast<QRgb*>(const_cast<unsigned char*>(referenceImage->bits()));

        const auto getPixel = [&](int x, int y) {
            return bits[(y * width) + x];
        };

        const QRgb oldRgba = getPixel(seed.x(), seed.y());

        std::vector<unsigned char> processedAlready(width * height);

        std::deque<QPoint> backlog = { seed };
        std::deque<QPoint> points;

        int maxIterations = INT_MAX;
        int counter = 0;

        while (!backlog.empty() && counter++ < maxIterations) {
            const QPoint& point = backlog.front();
            const int x = point.x();
            const int y = point.y();
            if (x >= 0 && y >= 0 && x < width && y < height) {
                const QRgb rgba = getPixel(x, y);
                if (rgba == oldRgba) {
                    unsigned char& isProcessedAlready = processedAlready[(y * width) + x];
                    if (!isProcessedAlready) {
                        isProcessedAlready = true;
                        points.push_back(point);
                        backlog.push_back(QPoint(x - 1, y));
                        backlog.push_back(QPoint(x, y - 1));
                        backlog.push_back(QPoint(x + 1, y));
                        backlog.push_back(QPoint(x, y + 1));
                    }
                }
            }
            backlog.pop_front();
        }

      QPainter painter(imageToDrawOn);
      painter.setPen(color);
      for (QPoint p: points) {
          painter.drawPoint(p);

      }
      painter.end();
}
