#-------------------------------------------------
#
# Project created by QtCreator 2011-06-06T21:10:21
#
#-------------------------------------------------

QT       += core gui opengl sql mysql

LIBS     += -lGLU

TARGET = SEGYView
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    segy.cpp \
    glwidget.cpp \
    tracewidget.cpp \
    tracewidgetdraw.cpp \
    ctrldb.cpp

HEADERS  += mainwindow.h \
    segy.h \
    glwidget.h \
    tracewidget.h \
    tracewidgetdraw.h \
    ctrldb.h \
    database.h
