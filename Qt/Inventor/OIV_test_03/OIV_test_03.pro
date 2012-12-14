#-------------------------------------------------
#
# Project created by QtCreator 2011-08-25T19:55:13
#
#-------------------------------------------------

QT       += core gui opengl
win32 {
DEFINES  += QT_DLL COIN_DLL SOQT_DLL
LIBS     += -LD:\SoftWare\QTSDK\Coin3D\lib
LIBS     += -lcoin3 -lcoin3d -lcoin3s -lcoin3sd  -lsoqt1 -lsoqt1s -lsoqt1d -lsoqt1sd
INCLUDEPATH += D:\SoftWare\QTSDK\Coin3D\include
}
unix {
LIBS     += -lCoin -lSoQt
}

TARGET = OIV_test_03
TEMPLATE = app


SOURCES += main.cpp\
        myexaminerviewer.cpp

HEADERS  += myexaminerviewer.h
