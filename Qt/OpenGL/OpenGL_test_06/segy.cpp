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
static unsigned char ASCIItoEBCDIC(const unsigned char c)
{
   return a2e[c];
}
static unsigned char EBCDICtoASCII(const unsigned char c)
{
   return e2a[c];
}


//! Byte swap unsigned short
static unsigned short swap_uint16( unsigned short val )
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap short
static short swap_int16( short val )
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
static unsigned int swap_uint32(unsigned int val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}

//! Byte swap int
static int swap_int32( int val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | ((val >> 16) & 0xFFFF);
}
static int64_t swap_int64( int64_t  val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
}

//static uint64_t swap_uint64( uint64_t  val )
//{
//    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
//    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
//    return (val << 32) | (val >> 32);
//}

static void ibm2float(float *indata,int N)
{
    register int fconv,fmant,i,t;
    for (i = 0;i < N; ++i) {
        fconv = *((int*)(&(indata[i])));
        /* if little endian, i.e. endian=0 do this */
        fconv = (fconv << 24) | ((fconv >> 24) & 0xff)|\
                ((fconv & 0xff00) << 8) | ((fconv & 0xff0000) >> 8);

        if (fconv)
        {
            fmant = 0x00ffffff & fconv;
            if (fmant == 0)
                printf("mantissa is zero data may not be in IBM FLOAT Format !");
            t = (int) ((0x7f000000 & fconv) >> 22) - 130;
            while (!(fmant & 0x00800000))
            {
                --t;
                fmant <<= 1;
            }
            if (t > 254)
                fconv = (0x80000000 & fconv)|0x7f7fffff;
            else if(t <= 0)
                fconv = 0;
            else
                fconv = (0x80000000&fconv)|(t<<23)|(0x007fffff&fmant);
        }
        indata[i] = *((float*)(&fconv));
    }
    return;
}

static void float2ibm(float *indata,int N)
{
    register int fconv, fmant, i, t;

    for (i=0;i<N;++i)
    {
        fconv = *((int*)(&indata[i]));
        if (fconv)
        {
            fmant = (0x007fffff & fconv) | 0x00800000;
            t = (int) ((0x7f800000 & fconv) >> 23) - 126;
            while (t & 0x3)
            {
                ++t;
                fmant >>= 1;
            }
            fconv = (0x80000000 & fconv) | (((t>>2) + 64) << 24) | fmant;
        }

        fconv = (fconv<<24) | ((fconv>>24)&0xff) |\
                ((fconv&0xff00)<<8) | ((fconv&0xff0000)>>8);
        indata[i] = *((float*)(&fconv));
    }
    return;
}

static float ibm2float(const float data)
{
    register int fconv, fmant, t;
    fconv = *((int*)(&data));
    /* if little endian, i.e. endian=0 do this */
    fconv = (fconv << 24) | ((fconv >> 24) & 0xff)|\
            ((fconv & 0xff00) << 8) | ((fconv & 0xff0000) >> 8);

    if (fconv)
    {
        fmant = 0x00ffffff & fconv;
        if (fmant == 0)
            printf("mantissa is zero data may not be in IBM FLOAT Format !");
        t = (int) ((0x7f000000 & fconv) >> 22) - 130;
        while (!(fmant & 0x00800000))
        {
            --t;
            fmant <<= 1;
        }
        if (t > 254)
            fconv = (0x80000000 & fconv)|0x7f7fffff;
        else if(t <= 0)
            fconv = 0;
        else
            fconv = (0x80000000&fconv)|(t<<23)|(0x007fffff&fmant);
    }
    return *((float*)(&fconv));
}

static int float2ibm(const float data)
{
    register int fconv, fmant, t;
    fconv = *((int*)(&data));
    if (fconv)
    {
        fmant = (0x007fffff & fconv) | 0x00800000;
        t = (int) ((0x7f800000 & fconv) >> 23) - 126;
        while (t & 0x3)
        {
            ++t;
            fmant >>= 1;
        }
        fconv = (0x80000000 & fconv) | (((t>>2) + 64) << 24) | fmant;
    }
    fconv = (fconv<<24) | ((fconv>>24)&0xff) |\
        ((fconv&0xff00)<<8) | ((fconv&0xff0000)>>8);
    return fconv;
}

