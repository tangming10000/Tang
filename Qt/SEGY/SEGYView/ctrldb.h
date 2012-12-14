#ifndef CTRLDB_H
#define CTRLDB_H
#include <Qt/QtGui>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
struct DBTrace
{
    int fp;
    int in;
    int cross;
    int st;
    int et;
};
class CtrlDB
{
public:
    CtrlDB();
    ~CtrlDB();
    void AddData(DBTrace t);
    std::vector<DBTrace> Search(int x, QString flags);
    void startDB();
private:
    std::vector<DBTrace> trace;
    QTableView *view;
    QSqlQuery query;
    QSqlQueryModel* model;

};

#endif // CTRLDB_H
