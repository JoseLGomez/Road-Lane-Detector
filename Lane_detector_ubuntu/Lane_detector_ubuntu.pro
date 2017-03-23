QT += core
QT -= gui

CONFIG += c++11
CONFIG += -O3
CONFIG += -floop-parallelize-all
CONFIG += -ftree-parallelize-loops=8

TARGET = Lane_detector
CONFIG += console
CONFIG -= app_bundle


TEMPLATE = app

SOURCES += main.cpp \
    LaneDetectorManager.cpp \
    StraightLaneDetector.cpp \
    Lanedetector.cpp \
    Tracking/src/Kalman/OpenCVKalman.cpp \
    Tracking/src/Track.cpp \
    calculusofsupport.cpp \
    Homography.cpp \
    procesamientodenivelbajo.cpp \
    ComnUtils/src/Utils/IniFileIO.cpp \
    ComnUtils/src/Utils/Utils.cpp \
    CurveLaneDetector.cpp



INCLUDEPATH += .
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include/opencv2/core
INCLUDEPATH += /usr/local/include/opencv2/highgui
INCLUDEPATH += /usr/local/include/opencv2/imgproc
INCLUDEPATH += /usr/local/include/opencv2/flann
INCLUDEPATH += /usr/local/include/opencv2/photo
INCLUDEPATH += /usr/local/include/opencv2/video
INCLUDEPATH += /usr/local/include/opencv2/features2d
INCLUDEPATH += /usr/local/include/opencv2/objdetect
INCLUDEPATH += /usr/local/include/opencv2/calib3d
INCLUDEPATH += /usr/local/include/opencv2/ml
INCLUDEPATH += /usr/local/include/opencv2/contrib

LIBS += `pkg-config opencv --cflags --libs`

DISTFILES +=

HEADERS += \
    LaneDetectorManager.h \
    StraightLaneDetector.h \
    Lanedetector.h \
    Tracking/src/Kalman/OpenCVKalman.h \
    Tracking/src/Track.h \
    calculusofsupport.h \
    Homography.h \
    procesamientodenivelbajo.h \
    ComnUtils/src/Utils/IniFileIO.h \
    CurveLaneDetector.h


