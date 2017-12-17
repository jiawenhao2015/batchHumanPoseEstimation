/*
1.����������������ʵ������
2.����ѵ���������ɲ��Խ����
3.kinect�Ľ��Ҳ�����
4.eval�Ľ����eval�Ǹ������� D��
*/

#include <iostream>
#include <iomanip>
#include <set>
#include "ml.h"
#include "highgui.h"
#include "ImageShow.h"
#include "Loading.h"
#include "opencv2/imgproc/imgproc.hpp" 
#include "common.h"


using namespace std;
using namespace cv;


#define HEIGHT 480
#define WIDTH  640

//----------------------------------------------------------------------------------------------------------------
#define  WITH_CONSTRAINT  0

#define		PROJECT_MAT		"E:/trainprocess/train/Amatrix.txt"		//ӳ�����·��
#define		JOINTS_NUM   15     //�ؽڵ�groundtruth����
#define		FEATURE_NUM  16    //���������

#define		START		0		//������ʼ֡����
#define		END 		150		//���Խ���֡����

int actionStart,actionEnd;
int frameStart, frameEnd;
int index;//��������ѵ������Եı��� index=5 ˵��ÿ5��ȡһ������

int KINECTHEIGHT = 480;
int KINECTWIDTH = 640;


//-------------------------------------------------------------------------------------------------------------------
// ��ȡ�ļ�
bool ReadFile(string filePath, vector<float>&errorVec)
{
	errorVec.clear();
	ifstream infile(filePath);
	float x;
	if (!infile.is_open())
	{
		cout << "���ܴ��ļ�----" << filePath << endl;
		return false;
	}
	while (!infile.eof())
	{
		infile >> x;
		errorVec.push_back(x);
	}
	errorVec.pop_back();
	return true;
}

//����һ��txt ����һ��m*n�ľ���
Mat InitMat(string matrixPath, int m, int n)
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

//���ɲ��Խ��   ���ÿһ֡�Ĺ��ƵĹؽڵ�λ�á�
set<pair<int, int>> wrongAction = { make_pair(2, 0), make_pair(2, 136), make_pair(7, 52),
									make_pair(7, 53), make_pair(7, 54) };

set<pair<int, int>>kinectWrong = { make_pair(1, 13), make_pair(3, 12), make_pair(3, 13), 
make_pair(3, 14), make_pair(3, 15), make_pair(3, 20), make_pair(3, 21),
make_pair(3, 22), make_pair(5, 8), make_pair(5, 38), make_pair(5, 41) };

void ComputeJoint(string matrixPath)
{
	Mat m = InitMat(matrixPath, FEATURE_NUM * 2, JOINTS_NUM * 2);//��ȡѵ���õ���ģ�;��� *2 ����Ϊ��2ά��

	stringstream ss;
	char  Path[128];
	for (int action = actionStart; action <= actionEnd; action++)
	{
		for (int people = 1; people <= 4;people++)
		{
			for (int frame = frameStart; frame <= frameEnd; frame++)
			{
				//if (wrongAction.count(make_pair(action, frame))) continue;
				if (frame % index != 0)continue;

				ss.str("");
				ss << "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action"<< action 
					<< "/people"<<people<<"/" << frame << "/clusterpoint.txt";
				sprintf_s(Path, "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action%d/people%d/%4d/clusterpoint.txt",action,people, frame);
				ifstream isexist(Path);
				if (!isexist.is_open())continue;//��ǰ������ 

				Mat featuresMat = InitMat(Path, 1, FEATURE_NUM * 2);

				Mat jointsMatrix(1, JOINTS_NUM * 2, CV_32F);//ӳ��õ��Ĺؽڵ�
				jointsMatrix = featuresMat * m;


				ss.str("");
				ss << "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action" << action
					<< "/people" << people << "/" << frame << "/guessPoints.txt";	

				sprintf_s(Path, "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action%d/people%d/%4d/guessPoints.txt", action, people, frame);
				//ifstream isexist(Path);

				ofstream of(Path);
				for (int i = 0; i < JOINTS_NUM; i++)//��������� ��������
				{
					of << jointsMatrix.at<float>(0, 2 * i) << " " << jointsMatrix.at<float>(0, 2 * i + 1) << endl;
				}
				of.close();
				cout << ss.str() << endl;
			}
		}		
	}
}

