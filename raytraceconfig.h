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
      qRegisterMetaTypeStreamOperators<RayTraceConfig>();

    }

    int recursionDepth = 1;
    int stepSize = 1;
    int minRayLength = 5;
    int maxRayLength = 200;
    int minRecursionRayLength = 20;
    int minRayDifferenceForRecursion = 20;

    friend QDataStream &operator<<(QDataStream &out, const RayTraceConfig &myObj);

    friend QDataStream &operator>>(QDataStream &in, RayTraceConfig &myObj);


};
Q_DECLARE_METATYPE(RayTraceConfig);




#endif // RAYTRACECONFIG_H
