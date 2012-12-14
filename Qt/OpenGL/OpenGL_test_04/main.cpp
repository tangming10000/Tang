#include <QtGui/QApplication>
#include "glwidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_WS_X11
    glutInit(&argc,argv);
#endif
    GLWidget w;
    w.show();

    return a.exec();
}
