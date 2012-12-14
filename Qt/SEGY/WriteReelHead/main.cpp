#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QVector>
#include <QDebug>

#include "segy.h"
class REEL;
class TRACE;
int main(int argc, char *argv[])
{
    QString infile1,infile2,outfile1;
//    QString str1 =  QString(QLatin1String(argv[1]));
//    QString str2 =  QString(QLatin1String(argv[2]));
//    QString str3 =  QString(QLatin1String(argv[3]));
    //QString str0 = ("/home/tommy/Program/test_01-build-desktop-Qt_4_8_1___PATH________/rho_cube.sgy");
    QString str1 = ("/home/tommy/Program/test_01-build-desktop-Qt_4_8_1___PATH________/stack_0_18.segy");
    //QString str2 = ("/home/tommy/Program/test_01-build-desktop-Qt_4_8_1___PATH________/rho_cube.dat");
    //QString str3 = ("/home/tommy/Program/test_01-build-desktop-Qt_4_8_1___PATH________/rho_cube_C.sgy");
    QString str2 = ("/home/tommy/low_3d_1.sgy");
    QString str3 = ("/home/tommy/data_little.dat");

    infile1 = str1;
    infile2 = str2;
    outfile1 = str3;

    REEL Reel;
    TRACE Trace;

//    QFile loadfile0(str0);
//    loadfile0.open(QIODevice::ReadOnly);
//    QDataStream lsr(&loadfile0);
//    lsr.setByteOrder(QDataStream::BigEndian);

    QFile loadfile1(infile1);
    loadfile1.open(QIODevice::ReadOnly);
    QDataStream lsh(&loadfile1);
    lsh.setByteOrder(QDataStream::BigEndian);

    QFile loadfile2(infile2);
    loadfile2.open(QIODevice::ReadOnly);
    QDataStream lsd(&loadfile2);
    lsd.setByteOrder(QDataStream::BigEndian);

    QFile outfile(outfile1);
    if ( outfile.exists() )
        outfile.remove(outfile1);
    outfile.open(QIODevice::WriteOnly);
    QDataStream osd(&outfile);
    osd.setByteOrder(QDataStream::BigEndian);

//    unsigned char Head = NULL;
//    lsd.device()->seek(0);
//    osd.device()->seek(0);
//    for( int i = 0; i < 3200; i++)
//    {
//        lsd>>Head;
//        osd<<Head;
//    }

    lsd.device()->seek(3200);
//    osd.device()->seek(3200);
    Reel.ReadREEL(lsd);
    Reel.format = 1;
    Reel.WriteREEL(osd);

//    int HNS = Reel.hns;
//    int DataType = Reel.DataType;
//    Reel.TotalTraces = loadfile2.size()/float(HNS)/float(DataType);
//    qDebug()<<"Total Traces:"<<Reel.TotalTraces;

//    int NN = Reel.TotalTraces;
//    qint64 fp = 0;

//    int flags = Reel.format;

//    float data[HNS];
//    float temp = 0;
//    memset(&data,HNS,0);
//    for( qint64 i = 0; i < NN; i ++)
//    {
//        fp =  qint64(3600  + i*(HNS*DataType + 240));
//        lsh.device()->seek(fp);
//        Trace.ReadTRACE(lsh);          //读取道头
//        osd.device()->seek(fp);
//        Trace.WriteTRACE(osd);         //写入道头

//        for( int j = 0; j < HNS; j++)
//        {
//            lsd>>temp;
//            //qDebug()<<temp;
//            data[j] = temp;
//        }

//        fp = fp + 240;
//        osd.device()->seek(fp);
//        WriteDataATP(osd,data,HNS,flags);
//    }


    qDebug()<<"Done!!!";
    return 0;
}
