#include <string>
#include <iostream>
#include <sstream>
using namespace std;
int main()
{
	string filename;
	stringstream str01;
	for( int i = 0; i < 100; i++ )
	{
		str01.str("");
		filename="";
		str01<<"jieguo"<<i<<".txt";
		filename = str01.str();
		cout <<filename<<endl;
	}
	return 0;
}
