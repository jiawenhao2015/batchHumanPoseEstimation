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
	
	void ShowPhoto(Mat& photo, vector<Data>&src);
	
	Mat ImageShow::getPhoto(vector<Data>src);

};

#endif