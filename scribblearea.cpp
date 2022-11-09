#include "debugdraw.h"
#include "scribblearea.h"

#include <QMouseEvent>
#include <QPainter>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#include <QtDebug>
#include <QMenu>
#include <QSettings>
#include <QVariant>
#include <QMessageBox>
#include <QColor>
#endif
#endif

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    myPenColor = QSettings().value("pen_color",QColor(Qt::red)).value<QColor>();
    brushColor_ = QSettings().value("brush_color",QColor(Qt::red)).value<QColor>();


}

bool ScribbleArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    return false;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    debugDrawImage.fill(Qt::transparent);
    modified = true;
    update();
}

void ScribbleArea::clearDebugDraw()
{
    debugDrawImage.fill(Qt::transparent);
    modified = true;
    update();
}

void ScribbleArea::startRayTraycing(const QPoint &point)
{
    RayTrace raytrace;
    RayTraceConfig config = QSettings().value("ray_trace_config",QVariant::fromValue(RayTraceConfig())).value<RayTraceConfig>();

    QImage combinedImage = this->image.copy();
    QPainter p(&combinedImage);
    p.drawImage(combinedImage.rect(), debugDrawImage);
    p.end();
    RayCastResult res = raytrace.start(&combinedImage,point,config);
    DebugDraw debugDraw(myPenColor,brushColor_);

    if(!res.fillPolygons().isEmpty())
    {
        if(QSettings().value("debug_draw_polygon",true).toBool()){
            debugDraw.polygon(&debugDrawImage,res);

        }
        if(QSettings().value("debug_draw_rays",false).toBool()){
            debugDraw.rays(&debugDrawImage,res);

        }
        if(QSettings().value("debug_draw_recursive_start_points",false).toBool()){
            debugDraw.startPoints(&debugDrawImage,res);

        }
        if(QSettings().value("debug_draw_gaps",false).toBool()){
            debugDraw.gaps(&debugDrawImage,res);

        }
        if(QSettings().value("debug_draw_flood_fill_points",false).toBool()){
            debugDraw.floodFillPoints(&debugDrawImage,res);

        }
        this->update();
    }
    else
    {
        emit statusMessage(tr("could not fill the specified area, try somewhere else"));
    }


}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(!qFuzzyCompare(scale,1.0))
    {
        QMessageBox::information(this, tr("Reset zoom"),
                                       tr("Drawing and filling not allowed when zoomed"),
                                       QMessageBox::Ok);
        return;

    }

    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();

        if(fillEnabled_)
        {
            auto point = event->pos();
           startRayTraycing(point);
        }
        else if(floodFillEnabled_)
        {
            DebugDraw debugDraw(myPenColor,brushColor_);
            debugDraw.floodFill(&image, &debugDrawImage,lastPoint,brushColor_);
            update();
        }
        else
        {
            scribbling = true;
        }
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());

    if(scribbling)
    {
       // qDebug() << "x: " << event->pos().x() << " y: " << event->pos().y();
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.translate(translation);
    painter.scale(scale,scale);
    QRect dirtyRect = event->rect();
   // painter.drawImage(QPoint(0,0),image);
    painter.drawImage(dirtyRect, image, dirtyRect);
    painter.drawImage(0,0,debugDrawImage);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));

        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::wheelEvent(QWheelEvent *event)
{
    auto delta = event->angleDelta().y();
    scale +=(delta/120);
    if(scale < 0)
    {
        scale = 1.0;
    }
    translation = delta <= 0.0 ? QPointF() :  - this->mapFromGlobal(event->globalPosition().toPoint());
    update();

}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32_Premultiplied);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);

    debugDrawImage = QImage(newSize, QImage::Format_ARGB32_Premultiplied);
    debugDrawImage.fill(qRgba(0,0,0,0));

    painter.drawImage(0,0,debugDrawImage);

    painter.end();

    *image = newImage;

}

void ScribbleArea::setFillEnabled(bool newFillEnabled)
{
    fillEnabled_ = newFillEnabled;
}

void ScribbleArea::setFloodFillEnabled(bool newFillEnabled)
{
    floodFillEnabled_ = newFillEnabled;

}

void ScribbleArea::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}

void ScribbleArea::resetZoom()
{
    scale = 1.0;
    translation = QPointF();
    update();
}

QColor ScribbleArea::brushColor() const
{
    return brushColor_;
}

void ScribbleArea::setBrushColor(const QColor &newBrushColor)
{
    QSettings().setValue("brush_color",newBrushColor);
    brushColor_ = newBrushColor;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    QSettings().setValue("pen_color",newColor);
    myPenColor = newColor;
}
