#ifndef _POSEMEASURE_H
#define _POSEMEASURE_H

#include <iostream>
#include <iomanip>
#include<string>
#include <vector>
#include <set>
#include <fstream>
#include<map>
#include <sstream>
#include"highgui.h"
#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
using namespace std;
using namespace cv;

//姿态度量相关函数

class FileTool
{
public:
	void ReadmidGT(string matrixPath, vector<vector<float>>& vec);
	void Write2GT(string matrixPath, vector<vector<int>>& vec);
	void Write2Fea(string matrixPath, vector<vector<int>>& vec);
	void Read2GT(string matrixPath, vector<vector<int>>& vec);
	void Read2Fea(string matrixPath, vector<vector<int>>& vec);
	void Read3GT(string matrixPath, map<vector<int>, vector<float>>& mp);
	Mat InitMat(string matrixPath, int m, int n, bool containlabel, int& label);


};
struct S_Point
{
	double x;
	double y;
	double z;
	S_Point(vector<float>p)
	{
		x = p[0], y = p[1], z = p[2];
	}
};

class PoseMeasure
{
public:
	int get3dGT(int actionBegin, int actionEnd, int peopleBegin, int peopleEnd, int indexBegin, int indexEnd);
	int get3dFea(int actionBegin, int actionEnd, int peopleBegin, int peopleEnd, int indexBegin, int indexEnd, int dim = 3);
	void  creatGroundTruthFeature(int actionBegin, int actionEnd, int peopleBegin, int peopleEnd, int indexBegin, int indexEnd);
	void  creatClusterFeature(int actionBegin, int actionEnd, int peopleBegin, int peopleEnd, int indexBegin, int indexEnd, int dim = 3);
	void  creatClusterFeature2(int actionBegin, int actionEnd,int peopleBegin, int peopleEnd,int indexBegin, int indexEnd);
	void  creatClusterFeature3(int actionBegin, int actionEnd, int peopleBegin, int peopleEnd, int indexBegin, int indexEnd, int dim = 3);
	void  creatClusterFeature4(int actionBegin, int actionEnd, int peopleBegin, int peopleEnd, int indexBegin, int indexEnd, int dim = 3);
	void  creatClusterFeatureDianxianmian(int actionBegin, int actionEnd, int peopleBegin, int peopleEnd, int indexBegin, int indexEnd, int dim = 3);

	void Normalization(Mat& mat);
	int knn(vector<Mat>&trainSample, vector<int>&trainLabel, Mat &test, int testindex,
		Mat& matrix, int k, string prefix, int actionBegin, int actionEnd, string matrixName);
	void getTrainAndTestData(vector<Mat>& trainSample, vector<Mat>& testSample,
		vector<int>& trainLabel, vector<int>& testLabel,
		string prefix, int row, int col, bool isjulei,
		int actionBegin, int actionEnd,int peopleBegin, int peopleEnd,
		int indexBegin, int indexEnd,int jiange,int dim = 3);
	void testknn(bool isjulei, int k, int startindex,int actionBegin, int actionEnd,int peopleBegin, int peopleEnd,	int indexBegin, int indexEnd,
		string matrixName, int col, int jiange, int dim = 3);
	int minDistance(vector<int>&input, vector<vector<int>>& gt2, vector<bool>&flag);
	void adjustClusterPoint(vector<vector<int>>& gt2);
	void adjustClusterPoint();
	void adjustNotOnBody(vector<vector<int>>& gt2);
	void adjustNotOnBody();
	void jiaozhun();


	float EucDis(vector<float>&a, vector<float>&b);
	vector<float> NormalizationUnit(vector<float>&a);

	double DistanceOfPointToLine(S_Point* a, S_Point* b, S_Point* s);


	FileTool filetool;

	map<int, int>indexmp;//记录训练集里面的帧的索引 对应实际的硬盘上的位置 比如训练集数组第0个代表硬盘上第一个姿态。
	map<int, int>indexmptest;//记录ceshi集里面的帧的索引
	PoseMeasure(){}
	~PoseMeasure(){}
};

//读写文件相关

#endif