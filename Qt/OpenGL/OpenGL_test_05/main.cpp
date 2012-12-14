#include <QtGui/QApplication>
#include "glwidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    glutInit(&argc,argv);
    GLWidget w;
    w.show();

    return a.exec();
}
