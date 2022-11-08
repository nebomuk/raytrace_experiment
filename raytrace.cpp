#include "raytrace.h"
#include "raycastresult.h"

#include <QImage>

#include <QPainter>
#include <QPoint>
#include <QtDebug>
#include <QtMath>
#include <QThread>
#include <QImage>

RayTrace::RayTrace(QObject *parent)
    : QObject{parent}
{

}

const QPoint NULL_POINT = QPoint(-1,-1);

RayCastResult RayTrace::start(const QImage * img, QPoint startPoint, RayTraceConfig config)
{
    RayCastResult res = startInternal(img,startPoint,config);
    return res;

}



void RayTrace::removeGapsInsidePolygon(const QPolygon& fillPolygon, QList<QLine> * gapLinesPointer)
{
    QList<QLine>& gapLines = *gapLinesPointer;
    QList<QLine>::iterator gi = gapLines.begin();
    while (gi != gapLines.end()) {
        QPolygon linePoly = QPolygon() << (*gi).p1() << (*gi).p2();
        if(fillPolygon.intersects(linePoly))
        {
            gi = gapLines.erase(gi); // only way iterator works for removing
        }
        else
        {
            gi++;
        }
    }
}

RayCastResult RayTrace::startInternal(const QImage * img, QPoint startPoint, RayTraceConfig config)
{
    qDebug() << "starting raycast" << startPoint;

    QPoint lastResultPoint = NULL_POINT;
    int lastRayLength = config.maxRayLength;

    QPoint lastGapStartPoint = NULL_POINT;

    QPolygon fillPolygon;
    QList<QLine> gapLines;

    QList<RayCastResult> recursiveResults;

    for (int i = 0; i < 360; i+=config.stepSize) {
        qreal sine = qSin(qDegreesToRadians((qreal)i));
        qreal cosine = qCos(qDegreesToRadians((qreal)i));


        int rayLength = lastResultPoint == NULL_POINT ?lastRayLength :  qRound(QLineF(startPoint,lastResultPoint).length());
        if(config.recursionDepth > 0 &&
                i > config.stepSize*2 &&
                fillPolygon.size() > 15 &&
                lastResultPoint != NULL_POINT &&
                qAbs(lastRayLength - rayLength) > config.minRayDifferenceForRecursion)
        {
            QPoint beforeLastPoint = fillPolygon[fillPolygon.length()-2];
            QPointF firstMidpoint = QLineF(startPoint,beforeLastPoint).center();
            QPoint center = QLineF(beforeLastPoint,firstMidpoint).center().toPoint();

            if(!checkIfLineArt(img, center.x(),center.y()) &&  (startPoint - center).manhattanLength() > config.minRecursionRayLength)
            {
                //qDebug() << "center" << center << " start Point: " << startPoint << " lastResultPoint " << lastResultPoint;
                RayTraceConfig configCopy = config;
                configCopy.recursionDepth -=1;
               RayCastResult res = start(img,center, configCopy);
                recursiveResults.append(res);
            }
        }

        lastRayLength = rayLength;

        int nextRayMaxLength = lastResultPoint.isNull() ? config.maxRayLength : qBound(config.minRayLength, rayLength+10,config.maxRayLength);

        //qDebug() << "manhattan " <<  manhattan <<"actual: "<< actualDistance;

        lastResultPoint = gbham(img, startPoint, startPoint + QPointF(sine*nextRayMaxLength,cosine*nextRayMaxLength).toPoint());

        if(lastResultPoint != NULL_POINT)
        {
            fillPolygon << lastResultPoint;
            if(fillPolygon.size() > 2)
            {
                if(lastGapStartPoint != NULL_POINT)
                {
                    gapLines << QLine(lastGapStartPoint, lastResultPoint);
                    lastGapStartPoint = NULL_POINT;
                }
            }
        }
        else if (lastResultPoint == NULL_POINT && lastGapStartPoint == NULL_POINT &&fillPolygon.size() > 2)
        {// FIXME first point not found handling
            lastGapStartPoint = fillPolygon.last();

        }
    }

    //removeGapsInsidePolygon(fillPolygon, &gapLines);

    QList<QPolygon> fillPolygonList;
    QList<QPoint> allStartPoints;
    fillPolygonList << fillPolygon;
    allStartPoints << startPoint;




    for (const RayCastResult &result : qAsConst(recursiveResults)) {
        fillPolygonList << result.fillPolygons();
        allStartPoints << result.startPoints();
        gapLines << result.gapLines();
    }



    RayCastResult result(fillPolygonList,allStartPoints,gapLines);
    return result;


}

