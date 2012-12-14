#include "anntrain.h"

ANNTrain::ANNTrain()
{
    is_Data = false;
    input_Num = 0;
    output_Num = 0;
    hide_Num = 0;

    //给定相关参数一定初值
    Error_Min = 1.0e-6;
    NN_Learn_hide = NN_Learn_output = 0.9999;
    NN_Learn_Bhide = NN_Learn_Boutput = 0.9999;
    Move_hide = Move_output = 0.2;
    Max_LoopNum = 20000;

    Error = 0;
}

ANNTrain::~ANNTrain()//
{
    if(is_Data)
    {        
        Clear_Weigh();//清除权值，阀值
    }
}

void ANNTrain::InitNet(int input, int hide, int output)
{
    //清空数组指针tang
    if(is_Data)
    {
        //清除V_input
        Clear_Weigh();
    }
    //输入三个参数
    input_Num = input;
    hide_Num = hide;
    output_Num = output;
    //初始化权值
    Init_Weigh();
}

void ANNTrain::set_MinError(double error)
{
    Error_Min = error;
}
void ANNTrain::set_MaxLoopNum(int Num)
{
    Max_LoopNum = Num;
}
void ANNTrain::set_hideLearnRatio(double learn)
{
    NN_Learn_hide = learn;
}
void ANNTrain::set_outputLearnRatio(double learn)
{
    NN_Learn_output = learn;
}
void ANNTrain::set_hideThresholdRation(double learn)
{
    NN_Learn_Bhide = learn;
}
void ANNTrain::set_outputThresholdRation(double learn)
{
    NN_Learn_Boutput = learn;
}
void ANNTrain::set_hideMove(double move)
{
    Move_hide = move;
}
void ANNTrain::set_outputMove(double move)
{
    Move_output = move;
}

void ANNTrain::Clear_Weigh()
{
    //清除V_input
    for( int j = 0; j < hide_Num; j++)
    {
        delete [] V_input[j];
    }

    delete [] V_input;
    //清除W_input
    for( int k = 0; k < output_Num; k++)
    {
        delete [] W_input[k];
    }
    delete [] W_input;
    delete [] b1;
    delete [] b2;
    b1 = NULL;
    b2 = NULL;
    V_input = NULL;
    W_input = NULL;
}

void ANNTrain::Init_Weigh()
{
    is_Data = true;
    //创建V_input
    V_input = new double*[hide_Num];

    for( int j = 0; j < hide_Num; j++)
    {
        V_input[j] = new double[input_Num];
    }
    //创建W_input
    W_input = new double*[output_Num];

    for( int k = 0; k < output_Num; k++)
    {
        W_input[k] = new double[hide_Num];
    }

    b1 = new double[hide_Num];
    b2 = new double[output_Num];

    //初始化权值,赋予0~1的随机数
    //srand((unsigned)time_t(NULL));
    srand((unsigned)time(0));
    for( int i = 0; i < input_Num; i++ )
    {
        for( int j = 0; j < hide_Num; j++ )
        {
            V_input[j][i] = (2.0*(double)rand()/RAND_MAX)-1.0;
        }
    }
    for( int j = 0; j < hide_Num; j++ )
    {
        for( int k = 0; k < output_Num; k++ )
        {
            W_input[k][j] = (2.0*(double)rand()/RAND_MAX)-1.0;
        }
    }
    //初始化阀值
    for( int j = 0; j < hide_Num; j++ )
        b1[j] = (2.0*(double)rand()/RAND_MAX)-1.0;
    for( int k = 0; k < output_Num; k++)
        b2[k] = (2.0*(double)rand()/RAND_MAX)-1.0;

}

