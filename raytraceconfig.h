#ifndef RAYTRACECONFIG_H
#define RAYTRACECONFIG_H

#include <QList>
#include <QMetaType>
#include <QDataStream>

struct RayTraceConfig
{
public :

    RayTraceConfig()
    {
      #if QT_VERSION < 0x060000
      qRegisterMetaTypeStreamOperators<RayTraceConfig>();
      #endif

    }

    int recursionDepth = 1;
    qreal stepSize = 1;
    int minRayLength = 5;
    int maxRayLength = 200;
    int minRecursionRayLength = 20;
    int minRayDifferenceForRecursion = 20;
    int maxConsecutiveRayLength = 10;

    friend QDataStream &operator<<(QDataStream &out, const RayTraceConfig &myObj);

    friend QDataStream &operator>>(QDataStream &in, RayTraceConfig &myObj);


};
Q_DECLARE_METATYPE(RayTraceConfig);




#endif // RAYTRACECONFIG_H
