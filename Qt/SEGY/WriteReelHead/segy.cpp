#include "segy.h"

static unsigned char a2e[256] = {
   0, 1, 2, 3, 55, 45, 46, 47, 22, 5, 37, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 60, 61, 50, 38, 24, 25, 63, 39, 28, 29, 30, 31,
   64, 79,127,123, 91,108, 80,125, 77, 93, 92, 78,107, 96, 75, 97,
   240,241,242,243,244,245,246,247,248,249,122, 94, 76,126,110,111,
   124,193,194,195,196,197,198,199,200,201,209,210,211,212,213,214,
   215,216,217,226,227,228,229,230,231,232,233, 74,224, 90, 95,109,
   121,129,130,131,132,133,134,135,136,137,145,146,147,148,149,150,
   151,152,153,162,163,164,165,166,167,168,169,192,106,208,161, 7,
   32, 33, 34, 35, 36, 21, 6, 23, 40, 41, 42, 43, 44, 9, 10, 27,
   48, 49, 26, 51, 52, 53, 54, 8, 56, 57, 58, 59, 4, 20, 62,225,
   65, 66, 67, 68, 69, 70, 71, 72, 73, 81, 82, 83, 84, 85, 86, 87,
   88, 89, 98, 99,100,101,102,103,104,105,112,113,114,115,116,117,
   118,119,120,128,138,139,140,141,142,143,144,154,155,156,157,158,
   159,160,170,171,172,173,174,175,176,177,178,179,180,181,182,183,
   184,185,186,187,188,189,190,191,202,203,204,205,206,207,218,219,
   220,221,222,223,234,235,236,237,238,239,250,251,252,253,254,255
};
static unsigned char e2a[256] = {
   0, 1, 2, 3,156, 9,134,127,151,141,142, 11, 12, 13, 14, 15,
   16, 17, 18, 19,157,133, 8,135, 24, 25,146,143, 28, 29, 30, 31,
   128,129,130,131,132, 10, 23, 27,136,137,138,139,140, 5, 6, 7,
   144,145, 22,147,148,149,150, 4,152,153,154,155, 20, 21,158, 26,
   32,160,161,162,163,164,165,166,167,168, 91, 46, 60, 40, 43, 33,
   38,169,170,171,172,173,174,175,176,177, 93, 36, 42, 41, 59, 94,
   45, 47,178,179,180,181,182,183,184,185,124, 44, 37, 95, 62, 63,
   186,187,188,189,190,191,192,193,194, 96, 58, 35, 64, 39, 61, 34,
   195, 97, 98, 99,100,101,102,103,104,105,196,197,198,199,200,201,
   202,106,107,108,109,110,111,112,113,114,203,204,205,206,207,208,
   209,126,115,116,117,118,119,120,121,122,210,211,212,213,214,215,
   216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,
   123, 65, 66, 67, 68, 69, 70, 71, 72, 73,232,233,234,235,236,237,
   125, 74, 75, 76, 77, 78, 79, 80, 81, 82,238,239,240,241,242,243,
   92,159, 83, 84, 85, 86, 87, 88, 89, 90,244,245,246,247,248,249,
   48, 49, 50, 51, 52, 53, 54, 55, 56, 57,250,251,252,253,254,255
};
unsigned char ASCIItoEBCDIC(const unsigned char c)
{
   return a2e[c];
}
unsigned char EBCDICtoASCII(const unsigned char c)
{
   return e2a[c];
}
unsigned int num2ibm(float input)/*将数据从float转为IBM的保存格式*/
{
    long sign;//符号
    sign = ( input<0?1:0 );
    long exp;//指数
    float input1 ; // attention : cannot use   long input1;
    input   = input * pow(-1, sign);// abs(input)
    exp=0;
    input1 = input;
    unsigned int result ;
    if (input >0 )    // 非0值才计算
    {
        if( (int)input>0)
        {
            exp++;
            while   ((int) input1/16 > 0)
            {
                exp++;
                input1= input1/16;
            }
        }
        else
        {
            while ( (int)input1*16 ==0)
            {
                exp--;
                input1=input1*16;
            }
            exp++;// attention :    ibm fmant   :     0.mant   not 1.mant !
        }    
        long e;
        e = (   exp + 64 ) ;
        double     fm = input * pow(16,-exp);////////////////
        long fmant=(long) (   fm * pow(2,24) ) ;//尾数
        result = ( sign<<31) | (   e <<24   )    |   fmant ;
    }
    else if( input == 0)
        result = 0;

    return result;
}
float ibm2num(unsigned int DataUint32)
{
    float result;
    float sign =(float)( DataUint32>>31 );
    float exp =(float)( (DataUint32&0x7f000000)>>24 );
    exp = exp - 64.0 ;
    float frac = (float)( DataUint32&0x00ffffff );
    frac = frac/(pow(2,24));
    result = (1.0-2.0*sign)*(pow(16.0, exp))*frac;
    return result;
}
float ieee2num(unsigned int ieee)
{
    int sign; //符号
    sign =( ieee& 0x80000000 ) *pow(2,-31);
    if (sign ==1)// for value < 0 :
        ieee =~( ieee&0x7fffffff );//负数则为补码

    int e;//指数
    e=(ieee&0x7f800000)*pow(2,-23)-127-sign;

    unsigned int x; //尾数
    x = ieee&0x007fffff-sign;// - sign : for value < 0
    float x0;
    x0 = x* pow(2,-23);

    float result;
    if ( x0 ==0 && e + 127 ==0 ) //0值特殊对待
        result = 0;
    else
        result = pow(-1,sign)*(1+x0)*pow(2,e);
    return result;
}
unsigned int num2ieee(float dec)
{
    int sign,e;
    uint x;

    sign = dec<0?1:0;   //符号位:负数取1,其他取0.:
    dec= dec* pow(-1, sign);   //abs(dec):
    float d1;           // integer part of float :
    d1=(float)(int )dec;
    double d2,d2d;           // other part of float :
    d2=(double )(dec - d1);
    int   e0=0;          // gain e:指数
    int d1d = d1;
    if ( dec >0 )        //非0值才有必要计算
    {
        if (d1d >= 1)// d1 will shift right :
        {
            while ( d1d>1)
            {
                d1d=(int)d1d/2;
                e0++;
            }
        }
        else //   d2 will shift left :
        {
            d2d=d2;
            while ((int)d2d!=1)
            {
                d2d*=2;
                e0--;
            }
        }
    }
    e= e0 + 127;
    // gain x :
    float x0;
    x0 = dec * pow(2,-e0) - 1 ;
    x = x0 * pow(2, 23 ) ;

    if (dec==0)
    {
        x = 0;
        e = 0;
    }     //0值特殊对待

    // merge sign,e,x:
    ulong result ;
    if ( sign==0)
        result = sign*pow(2,31) + e*pow(2,23) + x ;//正值以原码形式存放
    else
        result =   (~ e) *pow(2,23) + (~x )    +1 ;// 负值以补码形式存放
    return result;
}

