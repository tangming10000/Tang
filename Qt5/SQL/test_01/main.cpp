#include <QCoreApplication>
#include <QtSql/QSql>
#include <QtSql>
#include <QDebug>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("test_db");
    db.setUserName("tommy");
    db.setHostName("localhost");
    db.setPassword("tangming");
    if(!db.open())
    {
//        QMessageBox::information(this, "信息提示", "数据库连接失败.",
//                                        QMessageBox::Yes | QMessageBox::No,
//                                        QMessageBox::Yes);
        qDebug()<<"据库连接失败."<<db.lastError().text();
        db.close();
        exit(EXIT_FAILURE);
    }
    else
    {
        qDebug()<<"database connect success.";
    }

    QSqlQuery query;
    query.exec("select * from weather");
    qDebug()<<query.size()<<query.ValuesAsColumns<<query.ValuesAsRows;
    while( query.next())
    {

        qDebug()<<query.value(0).toString()<<query.value(1).toInt()
               <<query.value(2).toInt()<<query.value(3).toFloat()<<query.value(4).toDate();
    }

    return a.exec();
}
