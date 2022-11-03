QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = \
                RayCastResult.h \
                mainwindow.h \
                raytrace.h \
                scribblearea.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                raytrace.cpp \
                scribblearea.cpp

FORMS += \
    mainwindow.ui