void REEL::ReadREEL(QDataStream &rs)
{
    rs>>jobid;
    rs>>lino;
    rs>>reno;
    rs>>ntrpr;
    rs>>nart;
    rs>>hdt;
    rs>>dto;
    rs>>hns;
    rs>>nso;
    rs>>format;
    rs>>fold;
    rs>>tsort;
    rs>>vscode;
    rs>>hsfs;
    rs>>hsfe;
    rs>>hslen;
    rs>>hstyp;
    rs>>schn;
    rs>>hstas;
    rs>>hstae;
    rs>>htatyp;
    rs>>hcorr;
    rs>>bgrcv;
    rs>>rcvm;
    rs>>mfeet;
    rs>>polyt;
    rs>>vpol;
    for( int i = 0; i < 170; i++ )
        rs>>hunass[i];

    int num;
    if ( format == 1 || format == 2 || format == 4 || format == 5)
        num = 4.0;
    else if( format == 3 )
        num = 2.0;
    else
        num =1.0;
    DataType = num;
    TotalTraces = ( rs.device()->size() - 3600 )/( 240 + num * hns );
}
void REEL::WriteREEL(QDataStream &out)
{
    out<<jobid;
    out<<lino;
    out<<reno;
    out<<ntrpr;
    out<<nart;
    out<<hdt;
    out<<dto;
    out<<hns;
    out<<nso;
    out<<format;
    out<<fold;
    out<<tsort;
    out<<vscode;
    out<<hsfs;
    out<<hsfe;
    out<<hslen;
    out<<hstyp;
    out<<schn;
    out<<hstas;
    out<<hstae;
    out<<htatyp;
    out<<hcorr;
    out<<bgrcv;
    out<<rcvm;
    out<<mfeet;
    out<<polyt;
    out<<vpol;
    for( int i = 0; i < 170; i++ )
        out<<hunass[i];
}
void TRACE::ReadTRACE(QDataStream &ts)
{
    ts>>tracl;
    ts>>tracr;
    ts>>fldr;
    ts>>tracf;
    ts>>ep;
    ts>>cdp;
    ts>>cdpt;
    ts>>trid;
    ts>>nvs;
    ts>>nhs;
    ts>>duse;
    ts>>offset;
    ts>>gelev;
    ts>>selev;
    ts>>sdepth;
    ts>>gdel;
    ts>>sdel;
    ts>>swdep;
    ts>>gwdep;
    ts>>scalel;
    ts>>scalco;
    ts>>sx;
    ts>>sy;
    ts>>gx;
    ts>>gy;
    ts>>counit;
    ts>>wevel;
    ts>>swevel;
    ts>>sut;
    ts>>gut;
    ts>>sstat;
    ts>>gstat;
    ts>>tstat;
    ts>>laga;
    ts>>lagb;
    ts>>delrt;
    ts>>muts;
    ts>>mute;
    ts>>ns;
    ts>>dt;
    ts>>gain;
    ts>>igc;
    ts>>igi;
    ts>>corr;
    ts>>sfs;
    ts>>sfe;
    ts>>slen;
    ts>>styp;
    ts>>stas;
    ts>>stae;
    ts>>tatyp;
    ts>>afilf;
    ts>>afils;
    ts>>nofilf;
    ts>>nofils;
    ts>>lcf;
    ts>>hcf;
    ts>>lcs;
    ts>>hcs;
    ts>>year;
    ts>>day;
    ts>>hour;
    ts>>minute;
    ts>>sec;
    ts>>timbas;
    ts>>trwf;
    ts>>grnors;
    ts>>grnofr;
    ts>>grnlof;
    ts>>gaps;
    ts>>otrav;
    ts>>d1;
    ts>>f1;
    ts>>d2;
    ts>>f2;
    ts>>ungpow;
    ts>>unscale;
    ts>>ntr;
    ts>>mark;
    ts>>shortpad;
    for( int i = 0; i < 14; i++)
        ts>>unass[i];
}
void TRACE::WriteTRACE(QDataStream &out)
{
    out<<tracl;
    out<<tracr;
    out<<fldr;
    out<<tracf;
    out<<ep;
    out<<cdp;
    out<<cdpt;
    out<<trid;
    out<<nvs;
    out<<nhs;
    out<<duse;
    out<<offset;
    out<<gelev;
    out<<selev;
    out<<sdepth;
    out<<gdel;
    out<<sdel;
    out<<swdep;
    out<<gwdep;
    out<<scalel;
    out<<scalco;
    out<<sx;
    out<<sy;
    out<<gx;
    out<<gy;
    out<<counit;
    out<<wevel;
    out<<swevel;
    out<<sut;
    out<<gut;
    out<<sstat;
    out<<gstat;
    out<<tstat;
    out<<laga;
    out<<lagb;
    out<<delrt;
    out<<muts;
    out<<mute;
    out<<ns;
    out<<dt;
    out<<gain;
    out<<igc;
    out<<igi;
    out<<corr;
    out<<sfs;
    out<<sfe;
    out<<slen;
    out<<styp;
    out<<stas;
    out<<stae;
    out<<tatyp;
    out<<afilf;
    out<<afils;
    out<<nofilf;
    out<<nofils;
    out<<lcf;
    out<<hcf;
    out<<lcs;
    out<<hcs;
    out<<year;
    out<<day;
    out<<hour;
    out<<minute;
    out<<sec;
    out<<timbas;
    out<<trwf;
    out<<grnors;
    out<<grnofr;
    out<<grnlof;
    out<<gaps;
    out<<otrav;
    out<<d1;
    out<<f1;
    out<<d2;
    out<<f2;
    out<<ungpow;
    out<<unscale;
    out<<ntr;
    out<<mark;
    out<<shortpad;
    for( int i = 0; i < 14; i++)
        out<<unass[i];
}