/* signum function */
int RayTrace::sgn(int x)
{
    if (x > 0) return +1;
    if (x < 0) return -1;
    return 0;
}

bool RayTrace::checkIfLineArt(const QImage * image, int x, int y)
{
    if(image == nullptr)
    {
        return false;
    }

    QRgb color= image->pixel(x,y);
    //QRgb color2 = image->pixel(x+1,y-1);
   // QRgb color3 = image->pixel(x-1,y+1);
    QRgb colorWhite = qRgb(255,255,255);

    return color != colorWhite;
            //|| color2 != colorWhite;
            //|| color3 != colorWhite;

}

QPoint RayTrace::gbham(const QImage * image, int xstart, int ystart, int xend, int yend)
/*--------------------------------------------------------------
 * Bresenham-Algorithmus: Linien auf Rastergeräten zeichnen
 *
 * Eingabeparameter:
 *    int xstart, ystart        = Koordinaten des Startpunkts
 *    int xend, yend            = Koordinaten des Endpunkts
 *
 * Ausgabe:
 *    void SetPixel(int x, int y) setze ein Pixel in der Grafik
 *         (wird in dieser oder aehnlicher Form vorausgesetzt)
 *---------------------------------------------------------------
 */
{
    int x, y, t, dx, dy, incx, incy, pdx, pdy, ddx, ddy, deltaslowdirection, deltafastdirection, err;

    /* Entfernung in beiden Dimensionen berechnen */
    dx = xend - xstart;
    dy = yend - ystart;

    /* Vorzeichen des Inkrements bestimmen */
    incx = sgn(dx);
    incy = sgn(dy);
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    /* feststellen, welche Entfernung größer ist */
    if (dx > dy)
    {
        /* x ist schnelle Richtung */
        pdx = incx; pdy = 0;    /* pd. ist Parallelschritt */
        ddx = incx; ddy = incy; /* dd. ist Diagonalschritt */
        deltaslowdirection = dy;   deltafastdirection = dx;   /* Delta in langsamer Richtung, Delta in schneller Richtung */
    }
    else
    {
        /* y ist schnelle Richtung */
        pdx = 0;    pdy = incy; /* pd. ist Parallelschritt */
        ddx = incx; ddy = incy; /* dd. ist Diagonalschritt */
        deltaslowdirection = dx;   deltafastdirection = dy;   /* Delta in langsamer Richtung, Delta in schneller Richtung */
    }

    /* Initialisierungen vor Schleifenbeginn */
    x = xstart;
    y = ystart;
    err = deltafastdirection / 2;
    if(!image->valid(x,y))
    {
        return NULL_POINT;
    }
    else if(checkIfLineArt(image,x,y)){
        return QPoint(x,y);
    }
    //setPixel(x,y);


    /* Pixel berechnen */
    for(t = 0; t < deltafastdirection; ++t) /* t zaehlt die Pixel, deltafastdirection ist Anzahl der Schritte */
    {
        /* Aktualisierung Fehlerterm */
        err -= deltaslowdirection;
        if(err < 0)
        {
            /* Fehlerterm wieder positiv (>=0) machen */
            err += deltafastdirection;
            /* Schritt in langsame Richtung, Diagonalschritt */
            x += ddx;
            y += ddy;
        }
        else
        {
            /* Schritt in schnelle Richtung, Parallelschritt */
            x += pdx;
            y += pdy;
        }
        if(!image->valid(x,y))
        {
            return NULL_POINT;
        }
        else if(checkIfLineArt(image, x,y)){
            return QPoint(x,y);
        }
        //setPixel(x, y);

    }
    return NULL_POINT;
}

QPoint RayTrace::gbham(const QImage* image, QPoint start, QPoint end)
{
    return this->gbham(image, start.x(),start.y(),end.x(),end.y());
}

