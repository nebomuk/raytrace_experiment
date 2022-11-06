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

const QPoint NOT_FOUND = QPoint(-1,-1);


RayCastResult RayTrace::start(const QImage * img, QPoint startPoint, RayTraceConfig config)
{
    qDebug() << "starting raycast" << startPoint;

    QPoint lastResultPoint;
    int lastManhattan = config.maxRayLength;

    QPolygon fillPolygon;
    QList<QPoint> recursionStartPoints;

    QList<RayCastResult> recursiveResults;

    for (int i = 0; i < 360; i+=config.stepSize) {
        qreal sine = qSin(qDegreesToRadians((qreal)i));
        qreal cosine = qCos(qDegreesToRadians((qreal)i));



        int manhattan = lastResultPoint == NOT_FOUND ?lastManhattan :  (startPoint - lastResultPoint).manhattanLength();
        if(config.recursionDepth > 0 &&
                i > config.stepSize*2 &&
                fillPolygon.size() > 15 &&
                lastResultPoint != NOT_FOUND &&
                qAbs(lastManhattan - manhattan) > config.minRayDifferenceForRecursion)
        {
            QPoint beforeLastPoint = fillPolygon[fillPolygon.length()-2];
            QPointF firstMidpoint = QLineF(startPoint,beforeLastPoint).center();
            QPoint center = QLineF(beforeLastPoint,firstMidpoint).center().toPoint();

            if(!checkIfLineArt(img, center.x(),center.y()) &&  (startPoint - center).manhattanLength() > config.minRecursionRayLength)
            {
                //qDebug() << "center" << center << " start Point: " << startPoint << " lastResultPoint " << lastResultPoint;
                recursionStartPoints << center;
                RayTraceConfig configCopy = config;
                configCopy.recursionDepth -=1;
               RayCastResult res = start(img,center, configCopy);
                recursiveResults.append(res);
            }
        }

        lastManhattan = manhattan;

        int actualDistance = lastResultPoint.isNull() ? config.maxRayLength : qBound(config.minRayLength, manhattan+10,config.maxRayLength);

        //qDebug() << "manhattan " <<  manhattan <<"actual: "<< actualDistance;

        lastResultPoint = gbham(img, startPoint, startPoint + QPointF(sine*actualDistance,cosine*actualDistance).toPoint());

        if(lastResultPoint != NOT_FOUND)
        {
            fillPolygon << lastResultPoint;
        }
    }

    QList<QPolygon> fillPolygons;
    fillPolygons << fillPolygon;

    for (const RayCastResult &result : recursiveResults) {
        fillPolygons << result.fillPolygons();
        recursionStartPoints << result.recursionStartPoints();
    }

    RayCastResult result(fillPolygons,recursionStartPoints);
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
        return NOT_FOUND;
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
            return NOT_FOUND;
        }
        else if(checkIfLineArt(image, x,y)){
            return QPoint(x,y);
        }
        //setPixel(x, y);

    }
    return NOT_FOUND;
}

QPoint RayTrace::gbham(const QImage* image, QPoint start, QPoint end)
{
    return this->gbham(image, start.x(),start.y(),end.x(),end.y());
}

