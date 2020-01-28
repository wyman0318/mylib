#-------------------------------------------------
#
# Project created by QtCreator 2020-01-15T11:08:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camera_demo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    caminfo.c \
    mycamera.cpp \
    videostream_jpg.c \
    rfid.c

HEADERS  += widget.h \
    caminfo.h \
    mycamera.h \
    videostream_jpg.h \
    iso14443a.h \
    rfid.h

FORMS    += widget.ui

INCLUDEPATH += /home/gec/abc/include
LIBS += -L/home/gec/abc/lib/ -ljpeg
