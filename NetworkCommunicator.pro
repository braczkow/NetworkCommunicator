#-------------------------------------------------
#
# Project created by QtCreator 2015-03-29T22:57:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetworkCommunicator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    networkworkerthread.cpp

HEADERS  += mainwindow.h \
    networkworkerthread.h

FORMS    += mainwindow.ui
