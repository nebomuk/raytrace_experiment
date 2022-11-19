TEMPLATE = lib

QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = \
                src/debugdraw.h \
                src/mainwindow.h \
                src/preferences.h \
                src/pythoninterop.h \
                src/raycastresult.h \
                src/raytrace.h \
                src/raytraceconfig.h \
                src/scribblearea.h
SOURCES       = src/main.cpp \
                src/debugdraw.cpp \
                src/mainwindow.cpp \
                src/preferences.cpp \
                src/pythoninterop.cpp \
                src/raytrace.cpp \
                src/raytraceconfig.cpp \
                src/scribblearea.cpp

FORMS += \
    ui/mainwindow.ui \
    ui/preferences.ui

RESOURCES += \
    resource.qrc

