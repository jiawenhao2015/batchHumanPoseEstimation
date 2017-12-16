
#include "ImageShow.h"
#define HEIGHT 480
#define WIDTH   640
void ImageShow::ShowCluster(Mat photo, int one, int two, int three, char* file_path)//��ʾ����Ч��
{
	float x, y;
	int Cx = 0, Cy = 0;//
	if (_access(file_path, 0) == -1)//ͷ�ļ�io.h���ж��ļ��Ƿ����
	{
		printf("The path of the file is wrong...");
	}
	else
	{
		fstream fp;
		fp.open(file_path, ios::in);

		int num = 0;
		while (!fp.eof())//feof()���ط����ʾ�����ļ�β
		{
			fp >> x;
			fp >> y;
			Cx += x;//
			Cy += y;//
			photo.at<Vec3b>(y, x)[0] = one;
			photo.at<Vec3b>(y, x)[1] = two;
			photo.at<Vec3b>(y, x)[2] = three;

			photo.at<Vec3b>(y - 1, x)[0] = one;
			photo.at<Vec3b>(y - 1, x)[1] = two;
			photo.at<Vec3b>(y - 1, x)[2] = three;
			photo.at<Vec3b>(y + 1, x)[0] = one;
			photo.at<Vec3b>(y + 1, x)[1] = two;
			photo.at<Vec3b>(y + 1, x)[2] = three;
			photo.at<Vec3b>(y, x - 1)[0] = one;
			photo.at<Vec3b>(y, x - 1)[1] = two;
			photo.at<Vec3b>(y, x - 1)[2] = three;
			photo.at<Vec3b>(y, x + 1)[0] = one;
			photo.at<Vec3b>(y, x + 1)[1] = two;
			photo.at<Vec3b>(y, x + 1)[2] = three;
			photo.at<Vec3b>(y - 1, x - 1)[0] = one;
			photo.at<Vec3b>(y - 1, x - 1)[1] = two;
			photo.at<Vec3b>(y - 1, x - 1)[2] = three;
			photo.at<Vec3b>(y + 1, x + 1)[0] = one;
			photo.at<Vec3b>(y + 1, x + 1)[1] = two;
			photo.at<Vec3b>(y + 1, x + 1)[2] = three;
			photo.at<Vec3b>(y + 1, x - 1)[0] = one;
			photo.at<Vec3b>(y + 1, x - 1)[1] = two;
			photo.at<Vec3b>(y + 1, x - 1)[2] = three;
			photo.at<Vec3b>(y - 1, x + 1)[0] = one;
			photo.at<Vec3b>(y - 1, x + 1)[1] = two;
			photo.at<Vec3b>(y - 1, x + 1)[2] = three;
			//tuple.push_back(t);
			num++;
		}
		//eof������������������ʱ��ֵFALSE��������һ�����ݶ����һ��
		//PointSet.pop_back();

		Cx = Cx / num;//
		Cy = Cy / num;//
		///////////////////////////////////
		//for(int m=Cy-1;m<=Cy+1;m++)
		//	for(int n=Cx-1;n<=Cx+1;n++)
		//	{
		//		//photo.at<uchar>(m,n)=100;
		//		photo.at<Vec3b>(m,n)[0]=255;
		//		photo.at<Vec3b>(m,n)[1]=255;
		//		photo.at<Vec3b>(m,n)[2]=255;
		//	}
		/////////////////////////////////

		printf("%d  success\n", num - 1);
		fp.close();
	}

	namedWindow("show");
	imshow("show", photo);
	waitKey(1);
}

void ImageShow::ShowCluster(Mat photo, int one, int two, int three, char* file_path, vector<CvPoint2D32f>&tuple)//��ʾ����Ч��
{	
	CvPoint2D32f t;	
	if (_access(file_path, 0) == -1)//ͷ�ļ�io.h���ж��ļ��Ƿ����
	{
		printf("The path of the file is wrong...");
	}
	else
	{
		fstream fp;
		fp.open(file_path, ios::in);

		int num = 0;
		while (!fp.eof())//feof()���ط����ʾ�����ļ�β
		{
			fp >> t.x;
			fp >> t.y;
			//fp>>Tx;
			//fp>>Ty;
			//fp>>Tz;
			/*photo.at<Vec3b>(t.y, t.x)[0] = one;
			photo.at<Vec3b>(t.y, t.x)[1] = two;
			photo.at<Vec3b>(t.y, t.x)[2] = three;*///��ʱ��circle����Ч��


			circle(photo, Point(t.x, t.y), 2, Scalar(one, two, three), -1);

			tuple.push_back(t);
			num++;
		}
		//eof������������������ʱ��ֵFALSE��������һ�����ݶ����һ��
		//PointSet.pop_back();

		printf("%d  success\n", num - 1);
		fp.close();
	}
	
	namedWindow("show");
	imshow("show", photo);
	waitKey(1);
}

