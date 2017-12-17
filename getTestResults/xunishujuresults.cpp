/*
1.批量测试虚拟数据实验结果。
2.根据训练矩阵生成测试结果。
3.kinect的结果也在这儿
4.eval的结果在eval那个工程里 D盘
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

#define		PROJECT_MAT		"E:/trainprocess/train/Amatrix.txt"		//映射矩阵路径
#define		JOINTS_NUM   15     //关节点groundtruth个数
#define		FEATURE_NUM  16    //特征点个数

#define		START		0		//测试起始帧索引
#define		END 		150		//测试结束帧索引

int actionStart,actionEnd;
int frameStart, frameEnd;
int index;//用来控制训练与测试的比例 index=5 说明每5个取一个测试

int KINECTHEIGHT = 480;
int KINECTWIDTH = 640;


//-------------------------------------------------------------------------------------------------------------------
// 读取文件
bool ReadFile(string filePath, vector<float>&errorVec)
{
	errorVec.clear();
	ifstream infile(filePath);
	float x;
	if (!infile.is_open())
	{
		cout << "不能打开文件----" << filePath << endl;
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

//读入一个txt 返回一个m*n的矩阵
Mat InitMat(string matrixPath, int m, int n)
{
	ifstream matrixFile(matrixPath);
	float temp;
	Mat mat(m, n, CV_32F);
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
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat.at<float>(i, j) = xVec[i * n + j];
		}
	}
	return mat;
}

//生成测试结果   输出每一帧的估计的关节点位置。
set<pair<int, int>> wrongAction = { make_pair(2, 0), make_pair(2, 136), make_pair(7, 52),
									make_pair(7, 53), make_pair(7, 54) };

set<pair<int, int>>kinectWrong = { make_pair(1, 13), make_pair(3, 12), make_pair(3, 13), 
make_pair(3, 14), make_pair(3, 15), make_pair(3, 20), make_pair(3, 21),
make_pair(3, 22), make_pair(5, 8), make_pair(5, 38), make_pair(5, 41) };

void ComputeJoint(string matrixPath)
{
	Mat m = InitMat(matrixPath, FEATURE_NUM * 2, JOINTS_NUM * 2);//读取训练得到的模型矩阵 *2 是因为是2维的

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
				if (!isexist.is_open())continue;//当前不存在 

				Mat featuresMat = InitMat(Path, 1, FEATURE_NUM * 2);

				Mat jointsMatrix(1, JOINTS_NUM * 2, CV_32F);//映射得到的关节点
				jointsMatrix = featuresMat * m;


				ss.str("");
				ss << "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action" << action
					<< "/people" << people << "/" << frame << "/guessPoints.txt";	

				sprintf_s(Path, "E:/laboratory/dataset/synthesisdata/shixiongSCRLRESULTS/action%d/people%d/%4d/guessPoints.txt", action, people, frame);
				//ifstream isexist(Path);

				ofstream of(Path);
				for (int i = 0; i < JOINTS_NUM; i++)//输出到本地 四舍五入
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
		else if (flag == 0)//测试结果 与groundtruth只是颜色不一样
		{
			circle(photo, Point(X[i], Y[i]), 2, Scalar(0, 255, 0), 5);
			putText(photo, ss.str(), Point(X[i], Y[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0));
		}
	}
}
//eval数据集groundtruth
bool readGroundTruth(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	int temp, temp2;
	float tx,ty,tz;
	
	if (!infile.is_open())
	{
		cout << "不能打开文件！" << endl;
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

//真实数据 kinect产生的结果
bool kinectResults(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	float temp, temp2;
	float tx, ty, tz;

	if (!infile.is_open())
	{
		cout << "不能打开文件！" << endl;
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
//虚拟数据 groundtruth
bool readXuNiGroundTruth(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	string jointName;
	int x, y, z;

	if (!infile.is_open())
	{
		cout << "虚拟数据groundtruth 不能打开！" <<filePath << endl;
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
//自己拍的kinect 标定的groundtruth
bool readKinectGroundTruth(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	string jointName;
	int x, y;

	if (!infile.is_open())
	{
		cout << "虚拟数据groundtruth 不能打开！" << filePath << endl;
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
//读取 测试生成的关节点文件
bool readGuessFile(string filePath, vector<int>&xVec, vector<int>&yVec)
{
	ifstream infile(filePath);
	float temp, temp2;
	 

	if (!infile.is_open())
	{
		cout << "不能打开文件！" << endl;
		return false;
	}
	while (!infile.eof())
	{
		infile >> temp >> temp2 ;
		xVec.push_back(temp);//为了保证四舍五入int a = b+0.5;		
		yVec.push_back(temp2);
	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	return true;
}
//显示eval数据集 以及groundtruth
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


//根据像素位置，索引到世界坐标位置，计算误差 厘米数 XY是groundtruth myXmyY是测试结果
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
		//映射回去的关节点可能不在人体身上，没在点云里面，找最近的点云里的点
		int cnt = 0;//记录查找次数 如果找超过10次没找到 就算了 return
		while (index1 == -1)
		{
			pa = Load.SearchInNeighbor(X[i], Y[i], photo);
			index1 = Load.Search(pa.first, pa.second, src);
		}
		while (index2 == -1)
		{
			if (cnt == 10)//找不到了
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
//根据像素位置，索引到世界坐标位置，计算误差 像素数 XY是groundtruth myXmyY是测试结果
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
		//映射回去的关节点可能不在人体身上，没在点云里面，找最近的点云里的点

		int cnt = 0;//记录查找次数 如果找超过10次没找到 就算了 return

		while (index1 == -1)
		{
			pa = Load.SearchInNeighbor(X[i], Y[i], photo);
			index1 = Load.Search(pa.first, pa.second, src);
		}
		while (index2 == -1)
		{
			if (cnt == 10)//找不到了
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


//批量生成测试结果 将测试结果 保存  并统计误差 bool flag 是否显示图片
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
	vector<int>myX, myY;//测试结果
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
				temp = ShowTool.getPhoto(src);//保存一下原始数据  不能用temp=photo 因为是引用！相当于本身

				if (flag)
				{
					drawJoints(photo, X, Y, 1);//groundtruth
					drawJoints(photo, myX, myY, 0);//测试结果
					imshow("img", photo);
					waitKey(1);
				}


				//计算绝对误差 xiangsu像素数 保存到本地
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
	vector<int>myX, myY;//测试结果

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
				Load.DownLoad_XuNiData(inputPath, src, 1);//xuni跟kinect格式一样

				photo = ShowTool.getPhoto(src);
				temp = ShowTool.getPhoto(src);//保存一下原始数据  不能用temp=photo 因为是引用！相当于本身

				if (flag)
				{
					drawJoints(photo, X, Y, 1);//groundtruth
					drawJoints(photo, myX, myY, 0);//测试结果
					imshow("img", photo);
					waitKey(1);
				}
				//计算绝对误差 xiangsu像素数 保存到本地
				sserror << featurePathPrefix << "model" << model << "/action" << action << "/" << frame << "/abserror.txt";
				ComputeKinectRealError(src, X, Y, myX, myY, sserror.str(), temp);
			}
		}
	}
}



//euclid error  每一帧的误差已经计算好  现在将他们汇总到一起 求平均
//每一个action为一个单位
void computeFinalError(int action)
{
	vector<vector<float>>error;//所有帧误差
	vector<float>frameError;//单帧误差

	char path[128];
	sprintf_s(path, "E:/trainprocess/action%d_euclid.txt",action);
	ofstream euclidErrorFile(path);//所有测试帧的误差信息

	sprintf_s(path,"E:/trainprocess/action%d_euclidfinal.txt", action);
	ofstream finalEuclidErrorFile(path);//所有帧平均误差

	sprintf_s(path,"E:/trainprocess/action%d_percentage.txt", action);
	ofstream percentage(path);

	string euclidErrorPrefix("D:/EVAL20170704/EVAL/depth/");
	 
	
	
	for (int frame = START; frame <= END; frame++)//读取误差到vector
	{
		if (wrongAction.count(make_pair(action, frame))) continue;
		if (frame % 10 != 0)continue;

		stringstream ss;
		ss << euclidErrorPrefix << action << "/" << frame << "/abserror.txt";
		ifstream isexist(ss.str());
		if (!isexist.is_open())continue;//当前不存在 
		if (!ReadFile(ss.str(), frameError))continue;

		error.push_back(frameError);			
	}	
	for (int i = 0; i < error.size();i++)//将vector输出到文件
	{
		for (int j = 0; j < error[i].size();j++)
		{
			euclidErrorFile << error[i][j] << " ";
		}
		euclidErrorFile << endl;
	}
	euclidErrorFile.close();
	for (int i = 0; i < error[0].size();i++)//按列访问
	{
		float err = 0;
		int cnt = 0;//统计误差小于10cm的 百分比
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

//将 序列的所有误差统计到一起
void computAllFinalError(string matrixName)
{
	vector<vector<float>>error;//所有帧误差
	vector<float>frameError;//单帧误差

	char path[128];
	sprintf_s(path, "E:/trainprocess/test/xunishujushixiong/%s_actionALLeuclid.txt",matrixName.c_str());
	ofstream euclidErrorFile(path);//所有测试帧的误差信息
	
	sprintf_s(path, "E:/trainprocess/test/xunishujushixiong/%s_actionALLeuclidfinal.txt", matrixName.c_str());
	ofstream finalEuclidErrorFile(path);//所有帧平均误差

	
	
	
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
				if (!isexist.is_open())continue;//当前不存在 
				if (!ReadFile(path, frameError))continue;

				error.push_back(frameError);
			}
		}
	}
	

	for (int i = 0; i < error.size(); i++)//将vector输出到文件
	{
		for (int j = 0; j < error[i].size(); j++)
		{
			euclidErrorFile << error[i][j] << " ";
		}
		euclidErrorFile << endl;
	}
	euclidErrorFile.close();

	vector<float>meanErr;//用来保存这一帧的所有关节点的误差  求 平均误差 以及所有误差和

	for (int i = 0; i < error[0].size(); i++)//按列访问
	{
		float err = 0;
		int cnt = 0;//统计误差小于10cm的 百分比
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
	vector<vector<float>>error;//所有帧误差
	vector<float>frameError;//单帧误差

	char path[128];
	sprintf_s(path, "E:/trainprocess/test/kinectshixiong/%s_actionALLeuclid.txt", matrixName.c_str());
	ofstream euclidErrorFile(path);//所有测试帧的误差信息

	sprintf_s(path, "E:/trainprocess/test/kinectshixiong/%s_actionALLeuclidfinal.txt", matrixName.c_str());
	ofstream finalEuclidErrorFile(path);//所有帧平均误差
	
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
				if (!isexist.is_open())continue;//当前不存在 
				if (!ReadFile(ss.str(), frameError))continue;

				error.push_back(frameError);
			}
		}
	}

	for (int i = 0; i < error.size(); i++)//将vector输出到文件
	{
		for (int j = 0; j < error[i].size(); j++)
		{
			euclidErrorFile << error[i][j] << " ";
		}
		euclidErrorFile << endl;
	}
	euclidErrorFile.close();

	vector<float>meanErr;//用来保存这一帧的所有关节点的误差  求 平均误差 以及所有误差和

	for (int i = 0; i < error[0].size(); i++)//按列访问
	{
		float err = 0;
		int cnt = 0;//统计误差小于10cm的 百分比
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

//由于矩阵名字并不规则 直接读取文件夹下矩阵文件名 存到vector里  
void checkMatrix(vector<string> & matname)
{
	 ////读list文件 读矩阵
	ifstream infile("E:\\trainprocess\\train\\xunishujushixiong\\matrix\\LIST.TXT");
	vector<string>matrxPath;
	if (!infile.is_open())
	{
		cout << "不能打开文件----checkMatrix"  << endl;
		return;
	}
	string t;
	while (!infile.eof())
	{
		infile >> t;
		matrxPath.push_back(t);
	}
	matrxPath.pop_back(), matrxPath.pop_back(), matrxPath.pop_back();//后2个没有用
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
	////读list文件 读矩阵
	ifstream infile(path + "LIST.TXT");
	vector<string>matrxPath;
	if (!infile.is_open())
	{
		cout << "不能打开文件----checkMatrix" << endl;
		return;
	}
	string t;
	while (!infile.eof())
	{
		infile >> t;
		matrxPath.push_back(t);
	}
	matrxPath.pop_back(), matrxPath.pop_back(), matrxPath.pop_back();//后2个没有用
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
		ComputeJoint(t);//1.首先计算测试关节点位置
		saveTestResults(false);//2.统计每一帧测试关节点 误差	
		computAllFinalError(name);//3计算所有误差
	}
	system("PAUSE");
	return 0;
}
/*
20171105
自己采集的 kinect数据 统计结果
*/