void drawJoints(Mat &photo, vector<int>X, vector<int>Y,int flag)
{	
	for (int i = 0; i < X.size(); i++)
	{
		stringstream ss;
		ss << i;
		if (flag == 1)//groundtruth
		{
			circle(photo, Point(X[i], Y[i]), 2, Scalar(0, 255,0), 1);
			putText(photo, ss.str(), Point(X[i], Y[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
		}
		else if (flag == 0)//���Խ�� ��groundtruthֻ����ɫ��һ��
		{
			circle(photo, Point(X[i], Y[i]), 2, Scalar(0, 255, 0), 5);
			putText(photo, ss.str(), Point(X[i], Y[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
		}
	}
}
//eval���ݼ�groundtruth
bool readGroundTruth(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	int temp, temp2;
	float tx,ty,tz;
	
	if (!infile.is_open())
	{
		cout << "���ܴ��ļ���" << endl;
		return false;
	}		
	while (!infile.eof())
	{
		infile >> temp >> temp2 >> tx >> ty >> tz;
		xVec.push_back(temp);
		temp2 = HEIGHT - temp2;
		yVec.push_back(temp2);
	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	return true;
}

//��ʵ���� kinect�����Ľ��
bool kinectResults(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	float temp, temp2;
	float tx, ty, tz;

	if (!infile.is_open())
	{
		cout << "���ܴ��ļ���" << endl;
		return false;
	}
	while (!infile.eof())
	{
		infile >> temp >> temp2 >> tx >> ty >> tz>>tx>>tx>>tx>>tx;
		xVec.push_back(temp);		
		yVec.push_back(temp2);
		cout << temp <<" "<< temp2 << endl;
	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	return true;
}
//�������� groundtruth
bool readXuNiGroundTruth(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	string jointName;
	int x, y, z;

	if (!infile.is_open())
	{
		cout << "��������groundtruth ���ܴ򿪣�" <<filePath << endl;
		return false;
	}
	while (!infile.eof())
	{
		infile >> x >> y >> z >> jointName;
		xVec.push_back(x);
		yVec.push_back(y);
		 
	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	return true;
}
//�Լ��ĵ�kinect �궨��groundtruth
bool readKinectGroundTruth(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	string jointName;
	int x, y;

	if (!infile.is_open())
	{
		cout << "��������groundtruth ���ܴ򿪣�" << filePath << endl;
		return false;
	}
	while (!infile.eof())
	{
		infile >> x >> y >> jointName;
		xVec.push_back(x);
		yVec.push_back(y);

	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	return true;
}
//��ȡ �������ɵĹؽڵ��ļ�
bool readGuessFile(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	float temp, temp2;
	 

	if (!infile.is_open())
	{
		cout << "���ܴ��ļ���" << endl;
		return false;
	}
	while (!infile.eof())
	{
		infile >> temp >> temp2 ;
		xVec.push_back(temp);//Ϊ�˱�֤��������int a = b+0.5;		
		yVec.push_back(temp2);
	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	return true;
}
//��ʾeval���ݼ� �Լ�groundtruth
int showEval()
{
	
	Mat depthData(HEIGHT, WIDTH, CV_64FC3);
	Mat photo(HEIGHT, WIDTH, CV_8UC3);
	
	Mat tmp;
	Mat tmp2;
	ImageShow ShowTool;
	vector<Data> src; 
	Loading Load;

	stringstream ss;
	vector<int>X, Y;
	for (int i = 0; i <= 7;i++)
	{
		for (int j = 0; j <= 500;j++)
		{
			src.clear();
			X.clear();
			Y.clear();
			ss.str("");

			ss << "D:/EVAL20170704/EVAL/joints/joint" << i << "_" << j << ".txt";
			if(!readGroundTruth(ss.str(),X,Y))continue;
			ss.str("");
			ss << "D:/EVAL20170704/EVAL/depth/depth" << i << "_" << j << ".txt";
			
			Load.DownLoad_Info(ss.str(), src, 1);
			photo = ShowTool.getPhoto(src);
			drawJoints(photo,X,Y,1);
			
			//pyrUp(photo, tmp, Size(photo.cols * 2, photo.rows * 2));
			//pyrDown(tmp, tmp2, Size(tmp.cols / 2, tmp.rows / 2));
			
			imshow("eval", photo);
			waitKey(1);	

			
			stringstream spath;
			spath << "D:/EVAL20170704/EVAL/depth/"<< i << "/" << j << ".jpg";
			cout << spath.str() << endl;
			imwrite(spath.str(), photo);
			//imshow("pyrup", tmp);
			//waitKey(1);
			//imshow("pyrdown", tmp2);
			//waitKey(1);

		}
	}
		
	getchar();
	return 0;
}


//��������λ�ã���������������λ�ã�������� ������ XY��groundtruth myXmyY�ǲ��Խ��
void ComputeRealError(vector<Data>& src, vector<int>&X, vector<int>&Y, vector<int>&myX, vector<int>&myY, string path,Mat& photo)
{
	Loading Load;
	int n = X.size();
	ofstream of(path);
	pair<int, int>pa;
	
	for (int i = 0; i < n; i++)
	{
		int index1 = Load.Search(X[i],Y[i], src);
		int index2 = Load.Search(myX[i],myY[i], src);
		//ӳ���ȥ�Ĺؽڵ���ܲ����������ϣ�û�ڵ������棬������ĵ�����ĵ�
		int cnt = 0;//��¼���Ҵ��� ����ҳ���10��û�ҵ� ������ return
		while (index1 == -1)
		{
			pa = Load.SearchInNeighbor(X[i], Y[i], photo);
			index1 = Load.Search(pa.first, pa.second, src);
		}
		while (index2 == -1)
		{
			if (cnt == 10)//�Ҳ�����
			{
				break;
			}
			cnt++;
			pa = Load.SearchInNeighbor(myX[i], myY[i], photo);
			cout << myX[i] << " " << myY[i] << " " << pa.first << " " << pa.second << endl;
			index2 = Load.Search(pa.first, pa.second, src);			
		}
		float dis;
		if (cnt ==10)
		{
			dis = pow((src[index1].x - myX[i]), 2) + pow((src[index1].y - myY[i]), 2);
		}
		else
		{
			dis = pow((src[index1].x - src[index2].x), 2) + pow((src[index1].y - src[index2].y), 2);
		} 

		of << sqrt(dis) << " ";
		cout << sqrt(dis) << " ";
	}
	cout << endl;
	of.close();
}
//��������λ�ã���������������λ�ã�������� ������ XY��groundtruth myXmyY�ǲ��Խ��
void ComputeKinectRealError(vector<Data>& src, vector<int>&X, vector<int>&Y, vector<int>&myX, vector<int>&myY, string path, Mat& photo)
{
	Loading Load;
	int n = X.size();
	ofstream of(path);
	pair<int, int>pa;

	for (int i = 0; i < n; i++)
	{
		int index1 = Load.Search(X[i], Y[i], src);
		int index2 = Load.Search(myX[i], myY[i], src);
		//ӳ���ȥ�Ĺؽڵ���ܲ����������ϣ�û�ڵ������棬������ĵ�����ĵ�

		int cnt = 0;//��¼���Ҵ��� ����ҳ���10��û�ҵ� ������ return

		while (index1 == -1)
		{
			pa = Load.SearchInNeighbor(X[i], Y[i], photo);
			index1 = Load.Search(pa.first, pa.second, src);
		}
		while (index2 == -1)
		{
			if (cnt == 10)//�Ҳ�����
			{
				break;
			}
			cnt++;
			pa = Load.SearchInNeighbor(myX[i], myY[i], photo);
			cout << myX[i] << " " << myY[i] << " " << pa.first << " " << pa.second << endl;
			index2 = Load.Search(pa.first, pa.second, src);
		}
		float dis;
		if (cnt == 10)
		{
			 dis = pow((src[index1].x - myX[i]), 2) + pow((src[index1].y - myY[i]), 2);
		}
		else
		{
			 dis = pow((src[index1].x - src[index2].x), 2) + pow((src[index1].y - src[index2].y), 2);
		}
		of << sqrt(dis) << " ";
		cout << sqrt(dis) << " ";
	}
	cout << endl;
	of.close();
}

void ComputeError(vector<Data>& src, vector<int>&X, vector<int>&Y, vector<int>&myX, vector<int>&myY, string path, Mat& photo)
{
	Loading Load;
	int n = X.size();
	ofstream of(path);
	pair<int, int>pa;
	float dis;
	for (int i = 0; i < n; i++)
	{		
		dis = pow((X[i] - myX[i]), 2) + pow((Y[i] - myY[i]), 2);
		
		of << sqrt(dis) << " ";
		cout << sqrt(dis) << " ";
	}
	cout << endl;
	of.close();
}


//�������ɲ��Խ�� �����Խ�� ����  ��ͳ����� bool flag �Ƿ���ʾͼƬ
void saveTestResults(bool flag)
{
	Mat depthData(HEIGHT, WIDTH, CV_64FC3);
	Mat photo(HEIGHT, WIDTH, CV_8UC3);

	Mat temp(HEIGHT, WIDTH, CV_8UC3);
	
	ImageShow ShowTool;
	vector<Data> src;
	Loading Load;

	stringstream ssgroundtruth;
	stringstream sstest;
	stringstream sspointcloud;
	stringstream sserror;
	vector<int>X, Y;//groundtruth
	vector<int>myX, myY;//���Խ��
	char Path[128];
	for (int action = actionStart; action <= actionEnd; action++)
	{
		for (int people = 1; people <= 4; people++)//
		{
			for (int frame = frameStart; frame <= frameEnd; frame++)
			{
				//if (wrongAction.count(make_pair(action, frame))) continue;
				if (frame % index != 0)continue;

				cout << "-------------------" << action << "---------------" <<  people << "---------------" << frame << "----------------" << endl;
				src.clear();
				X.clear();
				Y.clear();
				myX.clear();
				myY.clear();
				ssgroundtruth.str("");
				sstest.str("");
				sspointcloud.str("");
				sserror.str("");

				string groundTruthPathPrefix("E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/");
				string featurePathPrefix("E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/");

				ssgroundtruth << groundTruthPathPrefix <<"action"<< action << "/people" <<people<<"/groundTruth"<<frame <<".txt";
				if (!readXuNiGroundTruth(ssgroundtruth.str(), X, Y))continue;

				sstest << featurePathPrefix << "action" << action << "/people" << people << "/" <<frame <<"/guessPoints.txt";
				sprintf_s(Path, "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action%d/people%d/%4d/guessPoints.txt", action, people, frame);
				if (!readGuessFile(Path, myX, myY))continue;

				sspointcloud << "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action" << action << "/people" <<people<<"/output"<<frame <<".txt";
				
				cout << sspointcloud.str() << endl;
				Load.DownLoad_XuNiData(sspointcloud.str(), src, 1);

				photo = ShowTool.getPhoto(src);
				temp = ShowTool.getPhoto(src);//����һ��ԭʼ����  ������temp=photo ��Ϊ�����ã��൱�ڱ���

				if (flag)
				{
					drawJoints(photo, X, Y, 1);//groundtruth
					drawJoints(photo, myX, myY, 0);//���Խ��
					imshow("img", photo);
					waitKey(1);
				}


				//���������� xiangsu������ ���浽����
				sserror << featurePathPrefix << "action" << action << "/people" << people << "/frame" << frame << "/abserror.txt";
				sprintf_s(Path, "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action%d/people%d/%4d/abserror.txt", action, people, frame);
				ComputeRealError(src, X, Y, myX, myY, Path, temp);
			}
		}
	}	
}


void saveKinectTestResults(bool flag)
{
	Mat depthData(HEIGHT, WIDTH, CV_64FC3);
	Mat photo(HEIGHT, WIDTH, CV_8UC3);

	Mat temp(HEIGHT, WIDTH, CV_8UC3);

	ImageShow ShowTool;
	vector<Data> src;
	Loading Load;

	stringstream ssgroundtruth;
	stringstream sstest;
	stringstream sspointcloud;
	stringstream sserror;
	vector<int>X, Y;//groundtruth
	vector<int>myX, myY;//���Խ��

	for (int model = 1; model <= 6; model++)
	{
		for (int action = actionStart; action <= actionEnd; action++)
		{
			for (int frame = frameStart; frame <= frameEnd; frame++)
			{
				if (model == 3 || model == 5)continue;
				if (kinectWrong.count(make_pair(model, frame))) continue;
				if (frame % index != 0)continue;
				cout << "-------------------" << model << "-----------------" << frame << "----------------" << endl;
				src.clear();
				X.clear();
				Y.clear();
				myX.clear();
				myY.clear();
				ssgroundtruth.str("");
				sstest.str("");
				sspointcloud.str("");
				sserror.str("");

				string groundTruthPathPrefix("E:/laboratory/dataset/realdataresults/");
				string featurePathPrefix("E:/laboratory/dataset/realdataresults/");

				ssgroundtruth << groundTruthPathPrefix << "model" << model << "/action" << action << "/" << frame << "/guessJointsNew.txt";
				if (!readKinectGroundTruth(ssgroundtruth.str(), X, Y))continue;

				sstest << featurePathPrefix << "model" << model << "/action" << action << "/" << frame << "/shixiongguessPoints.txt";
			
				if (!readGuessFile(sstest.str(), myX, myY))continue;
				char inputPath[128];
				
				sprintf_s(inputPath, "E:/laboratory/dataset/realdata/model%d/action%d/%04dpointscloud.txt", model, action, frame);

			//	sspointcloud << "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action" << action << "/people" << people << "/output" << frame << ".txt";

				cout << inputPath << endl;
				Load.DownLoad_XuNiData(inputPath, src, 1);//xuni��kinect��ʽһ��

				photo = ShowTool.getPhoto(src);
				temp = ShowTool.getPhoto(src);//����һ��ԭʼ����  ������temp=photo ��Ϊ�����ã��൱�ڱ���

				if (flag)
				{
					drawJoints(photo, X, Y, 1);//groundtruth
					drawJoints(photo, myX, myY, 0);//���Խ��
					imshow("img", photo);
					waitKey(1);
				}
				//���������� xiangsu������ ���浽����
				sserror << featurePathPrefix << "model" << model << "/action" << action << "/" << frame << "/abserror.txt";
				ComputeKinectRealError(src, X, Y, myX, myY, sserror.str(), temp);
			}
		}
	}
}



//euclid error  ÿһ֡������Ѿ������  ���ڽ����ǻ��ܵ�һ�� ��ƽ��
//ÿһ��actionΪһ����λ
void computeFinalError(int action)
{
	vector<vector<float>>error;//����֡���
	vector<float>frameError;//��֡���

	char path[128];
	sprintf_s(path, "E:/trainprocess/action%d_euclid.txt",action);
	ofstream euclidErrorFile(path);//���в���֡�������Ϣ

	sprintf_s(path,"E:/trainprocess/action%d_euclidfinal.txt", action);
	ofstream finalEuclidErrorFile(path);//����֡ƽ�����

	sprintf_s(path,"E:/trainprocess/action%d_percentage.txt", action);
	ofstream percentage(path);

	string euclidErrorPrefix("D:/EVAL20170704/EVAL/depth/");
	 
	
	
	for (int frame = START; frame <= END; frame++)//��ȡ��vector
	{
		if (wrongAction.count(make_pair(action, frame))) continue;
		if (frame % 10 != 0)continue;

		stringstream ss;
		ss << euclidErrorPrefix << action << "/" << frame << "/abserror.txt";
		ifstream isexist(ss.str());
		if (!isexist.is_open())continue;//��ǰ������ 
		if (!ReadFile(ss.str(), frameError))continue;

		error.push_back(frameError);			
	}	
	for (int i = 0; i < error.size();i++)//��vector������ļ�
	{
		for (int j = 0; j < error[i].size();j++)
		{
			euclidErrorFile << error[i][j] << " ";
		}
		euclidErrorFile << endl;
	}
	euclidErrorFile.close();
	for (int i = 0; i < error[0].size();i++)//���з���
	{
		float err = 0;
		int cnt = 0;//ͳ�����С��10cm�� �ٷֱ�
		for (int j = 0; j < error.size(); j++)
		{
			//if (error[j].size()!=12)cout << j<<" "<<error[j].size() << endl;
			err += error[j][i];
			if (error[j][i] <= 0.1) cnt++;
		}
		
		finalEuclidErrorFile << err << " ";
		percentage << cnt / (float)error.size() << " " << endl;
		cout << err << " ";
	}
	finalEuclidErrorFile.close();
	percentage.close();	
}

//�� ���е��������ͳ�Ƶ�һ��
void computAllFinalError(string matrixName)
{
	vector<vector<float>>error;//����֡���
	vector<float>frameError;//��֡���

	char path[128];
	sprintf_s(path, "E:/trainprocess/test/xunishujushixiong/%s_actionALLeuclid.txt",matrixName.c_str());
	ofstream euclidErrorFile(path);//���в���֡�������Ϣ
	
	sprintf_s(path, "E:/trainprocess/test/xunishujushixiong/%s_actionALLeuclidfinal.txt", matrixName.c_str());
	ofstream finalEuclidErrorFile(path);//����֡ƽ�����

	
	
	
	string featurePathPrefix("E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/");


	for (int action = actionStart; action <= actionEnd; action++)
	{
		for (int people = 1; people <= 4; people++)//
		{
			for (int frame = frameStart; frame <= frameEnd; frame++)
			{
				if (frame % index != 0)continue;

				stringstream ss;
				ss << featurePathPrefix << "action" << action << "/people" << people << "/" << frame << "/abserror.txt";
				sprintf_s(path, "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action%d/people%d/%4d/abserror.txt", action, people, frame);

				ifstream isexist(path);
				if (!isexist.is_open())continue;//��ǰ������ 
				if (!ReadFile(path, frameError))continue;

				error.push_back(frameError);
			}
		}
	}
	

	for (int i = 0; i < error.size(); i++)//��vector������ļ�
	{
		for (int j = 0; j < error[i].size(); j++)
		{
			euclidErrorFile << error[i][j] << " ";
		}
		euclidErrorFile << endl;
	}
	euclidErrorFile.close();

	vector<float>meanErr;//����������һ֡�����йؽڵ�����  �� ƽ����� �Լ���������

	for (int i = 0; i < error[0].size(); i++)//���з���
	{
		float err = 0;
		int cnt = 0;//ͳ�����С��10cm�� �ٷֱ�
		for (int j = 0; j < error.size(); j++)
		{	
			err += error[j][i];
			if (error[j][i] <= 0.1) cnt++;
		}
		float t = err / error.size();
		finalEuclidErrorFile << t << " ";
		
		meanErr.push_back(t);
	}
	float temp = 0;
	for (int i = 0; i < meanErr.size();i++)
	{
		temp += meanErr[i];
	}
	finalEuclidErrorFile << temp << " "<<temp/meanErr.size();
	finalEuclidErrorFile.close();
}


void computAllKinectFinalError(string matrixName)
{
	vector<vector<float>>error;//����֡���
	vector<float>frameError;//��֡���

	char path[128];
	sprintf_s(path, "E:/trainprocess/test/kinectshixiong/%s_actionALLeuclid.txt", matrixName.c_str());
	ofstream euclidErrorFile(path);//���в���֡�������Ϣ

	sprintf_s(path, "E:/trainprocess/test/kinectshixiong/%s_actionALLeuclidfinal.txt", matrixName.c_str());
	ofstream finalEuclidErrorFile(path);//����֡ƽ�����
	
	string featurePathPrefix("E:/laboratory/dataset/realdataresults/");
	
	for (int model = 1; model <= 6; model++)
	{
		for (int action = actionStart; action <= actionEnd; action++)
		{
			for (int frame = frameStart; frame <= frameEnd; frame++)
			{
				if (model == 3 || model == 5)continue;
				if (kinectWrong.count(make_pair(model, frame))) continue;
				if (frame % index != 0)continue;
				stringstream ss;
				ss << featurePathPrefix << "model" << model << "/action" << action << "/" << frame << "/abserror.txt";
				ifstream isexist(ss.str());
				if (!isexist.is_open())continue;//��ǰ������ 
				if (!ReadFile(ss.str(), frameError))continue;

				error.push_back(frameError);
			}
		}
	}

	for (int i = 0; i < error.size(); i++)//��vector������ļ�
	{
		for (int j = 0; j < error[i].size(); j++)
		{
			euclidErrorFile << error[i][j] << " ";
		}
		euclidErrorFile << endl;
	}
	euclidErrorFile.close();

	vector<float>meanErr;//����������һ֡�����йؽڵ�����  �� ƽ����� �Լ���������

	for (int i = 0; i < error[0].size(); i++)//���з���
	{
		float err = 0;
		int cnt = 0;//ͳ�����С��10cm�� �ٷֱ�
		for (int j = 0; j < error.size(); j++)
		{
			err += error[j][i];
			if (error[j][i] <= 0.1) cnt++;
		}
		float t = err / error.size();
		finalEuclidErrorFile << t << " ";

		meanErr.push_back(t);
	}
	float temp = 0;
	for (int i = 0; i < meanErr.size(); i++)
	{
		temp += meanErr[i];
	}
	finalEuclidErrorFile << temp << " " << temp / meanErr.size();
	finalEuclidErrorFile.close();
}

//���ھ������ֲ������� ֱ�Ӷ�ȡ�ļ����¾����ļ��� �浽vector��  
void checkMatrix(vector<string> & matname)
{
	 ////��list�ļ� ������
	ifstream infile("E:\\trainprocess\\train\\xunishujushixiong\\matrix\\LIST.TXT");
	vector<string>matrxPath;
	if (!infile.is_open())
	{
		cout << "���ܴ��ļ�----checkMatrix"  << endl;
		return;
	}
	string t;
	while (!infile.eof())
	{
		infile >> t;
		matrxPath.push_back(t);
	}
	matrxPath.pop_back(), matrxPath.pop_back(), matrxPath.pop_back();//��2��û����
	//////////---------------------------

	int setsize = 0;
	set<vector<float>> allmat;
	
	for (int i = 0; i < matrxPath.size();i++)
	{
		vector<float >  temp;
		if (!ReadFile("E:\\trainprocess\\train\\xunishujushixiong\\matrix\\"+matrxPath[i], temp))continue;	

		allmat.insert(temp);
		int nowsize = allmat.size();
		if (nowsize != setsize)
		{
			setsize = nowsize;
			matname.push_back(matrxPath[i]);
		}
		allmat.insert(temp);
	}
	cout <<"***********"<< allmat.size() << endl;	
}
void checkKinectMatrix(vector<string> & matname,string path)
{
	////��list�ļ� ������
	ifstream infile(path + "LIST.TXT");
	vector<string>matrxPath;
	if (!infile.is_open())
	{
		cout << "���ܴ��ļ�----checkMatrix" << endl;
		return;
	}
	string t;
	while (!infile.eof())
	{
		infile >> t;
		matrxPath.push_back(t);
	}
	matrxPath.pop_back(), matrxPath.pop_back(), matrxPath.pop_back();//��2��û����
	//////////---------------------------

	int setsize = 0;
	set<vector<float>> allmat;

	for (int i = 0; i < matrxPath.size(); i++)
	{
		vector<float >  temp;
		if (!ReadFile(path + matrxPath[i], temp))continue;

		allmat.insert(temp);
		int nowsize = allmat.size();
		if (nowsize != setsize)
		{
			setsize = nowsize;
			matname.push_back(matrxPath[i]);
		}
		allmat.insert(temp);
	}
	cout << "***********" << allmat.size() << endl;


}
int xunishuju()
{	
	vector<string>matnames;	 
	checkMatrix(matnames);

	actionStart = 4, actionEnd = 4;
	frameStart = 0, frameEnd = 49;
	index = 5;
	for (string name : matnames)
	{
		string t = "E:\\trainprocess\\train\\xunishujushixiong\\matrix\\" + name;
		ComputeJoint(t);//1.���ȼ�����Թؽڵ�λ��
		saveTestResults(false);//2.ͳ��ÿһ֡���Թؽڵ� ���	
		computAllFinalError(name);//3�����������
	}
	system("PAUSE");
	return 0;
}
/*
20171105
�Լ��ɼ��� kinect���� ͳ�ƽ��
*/

void ComputeKinectJoint(string matrixPath)
{

	Mat m = InitMat(matrixPath, FEATURE_NUM * 2, JOINTS_NUM * 2);//��ȡѵ���õ���ģ�;��� *2 ����Ϊ��2ά��

	stringstream ss;

	for (int model = 1; model <= 6; model++)
	{
		for (int action = actionStart; action <= actionEnd; action++)
		{
			for (int frame = frameStart; frame <= frameEnd; frame++)
			{				
				if (model == 3 || model == 5)continue;
				if (kinectWrong.count(make_pair(model, frame))) continue;
				if (frame % index != 0)continue;

				ss.str("");
				ss << "E:/laboratory/dataset/realdataresults/model" << model
					<< "/action" << action << "/" << frame << "/clusterpoint.txt";

				ifstream isexist(ss.str());
				if (!isexist.is_open())continue;//��ǰ������ 

				Mat featuresMat = InitMat(ss.str(), 1, FEATURE_NUM * 2);

				Mat jointsMatrix(1, JOINTS_NUM * 2, CV_32F);//ӳ��õ��Ĺؽڵ�
				jointsMatrix = featuresMat * m;


				ss.str("");
				ss << "E:/laboratory/dataset/realdataresults/model" << model
					<< "/action" << action << "/" << frame <<"/shixiongguessPoints.txt";
				ofstream of(ss.str());
				for (int i = 0; i < JOINTS_NUM; i++)//��������� ��������
				{
					of << jointsMatrix.at<float>(0, 2 * i) << " " << jointsMatrix.at<float>(0, 2 * i + 1) << endl;
				}
				of.close();
				cout << ss.str() << endl;
			}
		}
	}
}
int kinect()
{
	vector<string>matnames;	
	checkKinectMatrix(matnames,"E:\\trainprocess\\train\\kinectshixiong\\matrix\\");

	actionStart = 1, actionEnd = 1;
	frameStart = 0, frameEnd = 100;
	index = 5;
	for (string name : matnames)
	{
		string t = "E:\\trainprocess\\train\\kinectshixiong\\matrix\\" + name;
		ComputeKinectJoint(t);//1.���ȼ�����Թؽڵ�λ��
		saveKinectTestResults(false);//2.ͳ��ÿһ֡���Թؽڵ� ���	1 13
		computAllKinectFinalError(name);//3�����������
	}
	system("PAUSE");

	return 0;
}


/*20171111
�ֹ��궨��grountruthû�йؽڵ����� ���ڼ���*/
void addJointName(string path, string newpath)
{
	ifstream featuresFile(path);
	int x, y;
	vector<int>xVec;//���������������x����
	vector<int>yVec;
	if (featuresFile.is_open())
	{
		while (!featuresFile.eof())
		{
			featuresFile >> x >> y;
			xVec.push_back(x);
			yVec.push_back(y);
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << path.c_str() << endl;
	}

	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);

	ofstream of(newpath);
	for (int i = 0; i < xVec.size(); i++)
	{
		of << xVec[i] << " " << yVec[i] << " " << JOINTS[i] << endl;
	}
	of.close();
}
//20171109
//just��ʾkinetic�����Ľ��
void showKinectResult()
{

	Mat depthData(KINECTHEIGHT, KINECTWIDTH, CV_64FC3);
	Mat photo(KINECTHEIGHT, KINECTWIDTH, CV_8UC3);

	Mat tmp;
	Mat tmp2;
	ImageShow ShowTool;
	vector<Data> src;
	Loading Load;

	stringstream ss;
	vector<int>X, Y;
	for (int i = 0; i <= 0; i++)
	{
		for (int j = 0; j <= 99; j++)
		{
			src.clear();
			X.clear();
			Y.clear();
			ss.str("");

			//ss << "D:/KinectOneRecord/KinectOneRecord/New folder/dxh/1/body3DCoordinates" << i << "_" << setw(4) << setfill('0') << j << ".txt";
			//ss << "E:/laboratory/RecordProcess/RecordProcess/output/model1/action1/" << setw(4) << setfill('0') << j << "biaodingjoints.txt";
			ss << "E:/laboratory/RecordProcess/RecordProcess/output/model1/action1/frame" << j << "/guessPoints.txt";
	
			cout << ss.str() << endl;			
			if (!readGuessFile(ss.str(), X, Y))continue;
			ss.str("");
			ss << "E:/laboratory/RecordProcess/RecordProcess/output/model1/action1/" << setw(4) << setfill('0')<< j << "pointscloud.txt";
			cout << ss.str() << endl;
			
			Load.DownLoad_XuNiData(ss.str(), src, 1);
			photo = ShowTool.getPhoto(src);
			drawJoints(photo, X, Y, 0);

			//draw line
			//for (int j = 0; j < Skeleton_line_Num; j++)
			//{
			//	Point a(X[skeleton_line[2 * j]], Y[skeleton_line[2 * j]]);
			//	Point b(X[skeleton_line[2 * j + 1]], Y[skeleton_line[2 * j + 1]]);
			//	line(photo, a, b, Scalar(0, 0, 255), 3);
			//}

		/*	for (int k = 0; k < BIAODING_line_Num; k++)
			{
				Point a(X[biaoding_line[2 * k]], Y[biaoding_line[2 * k]]);
				Point b(X[biaoding_line[2 * k + 1]], Y[biaoding_line[2 * k + 1]]);
				line(photo, a, b, Scalar(255, 0, 255), 2);
			}*/

			imshow("kinect", photo);
			waitKey(1);

			stringstream spath;
			spath << "D:/KinectOneRecord/KinectOneRecord/record/ceshi" << i << "_" << setw(4) << setfill('0') << j << ".jpg";
			cout << spath.str() << endl;
			imwrite(spath.str(), photo);
		}
	}

	getchar();
}

//20171112
//�Լ��ɼ��� kinect���� ͳ�ƽ��
void ComputeKinectTestJoint(string matrixPath)
{

	Mat m = InitMat(matrixPath, FEATURE_NUM * 2, JOINTS_NUM * 2);//��ȡѵ���õ���ģ�;��� *2 ����Ϊ��2ά��

	stringstream ss;

	for (int model = 1; model <= 1; model++)
	{
		for (int action = actionStart; action <= actionEnd; action++)
		{
			for (int frame = frameStart; frame <= frameEnd; frame++)
			{
				/*if (model == 3 || model == 5)continue;
				if (kinectWrong.count(make_pair(model, frame))) continue;*/

				ss.str("");
				ss << "E:/laboratory/RecordProcess/RecordProcess/output/model" << model
					<< "/action" << action << "/frame" << frame << "/featurePoints.txt";

				ifstream isexist(ss.str());
				if (!isexist.is_open())continue;//��ǰ������ 

				Mat featuresMat = InitMat(ss.str(), 1, FEATURE_NUM * 2);

				Mat jointsMatrix(1, JOINTS_NUM * 2, CV_32F);//ӳ��õ��Ĺؽڵ�
				jointsMatrix = featuresMat * m;


				ss.str("");
				ss << "E:/laboratory/RecordProcess/RecordProcess/output/model" << model
					<< "/action" << action << "/frame" << frame << "/guessPoints.txt";
				ofstream of(ss.str());
				for (int i = 0; i < JOINTS_NUM; i++)//��������� ��������
				{
					of << jointsMatrix.at<float>(0, 2 * i) << " " << jointsMatrix.at<float>(0, 2 * i + 1) << endl;
				}
				of.close();
				cout << ss.str() << endl;
			}
		}
	}
}
//20171112
//�Լ��궨��100֡kinect���� ��ʱ��֤��� ֻ��Ҫ�����������
void testKinectData()
{
	vector<string>matnames;
	checkKinectMatrix(matnames, "E:\\trainprocess\\train\\newkinect\\matrix\\");

	actionStart = 1, actionEnd = 1;
	frameStart = 0, frameEnd = 99;
	index = 5;
	for (string name : matnames)
	{
		string t = "E:\\trainprocess\\train\\newkinect\\matrix\\" + name;
		ComputeKinectTestJoint(t);//1.���ȼ�����Թؽڵ�λ��
		
	}
	system("PAUSE");	 
}


//��ʾͼƬ ���ؽڵ�  ���� 
void temp()
{
	Mat depthData(KINECTHEIGHT, KINECTWIDTH, CV_64FC3);
	Mat photo(KINECTHEIGHT, KINECTWIDTH, CV_8UC3);

	Mat tmp;
	Mat tmp2;
	ImageShow ShowTool;
	vector<Data> src;
	Loading Load;

	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";

	stringstream ss;
	vector<int>X, Y;
	for (int action = 4; action <= 4; action++)
	{
		for (int people = 1; people <= 1; people++)
		{
			for (int index = 0; index <= 0; index++)
			{
				src.clear();
				X.clear();
				Y.clear();
				ss.str("");
				ss << prefix << "\\action" << action << "\\people" << people << "\\groundTruth" << index << ".txt";

				cout << ss.str() << endl;
				if (!readXuNiGroundTruth(ss.str(), X, Y))continue;

				ss.str("");
				ss << prefix << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
				Load.DownLoad_XuNiData(ss.str(), src, 1);
				photo = ShowTool.getPhoto(src);
				drawJoints(photo, X, Y, 0);
				
				ss.str("");
				ss << index;
				putText(photo, ss.str(), Point(60, 60), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255));
				/*
				for (int k = 0; k < 6; k++)
				{
				Point a(X[xuni_line[2 * k]], Y[xuni_line[2 * k]]);
				Point b(X[xuni_line[2 * k + 1]], Y[xuni_line[2 * k + 1]]);
				line(photo, a, b, Scalar(0, 0, 255), 4);
				}
				for (int k = 6; k < 20; k++)
				{
				Point a(X[xuni_line[2 * k]], Y[xuni_line[2 * k]]);
				Point b(X[xuni_line[2 * k + 1]], Y[xuni_line[2 * k + 1]]);
				line(photo, a, b, Scalar(0, 255, 255), 2);
				}*/

				imshow("kinect", photo);
				waitKey(1);

				stringstream spath;
				spath << prefix << "\\action" << action << "\\people" << people << "\\" << index << ".jpg";
				cout << spath.str() << endl;
				imwrite(spath.str(), photo);
			}
		}
	}	
}
//��ʾ3������㲢����
void temp3()
{
	Mat depthData(KINECTHEIGHT, KINECTWIDTH, CV_64FC3);
	Mat photo(KINECTHEIGHT, KINECTWIDTH, CV_8UC3);

	Mat tmp;
	Mat tmp2;
	ImageShow ShowTool;
	vector<Data> src;
	Loading Load;

	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";
	string prefix2 = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";

	stringstream ss;
	vector<int>X, Y;
	for (int action = 7; action <= 7; action++)
	{
		for (int people = 1; people <= 1; people++)
		{
			for (int index = 0; index <= 299; index++)
			{
				src.clear();
				X.clear();
				Y.clear();
				ss.str("");
				ss << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";

				cout << ss.str() << endl;
				if (!readGuessFile(ss.str(), X, Y))continue;

				ss.str("");
				ss << prefix2 << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
				Load.DownLoad_XuNiData(ss.str(), src, 1);
				photo = ShowTool.getPhoto(src);
				drawJoints(photo, X, Y, 0);


				/*for (int k = 0; k < 5; k++)
				{
				Point a(X[julei_line[2 * k]], Y[julei_line[2 * k]]);
				Point b(X[julei_line[2 * k + 1]], Y[julei_line[2 * k + 1]]);
				line(photo, a, b, Scalar(0, 0, 255), 4);
				}
				for (int k = 5; k < 22; k++)
				{
				Point a(X[julei_line[2 * k]], Y[julei_line[2 * k]]);
				Point b(X[julei_line[2 * k + 1]], Y[julei_line[2 * k + 1]]);
				line(photo, a, b, Scalar(0, 255, 255), 2);
				}*/

				imshow("kinect", photo);
				waitKey(1);

				stringstream spath;
				spath << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\001.jpg";

				cout << spath.str() << endl;
				imwrite(spath.str(), photo);
			}
		}
	}

	getchar();
}
//��ʾ4������㲢����
void temp4()
{
	Mat depthData(KINECTHEIGHT, KINECTWIDTH, CV_64FC3);
	Mat photo(KINECTHEIGHT, KINECTWIDTH, CV_8UC3);

	Mat tmp;
	Mat tmp2;
	ImageShow ShowTool;
	vector<Data> src;
	Loading Load;

	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";
	string prefix2 = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";

	stringstream ss;
	vector<int>X, Y;
	for (int action = 7; action <= 7; action++)
	{
		for (int people = 1; people <= 1; people++)
		{
			for (int index = 79; index <= 79; index++)
			{
				src.clear();
				X.clear();
				Y.clear();
				ss.str("");
				ss << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePoints.txt";

				cout << ss.str() << endl;
				if (!readGuessFile(ss.str(), X, Y))continue;

				ss.str("");
				ss << prefix2 << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
				Load.DownLoad_XuNiData(ss.str(), src, 1);
				photo = ShowTool.getPhoto(src);
				drawJoints(photo, X, Y, 0);


				for (int k = 0; k < 5; k++)
				{
					Point a(X[julei4_line[2 * k]], Y[julei4_line[2 * k]]);
					Point b(X[julei4_line[2 * k + 1]], Y[julei4_line[2 * k + 1]]);
					line(photo, a, b, Scalar(0, 0, 255), 4);
				}
				for (int k = 5; k < 27; k++)
				{
					Point a(X[julei4_line[2 * k]], Y[julei4_line[2 * k]]);
					Point b(X[julei4_line[2 * k + 1]], Y[julei4_line[2 * k + 1]]);
					line(photo, a, b, Scalar(0, 255, 255), 2);
				}

				imshow("kinect", photo);
				waitKey(1);

				stringstream spath;
				spath << "C:\\Users\\jwh\\Desktop\\00000.jpg";
				cout << spath.str() << endl;
				imwrite(spath.str(), photo);
			}
		}
	}

	getchar();
}

// �ϲ�����ͼ�� ��ʾ���
//20171205
void mymerge()
{
	string  path = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion\\action7\\people1\\";
	
	vector<Mat>input(20);
	for (int i = 0; i < 20;i++)
	{
		stringstream ss;
		ss << path << i << ".jpg";
		input[i] = imread(ss.str());		
	}

	Size bigsize(input[0].cols * 10, input[0].rows*2);//�ϲ���ͼƬsize ��*��
		
	vector<Mat>temp(20);

	Mat mergefinal;
	mergefinal.create(bigsize, CV_MAKETYPE(input[0].depth(), 3));//rgb 3ͨ��
	mergefinal = Scalar::all(0);

	for (int i = 0; i < 20;i++)
	{
		if (i<10)temp[i] = mergefinal(Rect(i*input[0].cols, 0, input[0].cols, input[0].rows));
		else temp[i] = mergefinal(Rect((i - 10)*input[0].cols, input[0].rows, input[0].cols, input[0].rows));
		input[i].copyTo(temp[i]); //copyͼƬ����Ӧλ��
	}

	imshow("merge", mergefinal);
	imwrite("merge.jpg", mergefinal);

	waitKey(0);
}
void mymerge2()
{
	string  path1 = "E:\\laboratory\\batchHumanPoseEstimation\\x64\\Debug\\body3DCoordinates0_0017.jpg";
	string  path2 = "E:\\laboratory\\batchHumanPoseEstimation\\x64\\Debug\\body3DCoordinates0_0017biaoding.jpg";
	string  path3 = "E:\\laboratory\\batchHumanPoseEstimation\\x64\\Debug\\xiacaiyang12.jpg";
	vector<Mat>input(2);
	
	input[0] = imread(path1), input[1] = imread(path2);
	

	Size bigsize(input[0].cols * 2, input[0].rows );//�ϲ���ͼƬsize 

	vector<Mat>temp(2);

	Mat mergefinal;
	mergefinal.create(bigsize, CV_MAKETYPE(input[0].depth(), 3));//rgb 3ͨ��
	mergefinal = Scalar::all(0);

	for (int i = 0; i < 2; i++)
	{
		temp[i] = mergefinal(Rect(i*input[0].cols, 0, input[0].cols, input[0].rows));
		input[i].copyTo(temp[i]); //copyͼƬ����Ӧλ��
	}

	imshow("merge", mergefinal);
	imwrite("kinect.jpg", mergefinal);

	waitKey(0);
}



int main()
{
	//xunishuju();
	//kinect();

	//mymerge2();

	temp4();
	//showKinectResult();
	//testKinectData();
	system("PAUSE");
}
