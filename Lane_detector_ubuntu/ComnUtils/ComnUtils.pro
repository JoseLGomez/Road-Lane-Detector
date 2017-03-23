#-------------------------------------------------
#
# Project created by QtCreator 2014-02-19T13:33:42
#
#-------------------------------------------------
TEMPLATE = lib
TARGET = ComnUtils
QT += core
CONFIG += staticlib c++11
CONFIG( release, debug|release ){ QMAKE_CXXFLAGS += -Ofast -flto -fwhole-program -march=native -fopenmp} else { QMAKE_CXXFLAGS += -march=native -fopenmp } -mavx
DEFINES +=  QT_DLL _MBCS
DEFINES -= UNICODE
INCLUDEPATH += $$PWD/../Image/src \
    $$PWD/../ParamHandling/src \
    $$PWD/../ComnUtils/src \
    $$PWD/../../External/Lapack/src \
    $$PWD/../StereoImage/src \
    C:\Program Files\MATLAB\R2010a\extern\include \
    C:\Program Files\MATLAB\R2010b\extern\include
win32{
    INCLUDEPATH += $$PWD/../../External/ComputerVision/opencv/include/opencv2 \
    $$PWD/../../External/ComputerVision/opencv/include
} else {
    INCLUDEPATH += /usr/local/include/opencv2 \
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
LIBS += -lLapack
include(src/ComnUtils.pri)