static void swap_REEL(REEL *Reel)
{
    Reel->jobid = swap_int32(Reel->jobid);
    Reel->lino  = swap_int32(Reel->lino);
    Reel->reno  = swap_int32(Reel->reno);

    Reel->ntrpr = swap_int16(Reel->ntrpr);
    Reel->nart  = swap_int16(Reel->nart );

    Reel->hdt   = swap_uint16(Reel->hdt );
    Reel->dto   = swap_uint16(Reel->dto);
    Reel->hns   = swap_uint16(Reel->hns);
    Reel->nso   = swap_uint16(Reel->nso);

    Reel->format= swap_int16(Reel->format);
    Reel->fold  = swap_int16(Reel->fold);
    Reel->tsort = swap_int16(Reel->tsort);
    Reel->vscode= swap_int16(Reel->vscode);
    Reel->hsfs  = swap_int16(Reel->hsfs);
    Reel->hsfe  = swap_int16(Reel->hsfe);
    Reel->hslen = swap_int16(Reel->hslen);
    Reel->hstyp = swap_int16(Reel->hstyp);
    Reel->schn  = swap_int16(Reel->schn);
    Reel->hstae = swap_int16(Reel->hstae);
    Reel->htatyp= swap_int16(Reel->htatyp);
    Reel->hcorr = swap_int16(Reel->hcorr);
    Reel->bgrcv = swap_int16(Reel->bgrcv);
    Reel->rcvm  = swap_int16(Reel->rcvm);
    Reel->mfeet = swap_int16(Reel->mfeet);
    Reel->polyt = swap_int16(Reel->polyt);
    Reel->vpol  = swap_int16(Reel->vpol);
    int i = 0;
    for( i = 0; i < 170; i++ )
    {
        Reel->hunass[i] = swap_int16(Reel->hunass[i]);
    }
}

