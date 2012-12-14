#include <stdio.h>
#include <math.h>
double distance(double x1,double y1,double x2,double y2)
{
	double dx = x2-x1;
	double dy = y2-y1;
	double dis = dx*dx+dy*dy;
	return sqrt(dis);
}


int main(int argc,char* argv[])
{
	int x1 = atoi(argv[1]);
	int y1 = atoi(argv[2]);
	int x2 = atoi(argv[3]);
	int y2 = atoi(argv[4]);

	printf("The distance of (%f,%f) to (%f,%f) is %f.\n",x1,y1,x2,y2,distance(x1,y1,x2,y2));
	return 0;
}
