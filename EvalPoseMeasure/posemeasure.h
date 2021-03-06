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


//读写文件相关
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
	void  resultPicMerge(int testindex);
	bool ReadFile(string filePath, vector<float>&errorVec);

	void jiaozheng();

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
//姿态度量相关函数
class PoseMeasure
{
public:
	int get3dGT(int actionBegin, int actionEnd, int indexBegin, int indexEnd);
	int get3dFea(int actionBegin, int actionEnd, int indexBegin, int indexEnd, int dim = 3);
	void  creatGroundTruthFeatureDianxianmian(int actionBegin, int actionEnd,int indexBegin, int indexEnd);
	void  creatGroundTruthFeatureDianxianmian222(int actionBegin, int actionEnd, int indexBegin, int indexEnd);

	void  creatClusterFeatureDianxianmian(int actionBegin, int actionEnd,int indexBegin, int indexEnd, int dim = 3);
	void gather3dFeature(int actionBegin, int actionEnd, int indexBegin, int indexEnd);
	void gather3dGTFeature(int actionBegin, int actionEnd, int indexBegin, int indexEnd);

	void Normalization(Mat& mat);
	int knn(vector<Mat>&trainSample, vector<int>&trainLabel, Mat &test, int testindex,
		Mat& matrix, int k, string prefix, int actionBegin, int actionEnd, string matrixName);
	void getTrainAndTestData(vector<Mat>& trainSample, vector<Mat>& testSample,
		vector<int>& trainLabel, vector<int>& testLabel,
		string prefix, int row, int col, bool isjulei,
		int actionBegin, int actionEnd,int indexBegin, int indexEnd,int jiange,int dim = 3);
	void testknn(bool isjulei, int k, int startindex,int actionBegin, int actionEnd,int indexBegin, int indexEnd,
		string matrixName, int col, int jiange, int dim = 3);
	int minDistance(vector<int>&input, vector<vector<int>>& gt2, vector<bool>&flag);
	void adjustClusterPoint(vector<vector<int>>& gt2);
	void adjustClusterPoint();
	void adjustNotOnBody(vector<vector<int>>& gt2);
	void adjustNotOnBody();
	void jiaozhun();
	int SetLabel(int action, int people, int index);
	bool evaluatePrecision(int testindex, vector<int>& result, int threshold);
	int  evaluatePrecision2(int testindex, vector<int>& result);

	float EucDis(vector<float>&a, vector<float>&b);
	vector<float> NormalizationUnit(vector<float>&a);

	double DistanceOfPointToLine(S_Point* a, S_Point* b, S_Point* s);

	void cvFitPlane(const CvMat* points, float* plane);
	vector<float> getPlaneNorm(vector<vector<float>>&part);

	float getTwoNormalAngle(vector<float>&norm1, vector<float>&norm2);
	FileTool filetool;

	map<int, int>mplabel;//索引对应的标签
	vector<int> knnresult;//knn结果  前k个相似的图片索引
	map<int, int>indexmp;//记录训练集里面的帧的索引 对应实际的硬盘上的位置 比如训练集数组第0个代表硬盘上第一个姿态。
	map<int, int>indexmptest;//记录ceshi集里面的帧的索引
	PoseMeasure(){}
	~PoseMeasure(){}
};



#endif