void ComputeKinectJoint(string matrixPath)
{

	Mat m = InitMat(matrixPath, FEATURE_NUM * 2, JOINTS_NUM * 2);//读取训练得到的模型矩阵 *2 是因为是2维的

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
				if (!isexist.is_open())continue;//当前不存在 

				Mat featuresMat = InitMat(ss.str(), 1, FEATURE_NUM * 2);

				Mat jointsMatrix(1, JOINTS_NUM * 2, CV_32F);//映射得到的关节点
				jointsMatrix = featuresMat * m;


				ss.str("");
				ss << "E:/laboratory/dataset/realdataresults/model" << model
					<< "/action" << action << "/" << frame <<"/shixiongguessPoints.txt";
				ofstream of(ss.str());
				for (int i = 0; i < JOINTS_NUM; i++)//输出到本地 四舍五入
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
		ComputeKinectJoint(t);//1.首先计算测试关节点位置
		saveKinectTestResults(false);//2.统计每一帧测试关节点 误差	1 13
		computAllKinectFinalError(name);//3计算所有误差
	}
	system("PAUSE");

	return 0;
}


/*20171111
手工标定的grountruth没有关节点名称 现在加上*/
void addJointName(string path, string newpath)
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
	for (int i = 0; i < xVec.size(); i++)
	{
		of << xVec[i] << " " << yVec[i] << " " << JOINTS[i] << endl;
	}
	of.close();
}
//20171109
//just显示kinetic给出的结果
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
//自己采集的 kinect数据 统计结果
void ComputeKinectTestJoint(string matrixPath)
{

	Mat m = InitMat(matrixPath, FEATURE_NUM * 2, JOINTS_NUM * 2);//读取训练得到的模型矩阵 *2 是因为是2维的

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
				if (!isexist.is_open())continue;//当前不存在 

				Mat featuresMat = InitMat(ss.str(), 1, FEATURE_NUM * 2);

				Mat jointsMatrix(1, JOINTS_NUM * 2, CV_32F);//映射得到的关节点
				jointsMatrix = featuresMat * m;


				ss.str("");
				ss << "E:/laboratory/RecordProcess/RecordProcess/output/model" << model
					<< "/action" << action << "/frame" << frame << "/guessPoints.txt";
				ofstream of(ss.str());
				for (int i = 0; i < JOINTS_NUM; i++)//输出到本地 四舍五入
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
//自己标定了100帧kinect数据 临时验证结果 只需要产生结果即可
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
		ComputeKinectTestJoint(t);//1.首先计算测试关节点位置
		
	}
	system("PAUSE");	 
}


