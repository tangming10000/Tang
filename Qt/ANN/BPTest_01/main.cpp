#include <iostream>
#include "anntrain.h"
#include <QFile>
#include <QTextStream>
using namespace std;

double functions(double* x, int N)
{
    double sum = 0.0;
    for( int i = 0; i < N; i++ )
        sum += x[i];
    //return 0.6*( 1.0 - sum + 1.9*sum*sum)*exp( -sum*sum / 2.0);
    //return result;
    return (cos(sum)+1.0)/2.5;
    //return sum;
}

int main()
{
    int NN_Sample = 40;
    int input = 1;
    int hide = 20;
    int output = 1;
    //double u0 = 0.8;
    srand((unsigned)time(0));
    const char* filename = "hello.txt";

    double **x = new double*[NN_Sample];
    double **d = new double*[NN_Sample];
    for( int i = 0; i < NN_Sample; i++)
    {
        x[i] = new double[input];
        d[i] = new double[output];
    }

    ANNTrain *BP_Train = new ANNTrain;

    //BP_Train->OutPut_Weigh();
    BP_Train->set_MinError(1.0e-6);
    BP_Train->set_MaxLoopNum(400000);
    //BP_Train->set_NNLearn(u0); //设置学习率
    //BP_Train->set_Move( 0.2);   //动量因子

    for( int i = 0; i < NN_Sample; i++ )
    {
        //cout << i <<"  ";
        for( int j = 0; j < input; j++)
        {
            //x[i][j] = (double)rand() / RAND_MAX;
            x[i][j] = double(i ) * PI * 2.5  / NN_Sample ;
            //cout << x[i][j]<<"  ";
        }
        //d[i][0] = sin(x[i][0]);
        for( int k = 0; k < output; k++ )
        {
            d[i][k] = functions(x[i],input);
        }
        //cout << d[i][0]<< endl;
    }
    cout <<"Start...."<<endl;

//    if(!BP_Train->Loadfile_Weigh(filename))
//        cout <<"Load weight file errror!!"<<endl;


    BP_Train->InitNet(input,hide,output);
    if( BP_Train->BP_Train01(x,d,NN_Sample))
        cout <<"The BP Network have train success!"<<endl;
    else
        cout <<"The BP Network have train failed!"<<endl;

        //BP_Train->set_NNLearn(u0 / ( 1.0 + Num / 200 ));
       // BP_Train->set_NNLearn(u0 );
        //cout << Num<< "\t" <<BP_Train->Error<<"\t"<<BP_Train->NN_Learn_hide<<endl;

    double xx[1],o[1];
    QFile file("outdata.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream str(&file);
    for( double i =0.01; i <= 2.0; i += 0.01)
    {
        xx[0] = i*PI;
//        xx[0] =  (double)rand()/RAND_MAX;
//        xx[1] =  (double)rand()/RAND_MAX;
        o[0] = 0.0;
        BP_Train->BP_Count(xx,o);
        str <<"\t"<<functions(xx,input)<<"\t" <<o[0]<<"\r\n";
    }
    file.close();


    BP_Train->OutPutfile_Weigh(filename);

    for( int i = 0; i < NN_Sample; i++ )
    {
        delete [] x[i];
        delete [] d[i];
    }
    delete [] x;
    delete [] d;
    return 0;
}
