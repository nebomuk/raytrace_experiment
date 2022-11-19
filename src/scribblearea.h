#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include "raytrace.h"

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

//! [0]
class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool openQImage(QImage image);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

    QColor brushColor() const;
    void setBrushColor(const QColor &newBrushColor);

    void setFillEnabled(bool newFillEnabled);
    void setFloodFillEnabled(bool newFillEnabled);


signals:
    void statusMessage(QString str);


public slots:
    void clearImage();
    void clearDebugDraw();
    void print();
    void resetZoom();


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent( QWheelEvent * event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void startRayTraycing(const QPoint& point);

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 2;
    bool fillEnabled_ = false;
    bool floodFillEnabled_ = false;
    QColor myPenColor;
    QColor brushColor_;
    QImage image;
    QImage debugDrawImage;
    QPoint lastPoint;

    qreal scale = 1.0;
    QPointF translation;

    QImage createCombinedImage();
};
//! [0]

#endif
