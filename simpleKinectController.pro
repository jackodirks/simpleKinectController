#-------------------------------------------------
#
# Project created by QtCreator 2013-04-23T11:37:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

TARGET = simpleKinectController
TEMPLATE = app
INCLUDEPATH += "C:/Program Files/Microsoft SDKs/Kinect/v1.7/inc"
INCLUDEPATH += "C:\Program Files (x86)\Windows Kits\8.0"
LIBS += -L"C:/Program Files/Microsoft SDKs/Kinect/v1.7/lib/amd64/" -lKinect10
LIBS += -lOpenGL32

SOURCES += main.cpp\
        MainWindow.cpp \
    Kinect.cpp \
    KinectManager.cpp \
    OpenGLWidget.cpp

HEADERS  += MainWindow.h \
    Kinect.h \
    KinectManager.h \
    OpenGLWidget.h

FORMS    += MainWindow.ui





