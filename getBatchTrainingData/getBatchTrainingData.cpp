/*��������ѵ������
����֡���ɵ��������������ϵ�һ���ļ��� features.txt
�����еĶ�Ӧ�Ĺؽڵ��������ϵ�һ���ļ��� groundtruth.txt*/
#include<iostream>
#include <stdio.h>
#include<vector>
#include <fstream>
#include <string>
#include <set>
#include "highgui.h"
using namespace std;
using namespace cv;


set<pair<int, int>> kinectWrong = { make_pair(1, 13), make_pair(3, 12), make_pair(3, 13),
make_pair(3, 14), make_pair(3, 15), make_pair(3, 20), make_pair(3, 21),
make_pair(3, 22), make_pair(5, 8), make_pair(5, 38), make_pair(5, 41) };

int index;//��������ѵ������Եı��� index=5 ˵��ÿ5��ȡһ������

void addJointName(string path, string newpath);
//��ÿһ֡�������������ļ� ���ϵ�����һ���ļ���
//pathΪ�����ļ�·��  allFeatureFileΪ�������ϵ�һ������յ������ļ�
void integrateFeaturesFile(string path,ofstream& allFeatureFile)
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
		cout << "���ܴ��ļ���" << path.c_str()<<endl;
	}
	//�����һ�����һ�� ɾ�����һ��Ԫ��
	xVec.erase(xVec.end()-1);
	yVec.erase(yVec.end()-1);
	//ÿһ�д���һ֡��������
	for (int i = 0; i < xVec.size(); i++)
	{
		allFeatureFile << xVec[i] << " " << yVec[i] << " ";
	}
	allFeatureFile << endl;
}
//��ÿһ֡�Ĺؽڵ������ļ� ���ϵ�����һ���ļ���
//pathΪ�����ļ�·��  allGroundTruthFileΪ�������ϵ�һ������յĹؽ��ļ�
//��������grountruth ���ݸ�ʽ
void integrateGroundTruthFile(string path, ofstream& allGroundTruthFile, ofstream& allJointsDistanceFile)
{
	cout << path << endl;
	ifstream featuresFile(path);
	int x, y, z;
	string jointName;
	vector<int>xVec;//���������������x����
	vector<int>yVec;

	map<string, int>mpx;//��¼�ؽڵ�����������
	map<string, int>mpy;
	if (featuresFile.is_open())
	{
		while (!featuresFile.eof())
		{
			featuresFile >> x >> y >> z >>jointName;
			xVec.push_back(x);
			yVec.push_back(y);
			mpx[jointName] = x;
			mpy[jointName] = y;
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << path.c_str() << endl;
	}
	//�����һ�����һ�� ɾ�����һ��Ԫ��
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	//ÿһ�д���һ֡�Ĺؽڵ�
	for (int i = 0; i < xVec.size(); i++)
	{
		allGroundTruthFile << xVec[i] << " "<<yVec[i]<<" ";
	}
	allGroundTruthFile << endl;	

	///����ؽڵ�֮����� ���浽�ļ��� lForeArm �� lHand��rForeArm �� rHand 
	allJointsDistanceFile << mpx["lForeArm"] - mpx["lHand"] << " " << mpy["lForeArm"] - mpy["lHand"] << " "
						  << mpx["rForeArm"] - mpx["rHand"] << " " << mpy["rForeArm"] - mpy["rHand"] << endl;

}
//��ʵ����groundtruth
vector<string> JOINTS = { "hip", "chest", "neck", "lShldr", "lForeArm", "lHand", "rShldr", "rForeArm", "rHand", "lThigh", "lShin", "lFoot", "rThigh", "rShin", "rFoot" };
void integrateGroundTruthFileRealData(string path, ofstream& allGroundTruthFile, ofstream& allJointsDistanceFile)
{
	cout << path << endl;
	ifstream featuresFile(path);
	int x, y;
	string jointName;
	vector<int>xVec;//���������������x����
	vector<int>yVec;

	map<string, int>mpx;//��¼�ؽڵ�����������
	map<string, int>mpy;
	if (featuresFile.is_open())
	{
		while (!featuresFile.eof())
		{
			featuresFile >> x >> y >> jointName;
			xVec.push_back(x);
			yVec.push_back(y);
			mpx[jointName] = x;
			mpy[jointName] = y;
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << path.c_str() << endl;
	}
	//�����һ�����һ�� ɾ�����һ��Ԫ��
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	//ÿһ�д���һ֡�Ĺؽڵ�
	for (int i = 0; i < xVec.size(); i++)
	{
		allGroundTruthFile << xVec[i] << " " << yVec[i] << " ";
	}
	allGroundTruthFile << endl;

	///����ؽڵ�֮����� ���浽�ļ��� lForeArm �� lHand��rForeArm �� rHand 
	allJointsDistanceFile << mpx["lForeArm"] - mpx["lHand"] << " " << mpy["lForeArm"] - mpy["lHand"] << " "
		<< mpx["rForeArm"] - mpx["rHand"] << " " << mpy["rForeArm"] - mpy["rHand"] << endl;

}
//��ȡӳ�����mat
Mat InitMat(string matrixPath)
{
	ifstream matrixFile(matrixPath);
	float temp;
	Mat m(32, 30, CV_32F);
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
	for (int i = 0; i < 32;i++)
	{
		for (int j = 0; j < 30;j++)
		{
			m.at<float>(i, j) = xVec[i * 30 + j];
		}
	}
	return m;	
}

//����kinect�ɼ������� �������� groundtruth���ϵ�һ��
int getKinectTrainingData()
{

	ofstream allFeatureFile("E:/trainprocess/train/kinect/allfeatures.txt");
	string featurePathPrefix("E:/laboratory/dataset/realdataresults/");
	char featurePath[128];

	ofstream allGroundTruthFile("E:/trainprocess/train/kinect/allGroundTruthFile.txt");
	string groundTruthPathPrefix("E:/laboratory/dataset/realdataresults/");
	char groundTruthPath[128];
	char newgroundTruthPath[128];
	
	//�ؽڵ�֮������groundtruth
	ofstream allJointsDistanceFile("E:/trainprocess/train/kinect/allJointsDisFile.txt");

	for (int model = 6; model <= 6; model++)
	{
		for (int action = 1; action <= 1; action++)//
		{			
			for (int frame = 0; frame <= 150;frame++)
			{			
				 
				sprintf_s(featurePath, "model%d/action%d/frame%d/featurePoints.txt", model, action, frame);
				sprintf_s(groundTruthPath, "model%d/action%d/%d/guessJoints.txt", model, action, frame);
				sprintf_s(newgroundTruthPath, "model%d/action%d/%d/guessJointsNew.txt", model, action, frame);
				
				ifstream isexist(featurePathPrefix + featurePath);
				if (!isexist.is_open())continue;//��ǰ������ 
				ifstream isexist1(groundTruthPathPrefix + newgroundTruthPath);
				if (!isexist1.is_open())continue;//��ǰ������ 

				//addJointName(groundTruthPathPrefix + groundTruthPath, groundTruthPathPrefix + newgroundTruthPath);
				integrateFeaturesFile(featurePathPrefix + featurePath, allFeatureFile);
				//integrateGroundTruthFileRealData(groundTruthPathPrefix + groundTruthPath, allGroundTruthFile, allJointsDistanceFile);
				integrateGroundTruthFileRealData(groundTruthPathPrefix + newgroundTruthPath, allGroundTruthFile, allJointsDistanceFile);
				cout << featurePath << endl;
				cout << groundTruthPath << endl;
			}
		}
	}

	allFeatureFile.close();
	allGroundTruthFile.close();
	allJointsDistanceFile.close();
	getchar();

	return 0;
}


//������������ �������� groundtruth���ϵ�һ��
int geXuniTrainingData()
{

	ofstream allFeatureFile("E:/trainprocess/train/xunishujushixiong/allfeatures3.txt");
	string featurePathPrefix("E:/laboratory/dataset/synthesisdata/mypartresults/");
	char featurePath[128];

	ofstream allGroundTruthFile("E:/trainprocess/train/xunishujushixiong/allGroundTruthFile3.txt");
	string groundTruthPathPrefix("E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/");
	char groundTruthPath[128];
	

	//�ؽڵ�֮������groundtruth
	ofstream allJointsDistanceFile("E:/trainprocess/train/xunishujushixiong/allJointsDisFile3.txt");

	for (int action = 7; action <= 7; action++)
	{
		for (int people = 1; people <= 1; people++)
		{
			for (int frame = 0; frame <= 299; frame++)
			{
				if (frame % index == 0)continue;
				sprintf_s(featurePath, "action%d/people%d/frame%d/featurePoints.txt", action, people, frame);
				sprintf_s(groundTruthPath, "action%d/people%d/groundTruth%d.txt",action,people, frame);
				
				cout << featurePathPrefix + featurePath << endl;

				ifstream isexist(featurePathPrefix + featurePath);
				if (!isexist.is_open())continue;//��ǰ������ 
				

				integrateFeaturesFile(featurePathPrefix + featurePath, allFeatureFile);
				
				integrateGroundTruthFile(groundTruthPathPrefix + groundTruthPath, allGroundTruthFile, allJointsDistanceFile);
				cout << featurePath << endl;
				cout << groundTruthPath << endl;
			}
		}
	}

	allFeatureFile.close();
	allGroundTruthFile.close();
	allJointsDistanceFile.close();
	getchar();

	return 0;
}

/*�ֹ��궨��grountruthû�йؽڵ����� ���ڼ���*/
void addJointName(string path,string newpath)
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
	for (int i = 0; i < xVec.size();i++)
	{
		of << xVec[i] << " " << yVec[i] << " " << JOINTS[i] << endl;
	}
	of.close();
}




//����kinect���� ���������� �ֹ��궨��groundtruth
//20171104
void getKinectTrainingData1()
{
	ofstream allFeatureFile("E:/trainprocess/train/kinectshixiong/allfeatures.txt");
	string featurePathPrefix("E:/laboratory/dataset/realdataresults/");
	char featurePath[128];

	ofstream allGroundTruthFile("E:/trainprocess/train/kinectshixiong/allGroundTruthFile.txt");
	string groundTruthPathPrefix("E:/laboratory/dataset/realdataresults/");
	char groundTruthPath[128];
	char newgroundTruthPath[128];

	//�ؽڵ�֮������groundtruth
	ofstream allJointsDistanceFile("E:/trainprocess/train/kinectshixiong/allJointsDisFile.txt");

	for (int model = 1; model <= 6; model++)
	{
		for (int action = 1; action <= 1; action++)//
		{
			for (int frame = 0; frame <= 99; frame++)
			{
				if (model == 3 || model == 5)continue;
				if (kinectWrong.count(make_pair(model, frame))) continue;
				if (frame % index == 0)continue;

				sprintf_s(featurePath, "model%d/action%d/%d/clusterpoint.txt", model, action, frame);
				sprintf_s(groundTruthPath, "model%d/action%d/%d/guessJoints.txt", model, action, frame);
				sprintf_s(newgroundTruthPath, "model%d/action%d/%d/guessJointsNew.txt", model, action, frame);

				ifstream isexist(featurePathPrefix + featurePath);
				if (!isexist.is_open())continue;//��ǰ������ 
				ifstream isexist1(groundTruthPathPrefix + newgroundTruthPath);
				if (!isexist1.is_open())continue;//��ǰ������ 

				integrateFeaturesFile(featurePathPrefix + featurePath, allFeatureFile);
				integrateGroundTruthFileRealData(groundTruthPathPrefix + newgroundTruthPath, allGroundTruthFile, allJointsDistanceFile);
				cout << featurePath << endl;
				cout << groundTruthPath << endl;
			}
		}
	}

	allFeatureFile.close();
	allGroundTruthFile.close();
	allJointsDistanceFile.close();
	getchar();	
}

//20171112
//�Լ��궨��100֡kinect���� ��֤���
void testKinectTrainingData()
{
	ofstream allFeatureFile("E:/trainprocess/train/newkinect/allfeatures.txt");
	string featurePathPrefix("E:/laboratory/RecordProcess/RecordProcess/output/");
	char featurePath[128];

	ofstream allGroundTruthFile("E:/trainprocess/train/newkinect/allGroundTruthFile.txt");
	string groundTruthPathPrefix("E:/laboratory/RecordProcess/RecordProcess/output/");
	char groundTruthPath[128];
	

	//�ؽڵ�֮������groundtruth
	ofstream allJointsDistanceFile("E:/trainprocess/train/newkinect/allJointsDisFile.txt");

	for (int model = 1; model <=1; model++)
	{
		for (int action = 1; action <= 1; action++)//
		{
			for (int frame = 0; frame <= 99; frame++)
			{
				if (model == 3 || model == 5)continue;
				//if (kinectWrong.count(make_pair(model, frame))) continue;
				//if (frame % index == 0)continue;

				sprintf_s(featurePath, "model%d/action%d/frame%d/featurePoints.txt", model, action, frame);
				sprintf_s(groundTruthPath, "model%d/action%d/%04dbiaodingjoints.txt", model, action, frame);
				

				ifstream isexist(featurePathPrefix + featurePath);
				if (!isexist.is_open())continue;//��ǰ������ 
				ifstream isexist1(groundTruthPathPrefix + groundTruthPath);
				if (!isexist1.is_open())continue;//��ǰ������ 

				integrateFeaturesFile(featurePathPrefix + featurePath, allFeatureFile);
				integrateGroundTruthFileRealData(groundTruthPathPrefix + groundTruthPath, allGroundTruthFile, allJointsDistanceFile);
				cout << featurePath << endl;
				cout << groundTruthPath << endl;
			}
		}
	}

	allFeatureFile.close();
	allGroundTruthFile.close();
	allJointsDistanceFile.close();
	getchar();
}

int main()
{
	index = 5;
	geXuniTrainingData();
	//getKinectTrainingData1();
	//testKinectTrainingData();

	return 0;
}