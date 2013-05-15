HEADERS       = renderarea.h \
    svgpathparser.h \
    mainwindow.h
SOURCES       = main.cpp \
                renderarea.cpp \
    svgpathparser.cpp \
    mainwindow.cpp
RESOURCES     = basicdrawing.qrc
QT           += xml
QMAKE_CXXFLAGS += -std=c++11

# install
target.path = $$[QT_INSTALL_EXAMPLES]/painting/basicdrawing
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS basicdrawing.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/painting/basicdrawing
INSTALLS += target sources

symbian {
    TARGET.UID3 = 0xA000A649
    include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
}
maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

simulator: warning(This example might not fully work on Simulator platform)

FORMS += \
    mainwindow.ui
