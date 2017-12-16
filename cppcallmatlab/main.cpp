#include <stdio.h>  
#include <iostream>  
#include "mclmcr.h"  
#include "matrix.h"  
#include "mclcppclass.h"  
#include "l1_ls.h"  
using namespace std;

#pragma comment(lib,"l1_ls.lib")  

int main()
{
	if (!l1_lsInitialize())
	{
		cout <<"Could not initialize addfunction!"<< endl;
	}
	else
	{
		cout << "success init!" << endl;
	}



	mwArray x(32, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray status(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray history(32, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray A(1, 1, mxDOUBLE_CLASS, mxREAL);
	mwArray varargin(1, 1, mxDOUBLE_CLASS, mxREAL);
	double data1 = 2; double data2 = 3;
	a.SetData(&data1, 1);
	b.SetData(&data2, 1);
	result.SetData(&data2, 1);

	l1_ls(2, result, a, b);  //调用自己用matlab写的函数  
	x = a.Get(1, 1);
	y = b.Get(1, 1);
	z = result.Get(1, 1);
	printf("%lf\n", x);
	printf("%lf\n", y);
	printf("%lf\n", z);

	l1_lsTerminate();



	getchar();

}