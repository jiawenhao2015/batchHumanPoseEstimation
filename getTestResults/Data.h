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

	//friend bool operator<(Data a, Data b)   //��Ҫʵ����С�ѣ����������У������Ҫ������������ض������ȼ�����СΪ��  
	//{  
	//	return a.weight > b.weight;  
	//}  


	Data(){}
	~Data(){}
};

#endif