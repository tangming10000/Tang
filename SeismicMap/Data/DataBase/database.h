#ifndef DATABASE_H
#define DATABASE_H

#include <QDebug>
#include <QtSql/qsql.h>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QList>
#include <QObject>
#include <QTime>
#include <QFile>
#include <QDataStream>

#include "../SEGY/segy.h"

bool createConnection_MySql();
bool createConnection_Sqlite(QString dbname);
void disConnection_Mysql();
void disConnection_Sqlite();

bool db_loadSEGYHead(QString filename);                    //加载segy地震数据到头信息
bool db_selectXLine(int xline,QList<int> *fpsize);         //检索X Line号,fpsize文件指针位置数组
bool db_selectYLine(int yline,QList<int> *fpsize);         //检索Y Line号,fpsize文件指针位置数组


void mysqlTestLoad();
void mysqlTestSelect();
void mysqlTestUpdate();


#endif // DATABASE_H
