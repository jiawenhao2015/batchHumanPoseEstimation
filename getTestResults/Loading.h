#ifndef _LOADING_H
#define _LOADING_H


#include "Data.h"

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
	SeedPoint(int a, int b)
		:x(a), y(b)
	{}
	~SeedPoint(){}

};
class Loading
{
public:

	int TotalPoints;
	int  Search(int CenterX, int CenterY, vector<Data> src_data);
	void DownLoad_XuNiData(string infoName, vector<Data>&src_data, int scale);
	void DownLoad_Info(string infoName, vector<Data>& src_data, int scale);
	//void DownLoadKinectInfo(string infoName, vector<Data>& src_data, int scale);
	pair<int, int> SearchInNeighbor(int x, int y, Mat& photo);

		
	
	Loading(){}
	~Loading(){}

};

#endif