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
//һЩ�������� ���ߺ����ŵ��˴�
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

class Data//���ڴ洢���Ƶ����ݽṹ
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