#ifndef RAYCASTRESULT_H
#define RAYCASTRESULT_H

#include <QPolygon>
#include <QList>


class RayCastResult
{
public:
    RayCastResult(const QList<QPolygon>& fillPolygons, const QList<QPoint>& recursionStartPoints)
    {
        this->fillPolygons_ = fillPolygons;
        this->recursionStartPoints_ = recursionStartPoints;
    }



    const QList<QPolygon> &fillPolygons() const;

    const QList<QPoint> &recursionStartPoints() const;

private:

    QList<QPolygon> fillPolygons_;
    QList<QPoint> recursionStartPoints_;


};

inline const QList<QPolygon> &RayCastResult::fillPolygons() const
{
    return fillPolygons_;
}

inline const QList<QPoint> &RayCastResult::recursionStartPoints() const
{
    return recursionStartPoints_;
}


#endif // RAYCASTRESULT_H
