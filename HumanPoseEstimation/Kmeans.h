#ifndef _KMEANS_H
#define _KMEANS_H

/*kmeans算法实现（此处只考虑元组只有两个属性的情况）
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class Kmeans
{
public:
	int k;
	int downSampling;
	//CvPoint2D32f t.
	float getDistXY(CvPoint2D32f a, CvPoint2D32f b);
	int clusterOfPoint(CvPoint2D32f means[], CvPoint2D32f newPoint);
	CvPoint2D32f getMeans(vector<CvPoint2D32f> cluster);
	float getVar(vector<CvPoint2D32f>clusters[], CvPoint2D32f means[]);

	Kmeans(int m, int sampling);
	void K_solve(vector<CvPoint2D32f> tuples, int class_k, CvPoint2D32f gujia[], const int Num);
	//针对下采样的聚类
	void K_solve(vector<CvPoint2D32f> tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag);
	void K_solveSuperPixel(vector<CvPoint2D32f> tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag);
	void K_solveSuperPixel4(vector<CvPoint2D32f> tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag);

	void K_solve(vector<CvPoint2D32f> tuples);

	vector<CvPoint> getNeighbors(int x, int y);
	string outputPath;
};

#endif