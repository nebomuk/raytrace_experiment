#ifndef PYTHONINTEROP_H
#define PYTHONINTEROP_H
#include "qobject.h"
#include <QPolygon>
#include <QtGlobal>

extern "C" QObject *push_qobject(void *obj);

#endif // PYTHONINTEROP_H
