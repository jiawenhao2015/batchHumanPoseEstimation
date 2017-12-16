#ifndef _LOADING_H
#define _LOADING_H


#include "Data.h"
#include"CalculateAngle.h"

#include <io.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "highgui.h"
#include <map>
using namespace cv;
using namespace std;
class SeedPoint//���ӵ��� ��Ҫ����< ��������map��
{
public:
	int x;
	int y;
	friend bool operator<(const SeedPoint& a, const SeedPoint& b)
	{
		if (a.x < b.x) return true;
		if (a.x == b.x)return a.y < b.y;//���x��ȾͱȽ�y
		return false;	
	}
	SeedPoint(int a,int b)
		:x(a), y(b)
	{}
	~SeedPoint(){}

};
class Loading
{
public:

	int TotalPoints;
	void Load_Info(string infoName, vector<Data>& src_data);
	void DownLoad_Info(string infoName, vector<Data>& src_data, int scale);	
	
	bool Test(int CenterX, int CenterY, vector<Data> &src_data);//
	int Search(int CenterX, int CenterY, vector<Data> src_data);



	/*�ϱ���������ʦ��д�� �±���jwhд��*/
	int Search_Adjust(int &x, int &y, vector<Data> src_data);
	int Get_Depth_Info(Data &data, vector<Data> src_data, int index, MyPoint &point);
	void LoadStandardSkeleton(string path, vector<MyPoint> &skeleton, vector<Data> src);
	void Generate_Center_Point(vector<Data> &src_data, int &CenterX, int &CenterY, int windowSize, Mat slicBody);//
	int SearchInSlic(int CenterX, int CenterY, Mat& seedsBody);
	void Load_Info(string infoName, vector<Data>& src_data, int newmethod);

	void Generate_Center_Point(vector<Data> &src_data, int &CenterX, int &CenterY);

	map<SeedPoint, int>seedsmap;
	//map�ؼ��ֵ���Ҫ�������򷽷��ģ�������벻ͨ�� ƽ����string int�Ǵ������ص�
	int window;//Ѱ�ҳ����ص��ʱ���õ��� ��Χ����
	Loading(){}
	~Loading(){}

};

#endif