void ImageShow::ShowVertex(Mat photo, int Vertex[6][2])//��ʾ֫�˵�
{
	for (int i = 0; i<6; i++)
	{
		int x = Vertex[i][0], y = Vertex[i][1];
		for (int m = y - 1; m <= y + 1; m++)
		for (int n = x - 1; n <= x + 1; n++)
		{
			//photo.at<uchar>(m,n)=100;
			photo.at<Vec3b>(m, n)[0] = 255;
			photo.at<Vec3b>(m, n)[1] = 0;
			photo.at<Vec3b>(m, n)[2] = 0;
		}
	}
	namedWindow("show");
	imshow("show", photo);
	waitKey(1);
}
void ImageShow::ShowPoint(Mat photo, int x, int y, int b, int g, int r)
{
	for (int m = y - 1; m <= y + 1; m++)
	for (int n = x - 1; n <= x + 1; n++)
	{
		//photo.at<uchar>(m,n)=100;
		photo.at<Vec3b>(m, n)[0] = b;
		photo.at<Vec3b>(m, n)[1] = g;
		photo.at<Vec3b>(m, n)[2] = r;
	}
	namedWindow("show");
	imshow("show", photo);
	waitKey(1);
}
void ImageShow::ShowPhoto(Mat photo, vector<Data>src)//��ʾԭʼͼƬ��Ϣ��������ɫ���˰�ɫ
{
	double minD = 1000, maxD = 0;
	for (int i = 0; i<src.size(); i++)
	{
		if (src[i].Tz>maxD)
			maxD = src[i].Tz;
		if (src[i].Tz<minD)
			minD = src[i].Tz;
	}
	for (int i = 0; i<HEIGHT; i++)
	{
		//uchar* data=photo.ptr<uchar>(i);
		for (int j = 0; j<WIDTH; j++)
		{	//data[j]=0;
			photo.at<Vec3b>(i, j)[0] = 255;
			photo.at<Vec3b>(i, j)[1] = 255;
			photo.at<Vec3b>(i, j)[2] = 255;
		}
	}
	for (int j = 0; j<src.size(); j++)
	{
		photo.at<Vec3b>(src[j].y, src[j].x)[0] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
		photo.at<Vec3b>(src[j].y, src[j].x)[1] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
		photo.at<Vec3b>(src[j].y, src[j].x)[2] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
	}
	namedWindow("show");
	imshow("show", photo);
	waitKey(1);
}
//��ʾԭʼͼƬ��Ϣ��������ɫ���˰�ɫ depthData �����������
void ImageShow::ShowPhoto(Mat photo, vector<Data>src, Mat& depthData)
{
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
		//uchar* data=photo.ptr<uchar>(i);
		for (int j = 0; j < WIDTH; j++)
		{
			//data[j]=0;
			photo.at<Vec3b>(i, j)[0] = 255;
			photo.at<Vec3b>(i, j)[1] = 255;
			photo.at<Vec3b>(i, j)[2] = 255;

			depthData.at<Vec3d>(i, j)[0] = 255;
			depthData.at<Vec3d>(i, j)[1] = 255;
			depthData.at<Vec3d>(i, j)[2] = 255;
		}
	}
	for (int j = 0; j < src.size(); j++)
	{
		photo.at<Vec3b>(src[j].y, src[j].x)[0] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
		photo.at<Vec3b>(src[j].y, src[j].x)[1] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));
		photo.at<Vec3b>(src[j].y, src[j].x)[2] = (int)((src[j].Tz - minD)*(200.0 / (maxD - minD)));

		/*cv::Point position(src[j].x, src[j].y);
		circle(photo, position, 2, Scalar(0, 0, 255), -1);*/

		depthData.at<Vec3d>(src[j].y, src[j].x)[0] = src[j].Tx;//////////���� ��� ����
		depthData.at<Vec3d>(src[j].y, src[j].x)[1] = src[j].Ty;
		depthData.at<Vec3d>(src[j].y, src[j].x)[2] = src[j].Tz;
	}
	namedWindow("show");
	imshow("show", photo);
	waitKey(1);
}