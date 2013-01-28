#-------------------------------------------------
#
# Project created by QtCreator 2012-12-08T22:56:25
#
#-------------------------------------------------

QT       += core gui sql opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
LIBS     += -lglu32 -lglut32
}
unix {
LIBS     += -lGLU -lGL
}

TARGET = SeismicMap
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Data/DataBase/database.cpp \
    Data/DataManager/datamanager.cpp \
    Data/SEGY/showtrace.cpp \
    Data/SEGY/segy.cpp \
    Data/SEGY/showslice.cpp \
    Interface/GLWidget/glwidget.cpp \
    Interface/MapView/mapview.cpp \
    Interface/MapView/mapitem.cpp \
    Interface/SeisView/seisview.cpp \
    Interface/SeisView/seistrace.cpp \
    Interface/SeisView/seislabel.cpp \
    Interface/SeisView/seisback.cpp \
    Interface/SeisView/seiswidget.cpp

HEADERS  += mainwindow.h \
    Data/DataBase/database.h \
    Data/DataManager/datamanager.h \
    Data/SEGY/showtrace.h \
    Data/SEGY/segy.h \
    Data/SEGY/showslice.h \
    Interface/GLWidget/glwidget.h \
    Interface/MapView/mapview.h \
    Interface/MapView/mapitem.h \
    Interface/SeisView/seisview.h \
    Interface/SeisView/seistrace.h \
    Interface/SeisView/seislabel.h \
    Interface/SeisView/seisback.h \
    Interface/SeisView/seiswidget.h

RESOURCES += \
    icons.qrc
