#-------------------------------------------------
#
# Project created by QtCreator 2015-03-01T13:26:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Trident
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    spline.cpp \
    canvas.cpp \
    tabwidget.cpp

HEADERS  += mainwindow.h \
    spline.h \
    canvas.h \
    tabwidget.h

FORMS    += mainwindow.ui