static void swap_TRACE(TRACE *Trace)
{
    Trace->tracl    = swap_int32(Trace->tracl);
    Trace->tracr    = swap_int32(Trace->tracr);
    Trace->fldr     = swap_int32(Trace->fldr);
    Trace->tracf    = swap_int32(Trace->tracf);
    Trace->ep       = swap_int32(Trace->ep);
    Trace->cdp      = swap_int32(Trace->cdp);
    Trace->cdpt     = swap_int32(Trace->cdpt);

    Trace->trid     = swap_int16(Trace->trid);
    Trace->nvs      = swap_int16(Trace->nvs);
    Trace->nhs      = swap_int16(Trace->nhs);
    Trace->duse     = swap_int16(Trace->duse);

    Trace->offset   = swap_int32(Trace->offset);
    Trace->gelev    = swap_int32(Trace->gelev);
    Trace->selev    = swap_int32(Trace->selev);
    Trace->sdepth   = swap_int32(Trace->sdepth);
    Trace->gdel     = swap_int32(Trace->gdel);
    Trace->sdel     = swap_int32(Trace->sdel);
    Trace->swdep    = swap_int32(Trace->swdep);
    Trace->gwdep    = swap_int32(Trace->swdep);

    Trace->scalel   = swap_int16(Trace->scalel);
    Trace->scalco   = swap_int16(Trace->scalco);

    Trace->sx       = swap_int32(Trace->sx);
    Trace->sy       = swap_int32(Trace->sy);
    Trace->gx       = swap_int32(Trace->gx);
    Trace->gy       = swap_int32(Trace->gy);

    Trace->counit   = swap_int16(Trace->counit);
    Trace->wevel    = swap_int16(Trace->wevel);
    Trace->swevel   = swap_int16(Trace->swevel);
    Trace->sut      = swap_int16(Trace->sut);
    Trace->gut      = swap_int16(Trace->gut);
    Trace->sstat    = swap_int16(Trace->sstat);
    Trace->gstat    = swap_int16(Trace->gstat);
    Trace->tstat    = swap_int16(Trace->tstat);
    Trace->laga     = swap_int16(Trace->laga);
    Trace->lagb     = swap_int16(Trace->lagb);
    Trace->delrt    = swap_int16(Trace->delrt);
    Trace->muts     = swap_int16(Trace->muts);
    Trace->mute     = swap_int16(Trace->mute);
    Trace->ns       = swap_int16(Trace->ns);
    Trace->dt       = swap_int16(Trace->dt);
    Trace->gain     = swap_int16(Trace->gain);
    Trace->igc      = swap_int16(Trace->igc);
    Trace->igi      = swap_int16(Trace->igi);
    Trace->corr     = swap_int16(Trace->corr);
    Trace->sfs      = swap_int16(Trace->sfs);
    Trace->sfe      = swap_int16(Trace->sfe);
    Trace->slen     = swap_int16(Trace->slen);
    Trace->styp     = swap_int16(Trace->styp);
    Trace->stas     = swap_int16(Trace->stas);
    Trace->stae     = swap_int16(Trace->stae);
    Trace->tatyp    = swap_int16(Trace->tatyp);
    Trace->afilf    = swap_int16(Trace->afilf);
    Trace->afils    = swap_int16(Trace->afils);
    Trace->nofilf   = swap_int16(Trace->nofilf);
    Trace->nofils   = swap_int16(Trace->nofils);
    Trace->lcf      = swap_int16(Trace->lcf);
    Trace->hcf      = swap_int16(Trace->hcf);
    Trace->lcs      = swap_int16(Trace->lcs);
    Trace->hcs      = swap_int16(Trace->hcs);
    Trace->year     = swap_int16(Trace->year);
    Trace->day      = swap_int16(Trace->day);
    Trace->minute   = swap_int16(Trace->minute);
    Trace->sec      = swap_int16(Trace->sec);
    Trace->timbas   = swap_int16(Trace->timbas);
    Trace->trwf     = swap_int16(Trace->trwf);
    Trace->grnors   = swap_int16(Trace->grnors);
    Trace->grnofr   = swap_int16(Trace->grnofr);
    Trace->grnlof   = swap_int16(Trace->grnlof);
    Trace->gaps     = swap_int16(Trace->gaps);

    //CWP SU Headers
    Trace->d1       = ibm2float(Trace->d1);
    Trace->f1       = ibm2float(Trace->f1);
    Trace->d2       = ibm2float(Trace->d2);
    Trace->f2       = ibm2float(Trace->f2);
    Trace->ungpow   = ibm2float(Trace->ungpow);
    Trace->unscale  = ibm2float(Trace->unscale);
    Trace->ntr      = ibm2float(Trace->ntr);
    Trace->mark     = ibm2float(Trace->mark);
    Trace->shortpad = ibm2float(Trace->shortpad);

    int i = 0;
    for( i = 0; i < 14; i++ )
        Trace->unass[i] = swap_int16(Trace->unass[i]);
}

void ReadREEL(FILE *file, REEL *Reel)
{
    fseek(file,3200,SEEK_SET);
    fread((void*)Reel,sizeof(REEL),1,file);
    int i = 0;


    if ( Reel->format == 0 )
        return ;
    else
        swap_REEL(Reel);
    return ;
}

void WriteREEL(FILE *file, REEL *Reel)
{
    //继续做高低位转换
    if( Reel->format != 0)
        swap_REEL(Reel);
    fwrite((void*)Reel,sizeof(REEL),1,file);
    swap_REEL(Reel);
}

void ReadSEGYHead(FILE *file, REEL *Reel, SEGYHEAD *sh)
{
    fseek(file,0L,SEEK_END);
    long pos = ftell(file);
    int format = Reel->format;
    if( (format == 1) || ( format == 2) || (format == 5))
    {
        sh->DataType = 4;
    }
    else if ( format == 3)
        sh->DataType = 2;
    else if ( format == 8 )
        sh->DataType = 1;
    else
    {
        printf("The data format is not recongnized!\n");
        exit(-1);
    }

    sh->TotalTraces = ( pos - 3600L) / ( 240L + long(Reel->hns * sh->DataType));
}

void ReadTRACE(FILE *file, TRACE *Trace)
{
    fread((void*)Trace,sizeof(TRACE),1,file);
    swap_TRACE(Trace);
}

void WriteTRACE(FILE *file, TRACE *Trace)
{
    swap_TRACE(Trace);
    fread((void*)Trace,sizeof(TRACE),1,file);
    swap_TRACE(Trace);
}

