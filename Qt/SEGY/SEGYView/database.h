#ifndef DATABASE_H
#define DATABASE_H
#include <QDebug>
#include <QtSql/qsql.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Tang");
    db.setUserName("tommy");
    db.setPassword("tangming");
    if( !db.open())
    {
        qDebug()<<"Open Database Error!\n"<<db.lastError();
        return false;
    }
    return true;
}



#endif // DATABASE_H
