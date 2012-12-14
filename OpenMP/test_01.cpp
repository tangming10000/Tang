#include "omp.h"
#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;
int main()
{
        time_t t1,t2;
        t1 = time(0);
        int i,N=100000000;
        double sum = 0;
        cout <<"with out openMP:"<<endl;
        for( i = 1; i < N; i++ )
          sum += sin(i*0.001)*sin(i*0.001) - cos(i*0.001)*cos(i*0.001) ;
        cout << "The result of sum is " << sum << endl;
        cout << "The total time is "<< time(0)-t1<<"s."<< endl;

        cout <<"With the OpenMP"<< endl;
        t2=time(0);
        sum = 0;
        double lsum = 0;
        #pragma omp parallel private (lsum)
        {
                lsum = 0;
                #pragma omp for
                for(int i = 1; i < N; i++ )
                        lsum = lsum + sin(i*0.001)*sin(i*0.001) - cos(i*0.001)*cos(i*0.001);
                #pragma omp critical
                {
                        sum += lsum;
                }
        }
        cout <<"The Sum is "<< sum << endl;
        cout <<"The total time is "<< time(0)-t2<<"s."<<endl;
        return 0;
}