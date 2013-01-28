#include "database.h"
QSqlDatabase db;
bool createConnection_MySql()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("SeisMap");
    db.setUserName("seismap");
    db.setPassword("tangming");
    if( !db.open())
    {
        qDebug()<<"Open Database Error!\n"<<db.lastError();
        return false;
    }
    return true;
}
bool createConnection_Sqlite(QString dbname)
{
    const QString DBname = dbname;
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":memory:");
    db.setDatabaseName(DBname);
    if (!db.open()) {
        qDebug()<<"Open Database Error!\n"<<db.lastError();
        return false;
    }
    return true;
}

void disConnection_Mysql()
{
    QString name = db.database().connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(name);
}

void disConnection_Sqlite()
{
    if( db.isOpen() )
    {
        QString name = db.database().connectionName();
        db.close();
        db = QSqlDatabase();
        db.removeDatabase(name);
    }
}

bool db_loadSEGYHead(QString filename)
{
    //创建数据库表文件
    QSqlQuery query;
    query.exec("drop table if exists SeisHead");
    //query.exec("create table if not exists SeisHead(fp integer, Xin integer, Ycross integer, Z float) engine=myisam");
    query.exec("create table if not exists SeisHead(fp integer, Xin integer, Ycross integer, Z float)");
    query.exec("create index Seis_index on SeisHead(fp,Xin,Ycross)");
    QByteArray bt = filename.toLatin1();
    char* fname = bt.data();
    FILE* filesegy;
    if ((filesegy = fopen(fname,"rb")) == NULL )
    {
        printf("open segy file error!");
        return false;
    }
    REEL Reel;
    SEGYHEAD segyhead;
    ReadREEL(filesegy,&Reel);
    ReadSEGYHead(filesegy,&Reel,&segyhead);


    long TotalTraces  = segyhead.TotalTraces;
    int DataType = segyhead.DataType;
    int HNS = Reel.hns;

    query.exec("begin transaction");

    //设置进度条
    //QProgressBar *progress = new QProgressBar();
    //progress->setRange(1,TotalTraces);

    QTime time;
    time.start();
    long fp = 0L;
    int Xin, Ycross, Z;
    Xin = Ycross = Z = 0;
    for( long i = 0; i < TotalTraces; i++ )
    //for( long i = 0; i < 10; i++ )
    {
        fp =  qint64(3608L  + i*(HNS*DataType + 240L));
        fseek(filesegy,fp,SEEK_SET);
        fread((void*)(&Xin),sizeof(int),1,filesegy);
        Xin = swap_int32(Xin);

        fseek(filesegy,fp+12L,SEEK_SET);
        fread((void*)(&Ycross),sizeof(int),1,filesegy);
        Ycross = swap_int32(Ycross);
        QString str = QObject::tr("insert into SeisHead values(%1,%2,%3,%4)").arg(i).arg(Xin).arg(Ycross).arg(0);
        if(!query.exec(str))
        {
            qDebug()<<"Load head data to database Error!!!"<<query.lastError();
            return false;
        }
        //progress->setValue(i+1);
    }
    query.exec("commit");
    qDebug()<<QObject::tr("The time of load data is %1s.").arg(time.elapsed()/1000.0);
    fclose(filesegy);
    return true;

}

bool db_selectXLine(int xline, QList<int> *fpsize)
{
    fpsize->clear();
    QSqlQuery query;
    if(!query.exec(QObject::tr("select fp from SeisHead where Xin=%1").arg(xline)))
    {
        qDebug()<<"Search the XLine data Error!!"<<query.lastError();
        return false;
    }
    else
    {
        while (query.next() )
        {
            fpsize->append(query.value(0).toInt());
        }
    }
    return true;

}

bool db_selectYLine(int yline, QList<int> *fpsize)
{
    fpsize->clear();
    QSqlQuery query;
    if(!query.exec(QObject::tr("select fp from SeisHead where Ycross=%1").arg(yline)))
    {
        qDebug()<<"Search the XLine data Error!!"<<query.lastError();
        return false;
    }
    else
    {
        while (query.next() )
        {
            fpsize->append(query.value(0).toInt());
        }
    }
}

void mysqlTestLoad()
{
    QTime time;
    time.start();
    QSqlQuery query;
    query.exec("drop table test");
    //query.exec("create table test(fp integer,X integer, Y integer,Z float) engine=myisam");
    query.exec("create table test(fp bigint ,X int, Y int,Z float) engine=myisam");
    //query.exec("create index X_index on test(X,Y)");
    query.exec("begin transaction");
    for( int i = 0; i < 1000; i++)
    {
        for( int j = 0; j < 1000; j++)
        {
            query.exec(QObject::tr("insert into test values(%1,%2,%3,%4)").arg((i*1000)+j).arg(i+1000).arg(j+2300).arg(0));
        }
    }
    query.exec("commit");
    qDebug()<<QObject::tr("The Total Load time is %1s.").arg(time.elapsed()/1000.0);
}

void mysqlTestSelect()
{
    QTime time;
    time.start();
    QSqlQuery query;
    query.exec("begin transaction");
    for( int i = 0; i < 1000; i++ )
    {
        query.exec(QObject::tr("select * from test where X=%1").arg(i+1000));
    }
    query.exec("commit");
     qDebug()<<QObject::tr("The Select 1000 times X total time is %1s.").arg(time.elapsed()/1000.0);
     time.start();
     query.exec("begin transaction");
     for( int i = 0; i < 10; i++ )
     {
         query.exec(QObject::tr("select * from test where Y=%1").arg(i+2300));
     }
     query.exec("commit");
      qDebug()<<QObject::tr("The Select 1000 times Y total time is %1s.").arg(time.elapsed()/1000.0);
}

void mysqlTestUpdate()
{
    QTime time;
    time.start();
    QSqlQuery query;
    query.exec("begin transaction");
    for( int i = 0; i < 1000; i++ )
    {
        query.exec(QObject::tr("update test set Z=1 where X=%1 and Y=%2 ").arg(1000+i).arg(2299-i));
    }
    query.exec("commit");
    qDebug()<<QObject::tr("The update 1000 times Z total time is %1s.").arg(time.elapsed()/1000.0);
}
