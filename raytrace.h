#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <QImage>
#include <QObject>
#include <QPoint>

class RayTrace : public QObject
{
    Q_OBJECT
public:
    explicit RayTrace(QObject *parent = nullptr);

public slots:
    void start(QImage *image, QPoint pos, bool recursion);

signals:

private:
    QPoint gbham(int xstart, int ystart, int xend, int yend);

    QPoint gbham(QPoint start, QPoint end);


    void setPixel(int x, int y);

    bool checkIfLineArt(int x, int y);

    QImage * image = nullptr;
};

#endif // RAYTRACE_H
