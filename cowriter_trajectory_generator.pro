TARGET = svg_subsampler
HEADERS       = src/renderarea.h \
    src/svgpathparser.h \
    src/mainwindow.h \
    src/trajsampler.h \
    src/bezierpath.h
SOURCES       = src/main.cpp \
                src/renderarea.cpp \
    src/svgpathparser.cpp \
    src/mainwindow.cpp \
    src/bezierpath.cpp \
    src/trajsampler.cpp
QT           += xml
QT += widgets
QT += core gui
QMAKE_CXXFLAGS += -std=c++11

FORMS += \
    ui/mainwindow.ui
