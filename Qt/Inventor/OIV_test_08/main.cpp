#include <QtGui/QApplication>
#include "myexaminerviewer.h"

int main(int argc, char *argv[])
{
    QWidget * window = SoQt::init(argv[0]);

    MyExaminerViewer * viewer = new MyExaminerViewer(window);

    //viewer->setSceneGraph(new SoCone);
    viewer->show();

    SoQt::show(window);
    SoQt::mainLoop();

    delete viewer;
    return 0;
}