void ReadEBDIC(FILE *fp, unsigned char *head)
{
    int i = 0;
    fseek(fp,0,SEEK_SET);
    unsigned char c = ' ';
    for( i = 0; i < 3200; i++ )
    {
        fread(&c,sizeof(c),1,fp);
        head[i] = EBCDICtoASCII(c);
    }
}

void WriteEBDIC(FILE *fp,unsigned  char *head)
{
    int i  = 0;
    unsigned char c = ' ';
    for( i = 0; i < 3200; i++ )
    {
        c = (unsigned char)head[i];
        c = ( unsigned char )ASCIItoEBCDIC(c);
        fwrite(&c,sizeof(char),1,fp);
    }
}

void ReadData(FILE *file, float *data, REEL *Reel)
{
    int N = Reel->hns;
    switch( Reel->format )
    {
    case 0:
        fread((void*)data,sizeof(float),N,file);
        break;
    case 1:
        fread((void*)data,sizeof(float),N,file);
        ibm2float(data,N);
        break;
    case 2:
    {
        int i = 0;
        int *temp = NULL;

        if( ( temp = (int*)malloc(sizeof(int)*N)) == NULL )
        {
            printf("set memory for read int type segy data error!");
            exit(-1);
        }
        fread((void*)temp,sizeof(int),N,file);
        for( i = 0; i < N; i++ )
        {
            data[i] = float(swap_int32(temp[i]));
        }
        free(temp);
    }
    case 3:
    {
        int i  = 0;
        short *temp = NULL;

        if( ( temp = (short*)malloc(sizeof(short)*N)) == NULL )
        {
            printf("set memory for read int type segy data error!");
            exit(-1);
        }
        fread((void*)temp,sizeof(short),N,file);
        for( i = 0; i < N; i++ )
        {
            data[i] = float(swap_int16(temp[i]));
        }
        free(temp);
    }
    case 5:
        fread((void*)data,sizeof(float),N,file);
        break;
    case 8:
    {
        int i = 0;
        char *temp = NULL;

        if( ( temp = (char*)malloc(sizeof(char)*N)) == NULL )
        {
            printf("set memory for read int type segy data error!");
            exit(-1);
        }
        fread((void*)temp,sizeof(char),N,file);
        for( i = 0; i < N; i++ )
        {
            data[i] = float(swap_int16(temp[i]));
        }
        free(temp);
    }
    default:
        break;
    }
}
void WriteData(FILE *file, float *data, REEL *Reel)
{
    int N = Reel->hns;
    switch(Reel->format)
    {
    case 0:
        fwrite((void*)data,sizeof(float),N,file);
        break;
    case 1:
        float2ibm(data,N);
        fwrite((void*)data,sizeof(float),N,file);
        break;
    case 2:
    {
        int i = 0;
        int *temp = NULL;

        if( ( temp = (int*)malloc(sizeof(int)*N)) == NULL )
        {
            printf("set memory for read int type segy data error!");
            exit(-1);
        }

        for( i = 0; i < N; i++ )
        {
            temp[i] = int(data[i]);
            temp[i] = swap_int32(temp[i]);
        }
        fwrite((void*)temp,sizeof(int),N,file);
        free(temp);
        break;
    }
    case 3:
    {
        int i = 0;
        short *temp = NULL;

        if( ( temp = (short*)malloc(sizeof(short)*N)) == NULL )
        {
            printf("set memory for read int type segy data error!");
            exit(-1);
        }

        for( i = 0; i < N; i++ )
        {
            temp[i] = short(data[i]);
            temp[i] = swap_int16(temp[i]);
        }
        fwrite((void*)temp,sizeof(short),N,file);
        free(temp);
        break;
    }
    case 5:
    {
        fwrite((void*)data,sizeof(float),N,file);
        break;
    }
    case 8:
    {
        int i = 0;
        char *temp = NULL;

        if( ( temp = (char*)malloc(sizeof(char)*N)) == NULL )
        {
            printf("set memory for read int type segy data error!");
            exit(-1);
        }

        for( i = 0; i < N; i++ )
        {
            temp[i] = char(data[i]);
        }
        fwrite((void*)temp,sizeof(char),N,file);
        free(temp);
        break;
    }
    default:
        break;
    }
}
