#ifndef RAYCASTRESULT_H
#define RAYCASTRESULT_H

#include <QLine>
#include <QPolygon>
#include <QList>


class RayCastResult
{
public:
    RayCastResult(const QList<QPolygon> &fillPolygons, const QList<QPoint> &startPoints,const  QList<QLine> &gapLines);

    const QList<QPolygon> &fillPolygons() const;

    const QList<QPoint> &startPoints() const;

    const QList<QLine> &gapLines() const;

private:

    QList<QPolygon> fillPolygons_;
    QList<QPoint> startPoints_;
    QList<QLine> gapLines_;

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

inline const QList<QLine> &RayCastResult::gapLines() const
{
    return gapLines_;
}

inline RayCastResult::RayCastResult(const QList<QPolygon> &fillPolygons, const QList<QPoint> &startPoints, const QList<QLine> &gapLines) : fillPolygons_(fillPolygons),
    startPoints_(startPoints),
    gapLines_(gapLines)
{}



#endif // RAYCASTRESULT_H



