
#include <QApplication>
#include <QLibrary>

#include "mainwindow.h"
#include "pythoninterop.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QCoreApplication::setOrganizationName("RayTrace");
    QCoreApplication::setOrganizationDomain("RayTrace");
    QCoreApplication::setApplicationName("RayTrace Experiment");

    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    return app.exec();
}
