#-------------------------------------------------
#
# Project created by QtCreator 2011-08-29T17:17:15
#
#-------------------------------------------------

QT       += core gui opengl

win32{
LIBS     += -lglut32 -lGLU32
}
unix {
LIBS     += -lglut -lglu
}
TARGET = OpenGL_test_03
TEMPLATE = app


SOURCES += main.cpp\
        glwidget.cpp

HEADERS  += glwidget.h