bool ANNTrain::BP_Train01(double **Xinput, double **Dinput, int TrainSample)
{
    double *y = new double[hide_Num];
    double *o = new double[output_Num];
    double *x = new double[input_Num];
    double *d = new double[output_Num];
    //误差
    double *Error_hide = new double[hide_Num];
    double *Error_output = new double[output_Num];

    //权值参数矩阵备份
    double **V_back,**W_back;
    V_back = new double*[hide_Num];
    W_back = new double*[output_Num];
    for( int j = 0; j < hide_Num; j++)
        V_back[j] = new double[input_Num];

    for( int k = 0; k < output_Num; k++)
        W_back[k] = new double[hide_Num];
    for( int j = 0; j < hide_Num; j++ )
        for( int i = 0; i < input_Num; i++)
            V_back[j][i] = V_input[j][i];
    for( int k = 0; k < output_Num; k++ )
        for( int j = 0; j < hide_Num; j++ )
            W_back[k][j] = W_input[k][j];


    double sum = 0.0;
    int Num = 0;
    Error = 1.0;
    while( (Error > Error_Min) && ( Num < Max_LoopNum) )
    {
        for( int isamp = 0; isamp < TrainSample; isamp++ )
        {
            for( int i = 0; i < input_Num; i++)
                x[i] = Xinput[isamp][i];
            for( int k= 0; k < output_Num; k++ )
                d[k] = Dinput[isamp][k];

            for( int j = 0; j < hide_Num; j++ )
            {
                sum = 0.0;
                for( int i = 0; i < input_Num; i++)
                {
                    sum += V_input[j][i] * x[i];
                }
                y[j] = 1.0 / ( 1.0 + exp(-sum - b1[j]));
               // y[j] =( 1.0 - exp(-sum - b1[j])) / ( 1.0 + exp(-sum - b1[j]));
            }

            for( int k = 0; k < output_Num; k++ )
            {
                sum = 0.0;
                for( int j = 0; j < hide_Num; j++ )
                {
                    sum += W_input[k][j] * y[j];
                }
               o[k] =  1.0 / ( 1.0 + exp(-sum - b2[k]));
               // o[k] =   ( 1.0 - exp(-sum - b2[k])) / ( 1.0 + exp(-sum - b2[k]));
            }

            sum = 0.0;
            for( int k = 0; k < output_Num; k++ )
            {
                Error_output[k] = (d[k] - o[k]) * o[k] * ( 1.0 - o[k]);
                for( int j = 0; j < hide_Num; j++ )
                {
                    sum = (W_input[k][j] - W_back[k][j]) * Move_output;
                    W_input[k][j] = W_input[k][j] + NN_Learn_hide * Error_output[k] * y[j] + sum;
                    W_back[k][j] = W_input[k][j];
                }
            }

            for( int j = 0; j < hide_Num; j++ )
            {
                Error_hide[j] =0.0;
                for( int k = 0; k < output_Num; k++ )
                {
                    Error_hide[j] += Error_output[k] * W_input[k][j];
                }
                Error_hide[j] *= y[j] * ( 1.0 - y[j] );

                sum = 0.0;
                for( int i = 0; i < input_Num; i++ )
                {
                    sum = ( V_input[j][i] - V_back[j][i] ) * Move_hide;
                    V_input[j][i] = V_input[j][i] + NN_Learn_output * Error_hide[j] * x[i] + sum;
                    V_back[j][i] = V_input[j][i];
                }
            }

            Error = 0.0;
            for( int k = 0; k < output_Num; k++ )
            {
                Error += fabs( (d[k] - o[k] ) * ( d[k] - o[k]));
            }
            Error /=  2.0;
            for( int k = 0; k < output_Num; k++)
                b2[k] = b2[k] + NN_Learn_Boutput *Error_output[k];
            for( int j = 0; j < hide_Num; j++)
                b1[j] = b1[j] + NN_Learn_Bhide*Error_hide[j];
        }
        //Error = sqrt(Error / output_Num);
        Error = sqrt(Error);
        Num++;
    }
    for( int j = 0; j < hide_Num; j++ )
        delete [] V_back[j];
    for( int k = 0; k < output_Num; k++ )
        delete [] W_back[k];
    delete [] V_back;
    delete [] W_back;
    delete [] x;
    delete [] y;
    delete [] o;
    delete [] d;
    delete [] Error_hide;
    delete [] Error_output;

    std::cout <<"Current Error is "<<Error<<".\n"
             <<"Current Loop Number is "<< Num<<".\n";

    if( Num < Max_LoopNum )
        return true;
    else
        return false;
}

