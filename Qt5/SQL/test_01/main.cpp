/**
  @mainpage
  @author tangming10000@126.com
  */

#include <QCoreApplication>
#include <QtSql/QSql>
#include <QtSql>
#include <QDebug>
#include <QSqlQuery>
#include <QStringList>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /**
      check the Qt supported databases drivers.
      */
    qDebug()<<"The total Qt database supported drivers are:";
    QStringList drivers = QSqlDatabase::drivers();
    foreach (QString driver, drivers) {
        qDebug() << driver;
    }


    //connect to the postgresql database
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("test_db");
    db.setUserName("tommy");
    db.setHostName("localhost");
    db.setPassword("tangming");
    if(!db.open())
    {
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
