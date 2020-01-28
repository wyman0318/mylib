#-------------------------------------------------
#
# Project created by QtCreator 2019-12-11T10:39:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plantEvent
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    form.cpp \
    mainwindow.cpp \
    tty.c \
    mytty.cpp

HEADERS  += widget.h \
    form.h \
    mainwindow.h \
    tty.h \
    mytty.h

FORMS    += widget.ui \
    form.ui \
    mainwindow.ui
