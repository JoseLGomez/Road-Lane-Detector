#-------------------------------------------------
#
# Project created by QtCreator 2014-02-24T13:07:29
#
#-------------------------------------------------
TEMPLATE = lib
TARGET = Tracking
QT += core
CONFIG += staticlib c++11
CONFIG( release, debug|release ){ QMAKE_CXXFLAGS += -Ofast -flto -fwhole-program -march=native -fopenmp} else { QMAKE_CXXFLAGS += -march=native -fopenmp }
DEFINES +=  QT_DLL _MBCS
INCLUDEPATH += $$PWD/src \
    $$PWD/../ComnUtils/src \
    $$PWD/../Image/src \
    $$PWD/../Cameras/src \
    $$PWD/../RoiRefinement/src \
    $$PWD/../../External/Maths/Lapack/src \
    $$PWD/../../External/Maths/qsl/src \
    $$PWD/../../External/Maths/qsl/deps/qsl
win32{
    INCLUDEPATH += $$PWD/../../External/ComputerVision/opencv/include/opencv2 \
    $$PWD/../../External/ComputerVision/opencv/include
} else {
    INCLUDEPATH += /usr/local/include/opencv \
    /usr/local/opencv/include
}
DEPENDPATH += .
win32:   DESTDIR = ../../../Win32/
unix:    DESTDIR = ../../../Unix/
LIBPATH += $$OUT_PWD/$$DESTDIR
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
CONFIG( debug, debug|release ){
    DESTDIR = $${DESTDIR}Debug/Lib
    OBJECTS_DIR = debug
    MOC_DIR += ./GeneratedFiles/debug
} else {
    DESTDIR = $${DESTDIR}Release/Lib
    OBJECTS_DIR = release
    MOC_DIR += ./GeneratedFiles/relesase
}
LIBS += -lQSL
include(src/Tracking.pri)

