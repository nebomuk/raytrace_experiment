#include "raytrace.h"

#include <QImage>

#include <QPainter>
#include <QPoint>
#include <QtDebug>
#include <QtMath>

RayTrace::RayTrace(QObject *parent)
    : QObject{parent}
{

}

void RayTrace::start(QImage * img, QPoint pos)
{
    qDebug() << "starting raycast" << pos;
    this->image = img;


   // gbham(pos, pos + QPointF(30,25).toPoint());
   // gbham(pos, pos + QPointF(20,50).toPoint());

    bool DEBUG_RAY_DRAWING = true;
    bool DEBUG_POLYGON_DRAWING = false;

    QPoint nextResultPoint;
    int maxDistance = 200;
    int previousManhattan = maxDistance;

    QPolygon fillPolygon;

    for (int var = 0; var < 360; var+=2) {

        int minDistance = 10;
        qreal sine = qSin(qDegreesToRadians((qreal)var));
        qreal cosine = qCos(qDegreesToRadians((qreal)var));

        int manhattan = nextResultPoint == /*not found*/ QPoint(-1, -1) ?previousManhattan :  (pos - nextResultPoint).manhattanLength();
        previousManhattan = manhattan;

        int actualDistance = nextResultPoint.isNull() ? maxDistance : qBound(minDistance, manhattan+10, maxDistance);

        qDebug() << "manhattan " <<  manhattan <<"actual: "<< actualDistance;

        nextResultPoint = gbham(pos, pos + QPointF(sine*actualDistance,cosine*actualDistance).toPoint());

        if(nextResultPoint != QPoint(-1,-1))
        {
            fillPolygon << nextResultPoint;
        }
        if(DEBUG_RAY_DRAWING)
        {
            if(nextResultPoint != QPoint(-1,-1))
            {
                QPainter paint(image);
                paint.setPen(Qt::red);
                paint.drawLine(pos,nextResultPoint);
                paint.end();
            }
        }

    }

    if(DEBUG_POLYGON_DRAWING)
    {
        QPainter paint(image);
        paint.setPen(Qt::red);
        paint.setBrush(Qt::red);
        paint.drawPolygon(fillPolygon);
        paint.end();
    }


}

/* signum function */
int sgn(int x)
{
    if (x > 0) return +1;
    if (x < 0) return -1;
    return 0;
}

void RayTrace::setPixel(int x, int y)
{
    if(image != nullptr)
    {
        image->setPixel(x,y,QColor(Qt::red).rgba());
    }
}

bool RayTrace::checkIfLineArt(int x, int y)
{
    if(image == nullptr)
    {
        return false;
    }

    QRgb color= image->pixel(x,y);
    QRgb colorWhite = qRgb(255,255,255);

    return color != colorWhite && color != QColor(Qt::red).rgba();

}

QPoint RayTrace::gbham(int xstart, int ystart, int xend, int yend)
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
    if(checkIfLineArt(x,y)){
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
        if(checkIfLineArt(x,y)){
            return QPoint(x,y);
        }
        //setPixel(x, y);

    }
    return QPoint(-1,-1);
}

QPoint RayTrace::gbham(QPoint start, QPoint end)
{
    return this->gbham(start.x(),start.y(),end.x(),end.y());
}

