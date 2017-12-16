#include "public.h"

Public::Public()
{
	JOINTS = { "hip", "chest", "neck", "lShldr", "lForeArm", "lHand", "rShldr", "rForeArm", "rHand", "lThigh", "lShin", "lFoot", "rThigh", "rShin", "rFoot" };
}
// ����һ��txt ����һ��m*n�ľ���
Mat Public::InitMat(string matrixPath, int m, int n)
{
	ifstream matrixFile(matrixPath);
	float temp;
	Mat mat(m, n, CV_32F);
	vector<float>xVec;//������������
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> temp;
			xVec.push_back(temp);
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << matrixPath.c_str() << endl;
		return mat;
	}
	xVec.erase(xVec.end() - 1);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat.at<float>(i, j) = xVec[i * n + j];
		}
	}
	return mat;
}
//��mat���� float���� ���浽path�� ��ʽΪ 
void Public::WriteMat(Mat& m, string path)
{
	if (m.empty())return;
	ofstream of(path);
	for (int i = 0; i < m.rows;i++)
	{
		for (int j = 0; j < m.cols;j++)
		{
			of << m.at<float>(i, j) << " ";			
		}
		of << endl;
	}
	of.close();
}

/*
�������ݽ��ؽڵ�λ���������� ��Ϊһ���Ǽ�ģ��
01 hip
23 chest
45 neck
67 lShldr
89 lForeArm
10 11 lHand
12 13 rShldr
14 15 rForeArm
16 17 rHand
18 19 lThigh
20 21 lShin
22 23 lFoot
24 25 rThigh
26 27 rShin
28 29 rFoot
*/
void Public::DrawSkeleton(Mat& img, Mat& jointsMatrix)
{
	int thickness = 2;
	int lineType = 8;
	Scalar sca(0, 255, 0);

	vector<float>x, y;

	for (int i = 0; i < jointsMatrix.cols / 2; i++)
	{//ֻȡ��һ��
		x.push_back(jointsMatrix.at<float>(0, 2 * i));
		y.push_back(jointsMatrix.at<float>(0, 2 * i + 1));
	}
	for (int i = 0; i < 15; i++)
	{
		if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14)continue;

		line(img, Point(x[i], y[i]), Point(x[i + 1], y[i + 1]), sca, thickness, lineType);

	}
	line(img, Point(x[0], y[0]), Point(x[9], y[9]), sca, thickness, lineType);//hip--lthigh
	line(img, Point(x[0], y[0]), Point(x[12], y[12]), sca, thickness, lineType);//hip--rthigh
	line(img, Point(x[2], y[2]), Point(x[3], y[3]), sca, thickness, lineType);//neck--lshldr
	line(img, Point(x[2], y[2]), Point(x[6], y[6]), sca, thickness, lineType);//neck--rshldr
}
void Public::DrawSkeleton(Mat& photo, vector<float>& x, vector<float>& y)
{
	if (photo.empty())return;
	photo.copyTo(img);

	int thickness = 1;
	int lineType = 8;
	Scalar sca(0, 255, 0);
	char index[20];

	int vy = 55;
	int vx = 10;
	putText(img, "frame:"+to_string(frame), Point(vx, vy += 15), 1, 1, Scalar(0, 0, 0));
	putText(img, "s--save", Point(vx, vy += 15), 1, 1, Scalar(0, 0, 0));
	putText(img, "n--next", Point(vx, vy += 15), 1, 1, Scalar(0, 0, 0));
	putText(img, "p--previous", Point(vx, vy += 15), 1, 1, Scalar(0, 0, 0));
	
	for (int i = 0; i < JOINT_NUM; i++)
	{
		///У�� x y���� �����ڻ�����
		x[i] = max((float)20.0, x[i]);
		x[i] = min((float)WIDTH - 5, x[i]);
		y[i] = max((float)25.0, y[i]);
		y[i] = min((float)HEIGHT - 5, y[i]);
	}
	
	for (int i = 0; i < JOINT_NUM; i++)
	{
		sprintf_s(index, "%d", i);
		circle(img, Point(x[i], y[i]), 2, Scalar(0, 255, 0), 2, 8);
		putText(img, JOINTS[i], Point(x[i], y[i]), 1, 1, Scalar(0, 0, 255));
		putText(img, index, Point(x[i] + 5, y[i] - 10), 1, 1, Scalar(0, 0, 255));

		if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14)continue;

		line(img, Point(x[i], y[i]), Point(x[i + 1], y[i + 1]), sca, thickness, lineType);
	}
	line(img, Point(x[0], y[0]), Point(x[9], y[9]), sca, thickness, lineType);//hip--lthigh
	line(img, Point(x[0], y[0]), Point(x[12], y[12]), sca, thickness, lineType);//hip--rthigh
	line(img, Point(x[2], y[2]), Point(x[3], y[3]), sca, thickness, lineType);//neck--lshldr
	line(img, Point(x[2], y[2]), Point(x[6], y[6]), sca, thickness, lineType);//neck--rshldr
	imshow("calibration", img);//�����������waitkey �����û���˳������������ջ���
}


