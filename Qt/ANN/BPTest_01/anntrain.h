#ifndef ANNTRAIN_H_
#define ANNTRAIN_H_
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <fstream>


const double PI = 3.1415926;
class ANNTrain
{
public:
    ANNTrain();
    ~ANNTrain();

public: //BP神经网络参数设置
    int input_Num;  //输入层节点个数
    int hide_Num;   //隐藏层节点个数
    int output_Num; //输出层节点个数
    int Max_LoopNum;//最大循环次数
    double Error_Min;   //训练最小误差

    double NN_Learn_hide,NN_Learn_output;    //权值学习率
    double NN_Learn_Bhide,NN_Learn_Boutput;  //阀值学习率
    double Move_hide,Move_output;   //输出动量
    double **V_input,**W_input;   //输入层权值矩阵

    double *X,*Y,*O,*D;
    double *b1,*b2;      //阀值
    bool is_Data;       //判断数组内是否有值
    double Error;

public://接口函数
    void InitNet(int input, int hide, int output);  //初始化网络
    void set_hideLearnRatio(double learn);          //设置隐层学习率
    void set_outputLearnRatio(double learn);        //输出层学习率
    void set_hideThresholdRation(double learn);     //隐层阀值学习率
    void set_outputThresholdRation(double learn);   //输出层阀值学习率
    void set_hideMove(double move);                 //设置隐层动量因子
    void set_outputMove(double move);               //设置输出层的动量因子
    void set_MinError(double error);                //设置训练误差
    void set_MaxLoopNum(int Num);                   //设置最大循环次数


    bool BP_Train01(double **x, double **d, int TrainSample);//标准LMS
    bool BP_Train02(double **x, double **d, int TrainSample);//修正规范化LMS算法
    void BP_Count(double *x, double *d);            //信号计算
    void Output_Weight();
    bool OutPutfile_Weigh(const char* filename);        //输出权值
    bool Loadfile_Weigh(const char* filename);          //加载权值

private://相关函数
    void Clear_Weigh();     //清空权值
    void Init_Weigh();      //初始化权值




};

#endif // ANNTRAN_H
