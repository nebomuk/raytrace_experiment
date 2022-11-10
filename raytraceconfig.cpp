#include "raytraceconfig.h"


QDataStream &operator<<(QDataStream &out, const RayTraceConfig &myObj)
{
    out << myObj.maxRayLength << myObj.minRayLength << myObj.minRecursionRayLength \
        << myObj.minRayDifferenceForRecursion << myObj.recursionDepth << myObj.stepSize;
    return out;
}

QDataStream &operator>>(QDataStream &in, RayTraceConfig &myObj)
{
    in >> myObj.maxRayLength;
    in >> myObj.minRayLength;
    in >> myObj.minRecursionRayLength;
    in >> myObj.minRayDifferenceForRecursion;
    in >> myObj.recursionDepth;
    in >> myObj.stepSize;
    return in;
}
