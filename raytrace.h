#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "RayCastResult.h"

#include <QImage>
#include <QObject>
#include <QPoint>

class RayTrace : public QObject
{
    Q_OBJECT
public:
    explicit RayTrace(QObject *parent = nullptr);

public slots:
    RayCastResult start(QImage *image, QPoint startPoint, int recursionDepth);

    void debugDrawResult(QImage * image, QPoint startPoint, const RayCastResult& result);

signals:

private:
    QPoint gbham(int xstart, int ystart, int xend, int yend);

    QPoint gbham(QPoint start, QPoint end);

    static int sgn(int x);

    void setPixel(int x, int y);

    bool checkIfLineArt(int x, int y);

    QImage * image = nullptr;
};

#endif // RAYTRACE_H
