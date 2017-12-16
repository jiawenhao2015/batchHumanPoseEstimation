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
class SeedPoint//种子点类 需要重载< 才能用在map里
{
public:
	int x;
	int y;
	friend bool operator<(const SeedPoint& a, const SeedPoint& b)
	{
		if (a.x < b.x) return true;
		if (a.x == b.x)return a.y < b.y;//如果x相等就比较y
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



	/*上边是刘洪林师兄写的 下边是jwh写的*/
	int Search_Adjust(int &x, int &y, vector<Data> src_data);
	int Get_Depth_Info(Data &data, vector<Data> src_data, int index, MyPoint &point);
	void LoadStandardSkeleton(string path, vector<MyPoint> &skeleton, vector<Data> src);
	void Generate_Center_Point(vector<Data> &src_data, int &CenterX, int &CenterY, int windowSize, Mat slicBody);//
	int SearchInSlic(int CenterX, int CenterY, Mat& seedsBody);
	void Load_Info(string infoName, vector<Data>& src_data, int newmethod);

	void Generate_Center_Point(vector<Data> &src_data, int &CenterX, int &CenterY);

	map<SeedPoint, int>seedsmap;
	//map关键字得需要重载排序方法的，否则编译不通过 平常的string int是存在重载的
	int window;//寻找超像素点的时候用到的 范围窗口
	Loading(){}
	~Loading(){}

};

#endif