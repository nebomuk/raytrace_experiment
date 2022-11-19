#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "raycastresult.h"
#include "raytraceconfig.h"

#include <QImage>
#include <QObject>
#include <QPoint>

class RayTrace : public QObject
{
    Q_OBJECT
public:
    explicit RayTrace(QObject *parent = nullptr);

public slots:

    RayCastResult start(const QImage *image, QPoint startPoint, RayTraceConfig config);



signals:

private:
    RayCastResult startInternal(const QImage *image, QPoint startPoint, RayTraceConfig config);


    QPoint gbham(const QImage * image,int xstart, int ystart, int xend, int yend);

    QPoint gbham(const QImage * image,QPoint start, QPoint end);

    static int sgn(int x);

    void setPixel(int x, int y);

    bool checkIfLineArt(const QImage *image, int x, int y);



    void removeGapsInsidePolygon(const QPolygon &fillPolygon, QList<QLine> *gapLines);
};

#endif // RAYTRACE_H