bool ANNTrain::BP_Train02(double **Xinput, double **Dinput, int TrainSample)
{
    double *y = new double[hide_Num];
    double *o = new double[output_Num];
    double *x = new double[input_Num];
    double *d = new double[output_Num];
    //误差
    double *Error_hide = new double[hide_Num];
    double *Error_output = new double[output_Num];

    //权值参数矩阵备份
    double **V_back,**W_back;
    V_back = new double*[hide_Num];
    W_back = new double*[output_Num];
    for( int j = 0; j < hide_Num; j++)
        V_back[j] = new double[input_Num];

    for( int k = 0; k < output_Num; k++)
        W_back[k] = new double[hide_Num];
    for( int j = 0; j < hide_Num; j++ )
        for( int i = 0; i < input_Num; i++)
            V_back[j][i] = V_input[j][i];
    for( int k = 0; k < output_Num; k++ )
        for( int j = 0; j < hide_Num; j++ )
            W_back[k][j] = W_input[k][j];


    double sum = 0.0;
    int Num = 0;
    Error = 1.0;
    while( (Error > Error_Min) && ( Num < Max_LoopNum) )
    {
        for( int isamp = 0; isamp < TrainSample; isamp++ )
        {
            for( int i = 0; i < input_Num; i++)
                x[i] = Xinput[isamp][i];
            for( int k= 0; k < output_Num; k++ )
                d[k] = Dinput[isamp][k];

            for( int j = 0; j < hide_Num; j++ )
            {
                sum = 0.0;
                for( int i = 0; i < input_Num; i++)
                {
                    sum += V_input[j][i] * x[i];
                }
                y[j] = 1.0 / ( 1.0 + exp(-sum - b1[j]));
               // y[j] =( 1.0 - exp(-sum - b1[j])) / ( 1.0 + exp(-sum - b1[j]));
            }

            for( int k = 0; k < output_Num; k++ )
            {
                sum = 0.0;
                for( int j = 0; j < hide_Num; j++ )
                {
                    sum += W_input[k][j] * y[j];
                }
               o[k] =  1.0 / ( 1.0 + exp(-sum - b2[k]));
               // o[k] =   ( 1.0 - exp(-sum - b2[k])) / ( 1.0 + exp(-sum - b2[k]));
            }

            sum = 0.0;
            double aa = 0.1;
            double sum_avg = 0.0;
            for( int j = 0; j < hide_Num; j++ )
                sum_avg += y[j]*y[j];
            sum_avg = sqrt(sum_avg);
            for( int k = 0; k < output_Num; k++ )
            {
                Error_output[k] = (d[k] - o[k]) * o[k] * ( 1.0 - o[k]);
                for( int j = 0; j < hide_Num; j++ )
                {
                    sum = (W_input[k][j] - W_back[k][j]) * Move_output;
                    W_input[k][j] = W_input[k][j] + NN_Learn_hide *( Error_output[k] * y[j])/(aa + sum_avg) + sum;
                    W_back[k][j] = W_input[k][j];
                }
            }

            sum_avg = 0.0;
            for( int i = 0; i < input_Num; i++ )
                sum_avg = x[i]*x[i];
            sum_avg = sqrt(sum_avg);
            for( int j = 0; j < hide_Num; j++ )
            {
                Error_hide[j] =0.0;
                for( int k = 0; k < output_Num; k++ )
                {
                    Error_hide[j] += Error_output[k] * W_input[k][j];
                }
                Error_hide[j] *= y[j] * ( 1.0 - y[j] );

                sum = 0.0;
                for( int i = 0; i < input_Num; i++ )
                {
                    sum = ( V_input[j][i] - V_back[j][i] ) * Move_hide;
                    V_input[j][i] = V_input[j][i] + NN_Learn_output * Error_hide[j] * x[i]/(aa + sum_avg) + sum;
                    V_back[j][i] = V_input[j][i];
                }
            }

            Error = 0.0;
            for( int k = 0; k < output_Num; k++ )
            {
                Error += fabs( (d[k] - o[k] ) * ( d[k] - o[k]));
            }
            Error /=  2.0;
            for( int k = 0; k < output_Num; k++)
                b2[k] = b2[k] + NN_Learn_Boutput *Error_output[k];
            for( int j = 0; j < hide_Num; j++)
                b1[j] = b1[j] + NN_Learn_Bhide*Error_hide[j];
        }
        //Error = sqrt(Error / output_Num);
        Error = sqrt(Error);
        Num++;
    }
    for( int j = 0; j < hide_Num; j++ )
        delete [] V_back[j];
    for( int k = 0; k < output_Num; k++ )
        delete [] W_back[k];
    delete [] V_back;
    delete [] W_back;
    delete [] x;
    delete [] y;
    delete [] o;
    delete [] d;
    delete [] Error_hide;
    delete [] Error_output;

    std::cout <<"Current Error is "<<Error<<".\n"
             <<"Current Loop Number is "<< Num<<".\n";

    if( Num < Max_LoopNum )
        return true;
    else
        return false;
}


