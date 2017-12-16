#ifndef _ImageShow_H
#define _ImageShow_H

#include "Highgui.h"
#include "io.h"
#include "Data.h"

#include <fstream>

using namespace std;
using namespace cv;

class ImageShow
{
public:
	//Mat photo(480,640,CV_8UC1);
	void ShowPhoto(Mat photo, vector<Data>src);
	void ShowPhoto(Mat photo, vector<Data>src, Mat& depthData);
	void ShowCluster(Mat photo, int one, int two, int three, char* file_path, vector<CvPoint2D32f> &tuple);
	void ShowCluster(Mat photo, int one, int two, int three, char* file_path);
	void ShowVertex(Mat photo, int Vertex[6][2]);
	void ShowPoint(Mat photo, int x, int y, int b, int g, int r);


};

#endif