
#include "ImageShow.h"
#include "public.h"
#define HEIGHT 424
#define WIDTH  512


Mat ImageShow::getPhoto(vector<Data>src)//显示原始图片信息：背景黑色，人白色
{
	Mat photo(HEIGHT, WIDTH, CV_8UC3);
	
	double minD = 1000, maxD = 0;
	for (int i = 0; i<src.size(); i++)
	{
		if (src[i].Tz>maxD)
			maxD = src[i].Tz;
		if (src[i].Tz < minD)
			minD = src[i].Tz;
	}
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			photo.at<Vec3b>(i, j)[0] = 255;
			photo.at<Vec3b>(i, j)[1] = 255;
			photo.at<Vec3b>(i, j)[2] = 255;
		}
	}
	for (int j = 0; j < src.size(); j++)
	{
		photo.at<Vec3b>(src[j].y, src[j].x)[0] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
		photo.at<Vec3b>(src[j].y, src[j].x)[1] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
		photo.at<Vec3b>(src[j].y, src[j].x)[2] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
	}
	return photo;
}