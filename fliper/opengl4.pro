#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T23:07:51
#
#-------------------------------------------------

QT       += core gui opengl\
            multimedia

LIBS    += -lOpengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opengl4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oglwidget.cpp

HEADERS  += mainwindow.h \
    oglwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