void ReadData(QDataStream &in,float x[], int N)
{
    for( int i = 0; i < N; i++)
    {
        unsigned int temp = 0;
        in>>temp;
        x[i]=ibm2num(temp);
    }
}
void ReadData(QDataStream &in, float x[], int N, bool fp)
{
    if(fp == true)
    {
        for(int i = 0; i < N; i++)
        {
            unsigned int temp = 0;
            in>>temp;
            x[i]=ieee2num(temp);
        }
    }
}
void ReadData(QDataStream &in, int x[], int N)
{
    for( int i = 0; i < N; i++)
    {
        int temp;
        in>>temp;
        x[i]= temp;
    }
}
void ReadData(QDataStream &in, short x[], int N)
{
    for( int i = 0; i < N; i++)
    {
        short temp;
        in>>temp;
        x[i]= temp;
    }
}

void WriteData(QDataStream &out, float x[], int N)
{
    for(int i = 0; i < N; i++)
    {
        unsigned int temp = 0;
        temp = num2ibm(x[i]);
        out<<temp;
    }
}
void WriteData(QDataStream &out, float x[], int N, bool fp)
{
    if(fp == true)
    {
        for(int i= 0; i < N; i++)
        {
            unsigned int temp = 0;
            temp = num2ieee(x[i]);
            out<<temp;
        }
    }
}
void WriteData(QDataStream &out, int x[], int N)
{
    for( int i = 0; i < N; i++)
    {
        out<<x[i];
    }
}
void WriteData(QDataStream &out, short x[], int N)
{
    for( int i = 0; i < N; i++)
    {
        out<<x[i];
    }
}

void ReadDataATP(QDataStream &in, float x[], int Length, int flags)
{
    int N = Length;
    switch(flags)
    {
    case 1:
        ReadData(in,x,N);
        break;
    case 2:
        int tempI[N];
        ReadData(in,tempI,N);
        for(int i = 0; i<N;i++)
            x[i]=tempI[i];
        break;
    case 3:
        short tempS[N];
        ReadData(in,tempS,N);
        for(int i = 0; i<N;i++)
            x[i]=tempS[i];
        break;
    case 5:
        ReadData(in,x,N,true);
        break;
    }
}
void WriteDataATP(QDataStream &out, float x[], int Length, int flags)
{
    int N = Length;
    switch(flags)
    {
    case 1:
        WriteData(out,x,N);
        break;
    case 2:
        int tempI[N];
        for(int i = 0; i<N;i++)
            tempI[i]=int(x[i]);
        WriteData(out,tempI,N);
        break;
    case 3:
        short tempS[N];
        for(int i = 0; i<N;i++)
            tempS[i]=(short)x[i];
        WriteData(out,tempS,N);
        break;
    case 5:
        WriteData(out,x,N,true);
        break;
    }

}
