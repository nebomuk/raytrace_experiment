#include "raytrace.h"

#include <QImage>

#include <QPainter>
#include <QPoint>
#include <QtDebug>
#include <QtMath>
#include <QThread>

RayTrace::RayTrace(QObject *parent)
    : QObject{parent}
{

}

const QPoint NOT_FOUND = QPoint(-1,-1);


void RayTrace::start(QImage * img, QPoint startPoint, bool recursion)
{
    qDebug() << "starting raycast" << startPoint;
    this->image = img;


   // gbham(pos, pos + QPointF(30,25).toPoint());
   // gbham(pos, pos + QPointF(20,50).toPoint());

    bool DEBUG_RAY_DRAWING = false;
    bool DEBUG_POLYGON_DRAWING = true;


    QPoint lastResultPoint;
    int maxDistance = 200;
    int lastManhattan = maxDistance;

    QPolygon fillPolygon;

    for (int i = 0; i < 360; i+=1) {


        int minDistance = 5;
        qreal sine = qSin(qDegreesToRadians((qreal)i));
        qreal cosine = qCos(qDegreesToRadians((qreal)i));



        int manhattan = lastResultPoint == /*not found*/ QPoint(-1, -1) ?lastManhattan :  (startPoint - lastResultPoint).manhattanLength();
        if(recursion && i > 2 && lastResultPoint != NOT_FOUND && qAbs(lastManhattan - manhattan) > 20)
        {
            QPoint beforeLastPoint = fillPolygon[fillPolygon.length()-2];
            QPointF firstMidpoint = QPointF(startPoint+beforeLastPoint)/2.0;
            QPoint center = ((beforeLastPoint+ firstMidpoint)/2.0).toPoint();

            if((startPoint - center).manhattanLength() > 20)
            {
                qDebug() << "center" << center << " start Point: " << startPoint << " lastResultPoint " << lastResultPoint;
                QPainter paint(image);
                paint.setPen(Qt::green);
                paint.setBrush(Qt::green);
                paint.drawEllipse(center,5,5);
                paint.end();
                start(img,center, false);
            }


        }

        lastManhattan = manhattan;

        int actualDistance = lastResultPoint.isNull() ? maxDistance : qBound(minDistance, manhattan+10, maxDistance);

        //qDebug() << "manhattan " <<  manhattan <<"actual: "<< actualDistance;

        lastResultPoint = gbham(startPoint, startPoint + QPointF(sine*actualDistance,cosine*actualDistance).toPoint());

        if(lastResultPoint != NOT_FOUND)
        {
            fillPolygon << lastResultPoint;
        }
    }

    if(DEBUG_RAY_DRAWING)
    {
        for (QPoint p: qAsConst(fillPolygon)) {
                QPainter paint(image);
                paint.setPen(Qt::red);
                paint.drawLine(startPoint,p);
                paint.end();
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

    return color != colorWhite && color != QColor(Qt::red).rgba() && color != QColor(Qt::green).rgba();

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
    if(!image->valid(x,y))
    {
        return NOT_FOUND;
    }
    else if(checkIfLineArt(x,y)){
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
        else if(checkIfLineArt(x,y)){
            return QPoint(x,y);
        }
        //setPixel(x, y);

    }
    return NOT_FOUND;
}

QPoint RayTrace::gbham(QPoint start, QPoint end)
{
    return this->gbham(start.x(),start.y(),end.x(),end.y());
}

