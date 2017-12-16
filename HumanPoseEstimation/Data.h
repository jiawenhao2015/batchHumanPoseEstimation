#ifndef _DATA_H
#define _DATA_H


using namespace std;

class Data//用于存储点云的数据结构
{
public:
	int x;
	int y;
	float Tx;
	float Ty;
	float Tz;

	friend bool operator<(const Data& a,const Data& b)   //因要实现最小堆，按升序排列，因而需要重载运算符，重定义优先级，以小为先  
	{  
		if (a.x < b.x)	return true;
		
		if (a.x == b.x)return a.y < b.y;//如果x相等就比较y
		return false;
	}


	Data(){}
	~Data(){}
};

#endif