//显示图片 将关节点  连线 
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
//显示3个聚类点并连线
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
//显示4个聚类点并连线
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

// 合并多张图像 显示结果
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

	Size bigsize(input[0].cols * 10, input[0].rows*2);//合并后图片size 宽*高
		
	vector<Mat>temp(20);

	Mat mergefinal;
	mergefinal.create(bigsize, CV_MAKETYPE(input[0].depth(), 3));//rgb 3通道
	mergefinal = Scalar::all(0);

	for (int i = 0; i < 20;i++)
	{
		if (i<10)temp[i] = mergefinal(Rect(i*input[0].cols, 0, input[0].cols, input[0].rows));
		else temp[i] = mergefinal(Rect((i - 10)*input[0].cols, input[0].rows, input[0].cols, input[0].rows));
		input[i].copyTo(temp[i]); //copy图片到对应位置
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
	

	Size bigsize(input[0].cols * 2, input[0].rows );//合并后图片size 

	vector<Mat>temp(2);

	Mat mergefinal;
	mergefinal.create(bigsize, CV_MAKETYPE(input[0].depth(), 3));//rgb 3通道
	mergefinal = Scalar::all(0);

	for (int i = 0; i < 2; i++)
	{
		temp[i] = mergefinal(Rect(i*input[0].cols, 0, input[0].cols, input[0].rows));
		input[i].copyTo(temp[i]); //copy图片到对应位置
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
