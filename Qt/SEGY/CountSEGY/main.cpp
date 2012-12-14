#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QVector>
#include "segy.h"
class REEL;

struct DATA
{
    int angle;
    int line;
    int cdp;
};
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString infile,outfile;
//    infile = argv[1];
//    outfile = argv[2];
    infile = "/media/Tang/402_487.segy";
    outfile = "/home/Tang/out";

    REEL Reel;
    QFile loadfile(infile);
    loadfile.open(QIODevice::ReadOnly);
    QDataStream ls(&loadfile);
    ls.setByteOrder(QDataStream::BigEndian);
    ls.device()->seek(3200);
    Reel.ReadREEL(ls);
    int HNS = Reel.hns;
    int DataType = Reel.DataType;


    int t = Reel.hns*Reel.hdt/1000;
    int angle,line,cdp;
    DATA dd;
    QVector<DATA> Data;
    qint64 fp;

    for( qint64 i = 0; i < Reel.TotalTraces; i ++)
    {
        fp =  qint64(3600  + i*(HNS*DataType + 240));
        ls.device()->seek(fp+36);
        ls>>angle;
        if (angle <= 20)
        {
            ls.device()->seek(fp+180);
            ls>>line>>cdp;
            dd.angle = angle;
            dd.line = line;
            dd.cdp =cdp;
            Data.append(dd);
        }
    }
    qDebug()<<Data.size();

    QFile angle_file(outfile+"_angle.txt");
    angle_file.open(QIODevice::WriteOnly);
    QTextStream sa(&angle_file);

    QFile line_file(outfile+"_line.txt");
    line_file.open(QIODevice::WriteOnly);
    QTextStream sl(&line_file);

    QFile cdp_file(outfile+"_cdp.txt");
    cdp_file.open(QIODevice::WriteOnly);
    QTextStream sc(&cdp_file);

    for( int  i = 0; i < Data.size(); i++)
    {
        sa<<Data[i].angle;
        sl<<Data[i].line;
        sl<<Data[i].cdp;
    }
    
    return a.exec();
}
