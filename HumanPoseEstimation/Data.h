#ifndef _DATA_H
#define _DATA_H


using namespace std;

class Data//���ڴ洢���Ƶ����ݽṹ
{
public:
	int x;
	int y;
	float Tx;
	float Ty;
	float Tz;

	friend bool operator<(const Data& a,const Data& b)   //��Ҫʵ����С�ѣ����������У������Ҫ������������ض������ȼ�����СΪ��  
	{  
		if (a.x < b.x)	return true;
		
		if (a.x == b.x)return a.y < b.y;//���x��ȾͱȽ�y
		return false;
	}


	Data(){}
	~Data(){}
};

#endif