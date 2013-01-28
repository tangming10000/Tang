#include "datamanager.h"

DataManager::DataManager()
{
    Current_Slice_Index = 0;
    //setDataBase();
    InitFile();
    setMemory();
    setDefaultValues();
    ReadSEGYData();
    //GetSurface(800);
}

DataManager::~DataManager()
{
    qDebug()<<"Release the DataManager's memory!\n";
    delete Reel;
    delete segyHead;
    delete mapset;
    delete seisset;
    qDeleteAll(Total_Slices);
    fclose(filesegy);
    surface.clear();
}

void DataManager::WriteSetting()
{

}

void DataManager::ReadSetting()
{

}

void DataManager::setDefaultValues()
{
    //设置MapSetting
    mapset->Max_XLine = 4100;
    mapset->Min_XLine = 3200;
    mapset->Max_YLine = 2250;
    mapset->Min_YLine = 1400;
    mapset->color_Receiver = Qt::blue;
    mapset->color_Receiver_Selected = Qt::red;
    mapset->size_Receiver = 4;
    mapset->XInterval = 200;
    mapset->YInterval = 200;
    mapset->XScale = 1.0;
    mapset->YScale = 1.0;

    //设置SeisSetting
    seisset->dx = 4.0;
    seisset->dy = 2.0;
    seisset->gain = 2.5;
    seisset->lx = 60;
    seisset->ly = 60;
    seisset->color_Line = Qt::black;
    seisset->width_Line = 1;
    seisset->color_Area = Qt::black;
    seisset->grid_XInterval = 50;
    seisset->grid_YInterval = 100;
    seisset->color_grid = Qt::red;
    seisset->sample_interval = Reel->hns;
    seisset->seisType = WaveArea;
    seisset->XScale = 1.0;
    seisset->YScale = 1.0;
}

bool DataManager::InitFile()
{
#ifdef WIN32
    filename = QObject::tr("E:/SEGY_Data/dynp_dq.sgy");
    dbname = QObject::tr("C:/Users/Tang/test.db");
#else
    filename = QObject::tr("/media/Backup/SEGY_Data/dynp_dq.sgy");
    dbname = QObject::tr("/home/tommy/test.db");
#endif
    QFile dbfile(dbname);
    if(dbfile.size() < 10)
    {
        db_loadSEGYHead(filename);
        qDebug()<<"The database is not exits!Create a new one!";
    }

    QByteArray bt = filename.toLatin1();
    char* fname = bt.data();
    if ((filesegy = fopen(fname,"rb")) == NULL )
    {
        printf("open segy file error!");
        return false;
    }
    return true;
}

void DataManager::setMemory()
{
    Reel = new REEL;
    segyHead = new SEGYHEAD;
    ReadREEL(filesegy,Reel);
    ReadSEGYHead(filesegy,Reel,segyHead);
    Total_Slices.clear();

    mapset = new MapSetting;
    seisset= new SeisSetting;
//    qDebug()<<Reel->hns<<segyHead->TotalTraces;
}

bool DataManager::setDataBase()
{
    //if(!createConnection_MySql())
	return false;

}

void DataManager::ReadSEGYData()
{
    int fp;         //当前道在地震数据体中位置
    long vfp = 0;   //文件指针位置
    long tacelong = segyHead->DataType*segyHead->hns+240L;
    int Index = 0;  //当前道的序列号
    int size = Total_Slices.size();    

    ShowSlice *Current_Slice = new ShowSlice(size);

    QString str = QObject::tr("select fp from SeisHead where Xin=%1").arg(QString::number(3300));
    if(!query.exec(str))
    {
        qDebug()<<"Get data from SeisHead error!\n";
        //exit(-1);
        return ;
    }
    while( query.next() )
    {
        fp = query.value(0).toInt();
        vfp = ((long)fp*tacelong + 3600L);
        ShowTrace* trace = new ShowTrace(Index,filesegy,segyHead,vfp);
        Current_Slice->slice.push_back(trace);
        Index++;
    }
    Total_Slices.push_back(Current_Slice);

//    qDebug()<<Current_Slice->slice.size();

    Current_Slice = new ShowSlice(size);
    str = QObject::tr("select fp from SeisHead where Ycross=%1").arg(QString::number(2000));
    if(!query.exec(str))
    {
        qDebug()<<"Get data from SeisHead error!\n";
        //exit(-1);
        return ;
    }
    while( query.next() )
    {
        fp = query.value(0).toInt();
        vfp = ((long)fp*tacelong + 3600L);
        ShowTrace* trace = new ShowTrace(Index,filesegy,segyHead,vfp);
        Current_Slice->slice.push_back(trace);
        Index++;
    }
    Total_Slices.push_back(Current_Slice);

    Current_Slice = new ShowSlice(size);
    str = QObject::tr("select fp from SeisHead where Ycross=%1").arg(QString::number(1600));
    if(!query.exec(str))
    {
        qDebug()<<"Get data from SeisHead error!\n";
        //exit(-1);
        return ;
    }
    while( query.next() )
    {
        fp = query.value(0).toInt();
        vfp = ((long)fp*tacelong + 3600L);
        ShowTrace* trace = new ShowTrace(Index,filesegy,segyHead,vfp);
        Current_Slice->slice.push_back(trace);
        Index++;
    }
    Total_Slices.push_back(Current_Slice);


    qDebug()<<"Read data from segy have done!";
}

void DataManager::GetSurface( int ztime)
{
    Surface sf;
    sf.z = ztime;
    long trace_size = (long)(240L+segyHead->DataType*segyHead->hns);
    int datatype = segyHead->DataType;
    long fp = 0L;
    for( int i = 0; i < segyHead->TotalTraces; i += 10 )
    {
        fp = ( 3600L + (long)i*trace_size);
        fseek(filesegy,fp,SEEK_SET);
        fread((void*)(&sf.x+8L),sizeof(int),1,filesegy);
        sf.x = swap_int32(sf.x);

        fseek(filesegy,fp+20L,SEEK_SET);
        fread((void*)(&sf.y),sizeof(int),1,filesegy);
        sf.y = swap_int32(sf.y);

        fseek(filesegy,fp+datatype*sf.z+240L,SEEK_SET);
        if( segyHead->format == 1 )
        {
            fread((void*)(&sf.value),sizeof(float),1,filesegy);
            sf.value = ibm2float(sf.value);
        }
        else if( segyHead->format == 2)
        {
            int temp = 0;
            fread((void*)(&temp),sizeof(int),1,filesegy);
            sf.value = swap_int32(temp);
        }
        else if( segyHead->format == 3 )
        {
            short temp = 0;
            fread((void*)(&temp),sizeof(short),1,filesegy);
            sf.value = swap_int16(temp);
        }
        else if( segyHead->format == 5 )
        {
            fread((void*)(&sf.value),sizeof(float),1,filesegy);
        }
        else if( segyHead->format == 8 )
        {
            char temp = 0;
            fread((void*)(&temp),sizeof(char),1,filesegy);
            sf.value = (float)temp;
        }
        surface.push_back(sf);
    }
    qDebug()<<"Read surface have done!"<<surface.size();
}
