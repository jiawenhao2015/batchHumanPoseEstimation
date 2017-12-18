/*批量生成训练数据
将逐帧生成的特征点数据整合到一个文件里 features.txt
将所有的对应的关节点数据整合到一个文件里 groundtruth.txt*/
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

int index;//用来控制训练与测试的比例 index=5 说明每5个取一个测试

void addJointName(string path, string newpath);
//将每一帧的特征点坐标文件 整合到最终一个文件里
//path为输入文件路径  allFeatureFile为最终整合到一起的最终的特征文件
void integrateFeaturesFile(string path,ofstream& allFeatureFile)
{
	ifstream featuresFile(path);
	int x, y;
	vector<int>xVec;//保存所有特征点的x坐标
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
		cout << "不能打开文件！" << path.c_str()<<endl;
	}
	//多读了一遍最后一行 删除最后一个元素
	xVec.erase(xVec.end()-1);
	yVec.erase(yVec.end()-1);
	//每一行代表一帧的特征点
	for (int i = 0; i < xVec.size(); i++)
	{
		allFeatureFile << xVec[i] << " " << yVec[i] << " ";
	}
	allFeatureFile << endl;
}
//将每一帧的关节点坐标文件 整合到最终一个文件里
//path为输入文件路径  allGroundTruthFile为最终整合到一起的最终的关节文件
//虚拟数据grountruth 数据格式
void integrateGroundTruthFile(string path, ofstream& allGroundTruthFile, ofstream& allJointsDistanceFile)
{
	cout << path << endl;
	ifstream featuresFile(path);
	int x, y, z;
	string jointName;
	vector<int>xVec;//保存所有特征点的x坐标
	vector<int>yVec;

	map<string, int>mpx;//记录关节点名称与坐标
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
		cout << "不能打开文件！" << path.c_str() << endl;
	}
	//多读了一遍最后一行 删除最后一个元素
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	//每一行代表一帧的关节点
	for (int i = 0; i < xVec.size(); i++)
	{
		allGroundTruthFile << xVec[i] << " "<<yVec[i]<<" ";
	}
	allGroundTruthFile << endl;	

	///计算关节点之间距离 保存到文件中 lForeArm 到 lHand，rForeArm 到 rHand 
	allJointsDistanceFile << mpx["lForeArm"] - mpx["lHand"] << " " << mpy["lForeArm"] - mpy["lHand"] << " "
						  << mpx["rForeArm"] - mpx["rHand"] << " " << mpy["rForeArm"] - mpy["rHand"] << endl;

}
//真实数据groundtruth
vector<string> JOINTS = { "hip", "chest", "neck", "lShldr", "lForeArm", "lHand", "rShldr", "rForeArm", "rHand", "lThigh", "lShin", "lFoot", "rThigh", "rShin", "rFoot" };
void integrateGroundTruthFileRealData(string path, ofstream& allGroundTruthFile, ofstream& allJointsDistanceFile)
{
	cout << path << endl;
	ifstream featuresFile(path);
	int x, y;
	string jointName;
	vector<int>xVec;//保存所有特征点的x坐标
	vector<int>yVec;

	map<string, int>mpx;//记录关节点名称与坐标
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
		cout << "不能打开文件！" << path.c_str() << endl;
	}
	//多读了一遍最后一行 删除最后一个元素
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	//每一行代表一帧的关节点
	for (int i = 0; i < xVec.size(); i++)
	{
		allGroundTruthFile << xVec[i] << " " << yVec[i] << " ";
	}
	allGroundTruthFile << endl;

	///计算关节点之间距离 保存到文件中 lForeArm 到 lHand，rForeArm 到 rHand 
	allJointsDistanceFile << mpx["lForeArm"] - mpx["lHand"] << " " << mpy["lForeArm"] - mpy["lHand"] << " "
		<< mpx["rForeArm"] - mpx["rHand"] << " " << mpy["rForeArm"] - mpy["rHand"] << endl;

}
//读取映射矩阵到mat
Mat InitMat(string matrixPath)
{
	ifstream matrixFile(matrixPath);
	float temp;
	Mat m(32, 30, CV_32F);
	vector<float>xVec;//保存所有坐标
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
		cout << "不能打开文件！" << matrixPath.c_str() << endl;
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

//处理kinect采集的数据 将特征点 groundtruth整合到一起
int getKinectTrainingData()
{

	ofstream allFeatureFile("E:/trainprocess/train/kinect/allfeatures.txt");
	string featurePathPrefix("E:/laboratory/dataset/realdataresults/");
	char featurePath[128];

	ofstream allGroundTruthFile("E:/trainprocess/train/kinect/allGroundTruthFile.txt");
	string groundTruthPathPrefix("E:/laboratory/dataset/realdataresults/");
	char groundTruthPath[128];
	char newgroundTruthPath[128];
	
	//关节点之间距离的groundtruth
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
				if (!isexist.is_open())continue;//当前不存在 
				ifstream isexist1(groundTruthPathPrefix + newgroundTruthPath);
				if (!isexist1.is_open())continue;//当前不存在 

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


//处理虚拟数据 将特征点 groundtruth整合到一起
int geXuniTrainingData()
{

	ofstream allFeatureFile("E:/trainprocess/train/xunishujushixiong/allfeatures3.txt");
	string featurePathPrefix("E:/laboratory/dataset/synthesisdata/mypartresults/");
	char featurePath[128];

	ofstream allGroundTruthFile("E:/trainprocess/train/xunishujushixiong/allGroundTruthFile3.txt");
	string groundTruthPathPrefix("E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/");
	char groundTruthPath[128];
	

	//关节点之间距离的groundtruth
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
				if (!isexist.is_open())continue;//当前不存在 
				

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

/*手工标定的grountruth没有关节点名称 现在加上*/
void addJointName(string path,string newpath)
{
	ifstream featuresFile(path);
	int x, y;
	vector<int>xVec;//保存所有特征点的x坐标
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
		cout << "不能打开文件！" << path.c_str() << endl;
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




//处理kinect数据 整合特征点 手工标定的groundtruth
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

	//关节点之间距离的groundtruth
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
				if (!isexist.is_open())continue;//当前不存在 
				ifstream isexist1(groundTruthPathPrefix + newgroundTruthPath);
				if (!isexist1.is_open())continue;//当前不存在 

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
//自己标定了100帧kinect数据 验证结果
void testKinectTrainingData()
{
	ofstream allFeatureFile("E:/trainprocess/train/newkinect/allfeatures.txt");
	string featurePathPrefix("E:/laboratory/RecordProcess/RecordProcess/output/");
	char featurePath[128];

	ofstream allGroundTruthFile("E:/trainprocess/train/newkinect/allGroundTruthFile.txt");
	string groundTruthPathPrefix("E:/laboratory/RecordProcess/RecordProcess/output/");
	char groundTruthPath[128];
	

	//关节点之间距离的groundtruth
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
				if (!isexist.is_open())continue;//当前不存在 
				ifstream isexist1(groundTruthPathPrefix + groundTruthPath);
				if (!isexist1.is_open())continue;//当前不存在 

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