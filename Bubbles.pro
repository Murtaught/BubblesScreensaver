#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T19:37:10
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bubbles
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    bubbleitem.cpp

HEADERS  += mainwidget.h \
    bubbleitem.h

RESOURCES += \
    resources.qrc