void ANNTrain::Output_Weight()
{
    std::cout<<"V:\n";
    for( int i = 0; i < input_Num; i++ )
    {
        for( int j = 0; j < hide_Num; j++ )
        {
            std::cout << V_input[j][i]<<"\t";
        }
        std::cout << std::endl;
    }
    std::cout <<"W:\n";
    for( int j = 0; j < hide_Num; j++ )
    {
        for( int k = 0; k < output_Num; k++ )
        {
            std::cout << W_input[k][j] <<"\t";
        }
        std::cout<<std::endl;
    }
}

bool ANNTrain::OutPutfile_Weigh(const char *filename)
{
    using namespace std;
    ofstream file(filename,ios_base::out);
    if( !file.is_open() )
    {
        cout << "open output weight file error!" << endl;
        return false;
    }

    file << input_Num<<"\t"<<hide_Num<<"\t"<<output_Num<<"\r\n";

    //输出V
    for( int j = 0; j < hide_Num; j ++)
    {
        for( int i = 0; i < input_Num; i++)
        {
            file << V_input[j][i]<<"  ";
        }
        file <<"\r\n";
    }
    file <<"\r\n";
    //输出W
    for( int k = 0; k < output_Num; k++ )
    {
        for( int j = 0; j < hide_Num; j++)
        {
            file << W_input[k][j]<<"  ";
        }
        file << "\r\n";
    }

    //输出隐藏层阀值
    for( int j = 0; j < hide_Num; j++ )
        file << b1[j]<<"  ";
    file <<"\r\n";
    for( int k = 0; k < output_Num; k++ )
        file << b2[k]<<"  ";

    file.close();
    return true;
}

bool ANNTrain::Loadfile_Weigh(const char *filename)
{
    using namespace std;
    ifstream file(filename,ios_base::out);
    if ( !file.is_open() )
    {
        cout <<"open intput weight file error!!!"<<endl;
        return false;
    }
    if(is_Data)
        Clear_Weigh();
    file >> input_Num>>hide_Num>>output_Num;    //读入三层神经元个数

    InitNet(input_Num,hide_Num,output_Num);
    for( int j = 0; j < hide_Num; j++ )
        for( int i = 0; i < input_Num; i++ )
            file >>V_input[j][i];
    for( int k = 0; k < output_Num; k++)
        for( int j = 0; j < hide_Num; j++ )
            file >>W_input[k][j];
    for( int j = 0; j < hide_Num; j++ )
        file >> b1[j];
    for( int k = 0; k < output_Num; k++ )
        file >> b2[k];
    file.close();
    Output_Weight();
    return true;
}

void ANNTrain::BP_Count(double *x, double *O)
{
    double *Y = new double[hide_Num];
    //double *O = new double[output_Num];

    //求取隐藏层的值
    double sum = 0;
    for( int j = 0; j < hide_Num; j++)
    {
        sum = 0.0;
        for( int i = 0; i < input_Num; i++)
        {
            sum += x[i] * V_input[j][i];
        }
        Y[j] = 1.0 / ( 1.0 + exp(-sum - b1[j]));
        //Y[j] =  ( 1.0 - exp(-sum - b1[j]))/ ( 1.0 + exp(-sum - b1[j]));
    }

    //求取输出层的值
    for( int k =0; k < output_Num; k++ )
    {
        sum = 0.0;
        for( int j = 0; j < hide_Num; j++)
        {
            sum += Y[j] * W_input[k][j];
        }
        O[k] =   1.0 / ( 1.0 + exp(-sum - b2[k]));
        // O[k] =   ( 1.0 - exp(-sum - b2[k])) / ( 1.0 + exp(-sum - b2[k]));
    }
    delete [] Y;
}

