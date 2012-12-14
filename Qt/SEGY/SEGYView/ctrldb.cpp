#include "ctrldb.h"

CtrlDB::CtrlDB()
{
    //startDB();
    query.exec("drop tables Trace");
    query.exec("create table Trace(fp int not null,x int not null,y int not null,st int not null,et int not null)");
}
CtrlDB::~CtrlDB()
{
    //query.exec( QObject::tr("drop tables Trace"));
    query.exec("drop tables Trace");
}
void CtrlDB::startDB()
{
//    if(!createConnection())
//    {
//        qDebug()<<"Cann't open the database!";
//        exit(0);
//    }
}

void CtrlDB::AddData(DBTrace t)
{
    QString str;
    str= QObject::tr("insert into Trace values (");
    str = str + QString::number(t.fp) +  QObject::tr(",");
    str = str + QString::number(t.in) +  QObject::tr(",");
    str = str + QString::number(t.cross) +  QObject::tr(",");
    str = str + QString::number(t.st) +  QObject::tr(",");
    str = str + QString::number(t.et) +  QObject::tr(")");
    query.exec(str);

}

std::vector<DBTrace> CtrlDB::Search(int x, QString flags)
{
    QString str =  QObject::tr("select * from Trace where ");
    str = str + flags +  QObject::tr(" = %1").arg(QString::number(x));

    std::vector<DBTrace> trace;
    query.exec(str);
    while( query.next() )
    {
        //qDebug()<<query.value(0).toInt();
        DBTrace t;
        t.fp = query.value(0).toInt();
        t.in = query.value(1).toInt();
        t.cross=query.value(2).toInt();
        t.st = query.value(3).toInt();
        t.et = query.value(4).toInt();

        trace.push_back(t);
    }

    return trace;
}
