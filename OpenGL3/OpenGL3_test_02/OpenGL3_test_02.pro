#-------------------------------------------------
#
# Project created by QtCreator 2013-01-27T18:57:20
#
#-------------------------------------------------

QT       += core

LIBS     += -lglfw -lGLEW

TARGET = OpenGL3_test_02
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

OTHER_FILES += \
    SimpleVertexShader.vertexshader \
    SimpleFragmentShader.fragmentshader

RESOURCES += \
    Shader.qrc
