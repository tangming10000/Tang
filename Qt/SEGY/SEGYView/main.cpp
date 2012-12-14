#include <QtGui/QApplication>
#include "mainwindow.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//    if( !createConnection() )
//        return 1;
    MainWindow w;
    w.show();

    return a.exec();
}
