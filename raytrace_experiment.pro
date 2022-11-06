QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = \
                debugdraw.h \
                mainwindow.h \
                preferences.h \
                raycastresult.h \
                raytrace.h \
                raytraceconfig.h \
                scribblearea.h
SOURCES       = main.cpp \
                debugdraw.cpp \
                mainwindow.cpp \
                preferences.cpp \
                raytrace.cpp \
                raytraceconfig.cpp \
                scribblearea.cpp

FORMS += \
    mainwindow.ui \
    preferences.ui

RESOURCES += \
    resource.qrc

