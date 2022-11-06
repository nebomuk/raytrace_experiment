#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "raycastresult.h"

#include <QObject>

class DebugDraw : public QObject
{
    Q_OBJECT
public:
    explicit DebugDraw(QObject *parent = nullptr);

public slots:
    void polygon(QImage * image, QPoint startPoint, const RayCastResult& result);

    void recursiveStartPoints(QImage * image, QPoint startPoint, const RayCastResult& result);

    void rays(QImage * image, QPoint startPoint, const RayCastResult& result);



signals:

};

#endif // DEBUGDRAW_H
