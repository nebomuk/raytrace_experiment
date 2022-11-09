#ifndef RAYCASTRESULT_H
#define RAYCASTRESULT_H

#include <QLine>
#include <QPolygon>
#include <QList>


class RayCastResult
{
public:
    RayCastResult(const QList<QPolygon> &fillPolygons, const QList<QPoint> &startPoints,const  QList<QPoint> &gapPoints);

    const QList<QPolygon> &fillPolygons() const;

    const QList<QPoint> &startPoints() const;

    const QList<QPoint> &gapPoints() const;

private:

    QList<QPolygon> fillPolygons_;
    QList<QPoint> startPoints_;
    QList<QPoint> gapPoints_;

};

// generated getters and ctor

inline const QList<QPolygon> &RayCastResult::fillPolygons() const
{
    return fillPolygons_;
}

inline const QList<QPoint> &RayCastResult::startPoints() const
{
    return startPoints_;
}

inline const QList<QPoint> &RayCastResult::gapPoints() const
{
    return gapPoints_;
}

inline RayCastResult::RayCastResult(const QList<QPolygon> &fillPolygons, const QList<QPoint> &startPoints, const QList<QPoint> &gapPoints) : fillPolygons_(fillPolygons),
    startPoints_(startPoints),
    gapPoints_(gapPoints)
{}



#endif // RAYCASTRESULT_H



