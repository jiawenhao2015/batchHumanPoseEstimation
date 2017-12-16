#ifndef _SKELETON_H
#define _SKELETON_H

/*kmeans算法实现（此处只考虑元组只有两个属性的情况）
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class Skeleton
{
public:
	void ProcessingSkeleton(Mat photo,float** Dist,CvPoint2D32f gujiadian[5][3],int gujiadian2 [5][3],int Vertex[6][2],int vvv,int lefthand,int leftfoot,int righthand,int rightfoot,CvPoint2D32f Skeleton_point[16],const int Num);
	void ProcessingSkeleton4(Mat photo, float** Dist, CvPoint2D32f gujiadian[5][4], int gujiadian2[5][4], int Vertex[6][2], int vvv, int lefthand, int leftfoot, int righthand, int rightfoot, CvPoint2D32f Skeleton_point[21], const int Num);

};

#endif