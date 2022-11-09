#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "raycastresult.h"

#include <QColor>
#include <QObject>

class DebugDraw : public QObject
{
    Q_OBJECT
public:
    explicit DebugDraw(QObject *parent = nullptr);

public slots:
    void polygon(QImage * image, const RayCastResult& result);

    void startPoints(QImage * image, const RayCastResult& result);

    void rays(QImage * image, const RayCastResult& result);

    void gaps(QImage * image, const RayCastResult& result);

    void floodFillPoints(QImage * image, const RayCastResult& result);

    void floodFill(const QImage * referenceImage,QImage * imageToDrawOn, QPoint seed, QColor color);



 private:




signals:

};

#endif // DEBUGDRAW_H
