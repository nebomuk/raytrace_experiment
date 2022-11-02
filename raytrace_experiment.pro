QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = \
                mainwindow.h \
                raytrace.h \
                scribblearea.h
SOURCES       = main.cpp \
                mainwindow.cpp \
                raytrace.cpp \
                scribblearea.cpp

FORMS += \
    mainwindow.ui

