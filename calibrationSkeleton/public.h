#ifndef _PUBLIC_H
#define _PUBLIC_H

#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <highgui.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;
//一些公共参数 工具函数放到此处
#define HEIGHT 424
#define WIDTH   512
#define JOINT_NUM 15
#define NUM_SUPERPIXEL 2500

class Public
{
public:
	Mat InitMat(string matrixPath, int m, int n);
	void WriteMat(Mat& m, string path);
	void DrawSkeleton(Mat& img, Mat& jointsMatrix);
	void DrawSkeleton(Mat& img, vector<float>& x, vector<float>& y);
	vector<string> JOINTS;
	Mat img;
	int frame;
	Public();
	~Public(){}

};

class Data//用于存储点云的数据结构
{
public:
	int x;
	int y;
	float Tx;
	float Ty;
	float Tz;
	
	Data(){}
	~Data(){}
};
#endif