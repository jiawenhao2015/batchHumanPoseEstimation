/*处理训练数据、
每一帧 每一个关节点相对于中心点生成一个测地距离
将所有帧相应的关节点的测地距离 求平均值 作为约束方程里的约束项*/

//读入点云  求中心点v0   从v0出发  用Dijkstra算法 求每一个点的测地距离。
//15个关节点已知，然后挨个找到对应的标号？
#include <iostream>
#include "highgui.h"
#include<windows.h>
#include "slic.h"
#include "ImageShow.h"
#include "Loading.h"
#include "Dijkstra.h"
#include "Cluster.h"
#include "Kmeans.h"
#include <ctime>
#include "Skeleton.h"
#include<winbase.h>
#include<direct.h>
#include <set>
using namespace std;
using namespace cv;

#define HEIGHT 480
#define WIDTH   640
#define NUM_SUPERPIXEL 2500



void saveAllImage(int action,int people,int frame);
void saveBugImage(string path);
void readJointsFile(string path, vector<int>& xVec, vector<int>& yVec, vector<string>& nameVec);
bool dirExists2(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!  

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!  
	return false;    // this is not a directory!  
}
void run2(int action, int people, int frame);
int main1()
{
	//saveBugImage("E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/buglog.txt");
	for (int action = 3; action <= 3; action++)
	{
		for (int people = 1; people <= 4; people++)
		{
			for (int frame = 0; frame <= 49; frame++)
			{
				
				run2(action, people, frame);
				//saveAllImage(action, people, frame);
				
			}
		}
	}
	getchar();
	return 0;
}

void run2(int action, int people, int frame)
{
	char jointsPath[128];
	sprintf_s(jointsPath, "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action%d/people%d/groundTruth%d.txt", action, people, frame);


	char inputPath[128];
	sprintf_s(inputPath, "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action%d/people%d/output%d.txt", action, people, frame);
	cout << "------------------------" << inputPath << endl;
	char outputPath[128];
	sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/frame%d", action, people, frame);


	if (!dirExists2(outputPath))////路径不存在创建
	{
		if (_mkdir(outputPath) == 0)
		{
			sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/frame%d/1", action, people, frame);
			_mkdir(outputPath);
			sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/frame%d/2", action, people, frame);
			_mkdir(outputPath);
			sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/frame%d/3", action, people, frame);
			_mkdir(outputPath);
			sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/frame%d/4", action, people, frame);
			_mkdir(outputPath);
			sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/frame%d/5", action, people, frame);
			_mkdir(outputPath);
			cout << "success to mkdir" << endl;
		}
		else
		{
			cout << "fail to mkdir!!!!" << endl;
			getchar();
		}
	}
	sprintf_s(outputPath, "E:/laboratory/dataset/synthesisdata/mypartresults/action%d/people%d/frame%d", action, people, frame);

	DWORD dwLast;
	DWORD dwCurrent;
	Mat  slicResult;//存放超像素处理结果图 有种子点
	Mat  slicBody;//显示人体超像素点
	Mat  slicseeds;//用来保存超像素种子点
	SLIC slic;
	slic.outputPath = outputPath;

	Mat photo(HEIGHT, WIDTH, CV_8UC3);
	Mat depthData(HEIGHT, WIDTH, CV_64FC3);

	const int downSampling = 1;//downsampling=2
	const int clusterNumber = 3; //定义类内聚类个数
	CvPoint2D32f gujiadian[5][clusterNumber];//存储,左右手和左右脚加头，聚类个几何中心点3
	int gujiadian2[5][clusterNumber];
	ImageShow ShowTool;
	int CenterX, CenterY;
	vector<Data> src;
	vector<Data> originalsrc;//保留初始点云数据
	Loading Load;

	//将稠密点云数据读入向量src中
	Load.DownLoad_Info(inputPath, src, downSampling);
	ShowTool.ShowPhoto(photo, src, depthData);//保存点云数据到depthData     显示图片
	originalsrc = src;
	
	
	Load.Generate_Center_Point(src, CenterX, CenterY);

	bool sign = Load.Test(CenterX, CenterY, src);//判断几何中心点是否存在
	if (!sign)//采样导致几何中心点缺失
	{
		cout << "...Adjusting the center point..." << endl;
		if (CenterX % 2 == 0);
		else CenterX = CenterX + 1;
		if (CenterY % 2 == 0);
		else CenterY = CenterY + 1;
		sign = true;
	}
	int v0 = Load.Search(CenterX, CenterY, src);
	int center = v0;
	while (v0 == -1)
	{
		CenterX -= 2;
		v0 = Load.Search(CenterX, CenterY, src);
	}

	Dijkstra dijkstra(src.size(), CenterX, CenterY, 2, 2);
	dijkstra.Make_Matrix(src);

	//先使用师兄旧方法测试 
	int dingdian = v0;
	for (int i = 1; i <= 1; i++)
	{
		dijkstra.MakeDJ(v0, true, src);
		dijkstra.Vertex[i][0] = dijkstra.Xx;
		dijkstra.Vertex[i][1] = dijkstra.Yy;
		//dingdian = Load.Search(dijkstra.Vertex[i][0], dijkstra.Vertex[i][1], src);		
		//circle(photo, Point(dijkstra.Xx, dijkstra.Yy), 4, Scalar(0, 0, 255), -1);//种子点显示
		//imwrite("show.jpg", photo);
	}

	vector<int> xVec;
	vector<int> yVec;
	vector<string>jointName;
	readJointsFile(jointsPath, xVec, yVec, jointName);

	char groundTruthGeodistance[128];//grountruth 每一个关节点带上到中心点测地距离	
	sprintf_s(groundTruthGeodistance, "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action%d/people%d/groundTruth%ddis.txt", action, people, frame);
	ofstream groundTruthGeodistanceFile(groundTruthGeodistance);
	
	//记录log 
	ofstream buglog("E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/buglog.txt", ios::out | ios::app);
	for (int i = 0; i < xVec.size();i++)
	{
		int index = Load.Search(xVec[i], yVec[i], src);	//太慢 待优化

		if (index == -1)//说明没有找到  有bug！！！！！
		{
			//buglog << jointsPath <<" "<< jointName[i] << endl;
			cout << jointName[i] << index << " " << dijkstra.dis[index] << endl;
		}

		cout <<jointName[i]<<index<<"  "<< dijkstra.dis[index] << endl;
		//groundTruthGeodistanceFile << xVec[i] << " " << yVec[i] << " " << jointName[i] << " " << dijkstra.dis[index]<< endl;
		circle(photo, Point(xVec[i], yVec[i]), 2, Scalar(0, 0, 255), -1);//guanjie点显示
		putText(photo, jointName[i], Point(xVec[i], yVec[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	}
	imwrite("joints.jpg", photo);

	buglog.close();
	groundTruthGeodistanceFile.close();

}
//读取关节点坐标文件保存到vector中
void readJointsFile(string path, vector<int>& xVec, vector<int>& yVec, vector<string>& nameVec)
{
	ifstream featuresFile(path);
	int x, y, z;
	string jointName;
	
	if (featuresFile.is_open())
	{
		while (!featuresFile.eof())
		{
			featuresFile >> x >> y >> z >> jointName;
			xVec.push_back(x);
			yVec.push_back(y);
			nameVec.push_back(jointName);
		}
	}
	else
	{
		cout << "不能打开文件！" << path.c_str() << endl;
	}
	//多读了一遍最后一行 删除最后一个元素
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	nameVec.pop_back();	

}

//显示有bug的groundtruth  path是存放所有 有bug的
void saveBugImage(string path)
{
	ifstream bugFile(path);
	string gtFilePath, jointName;
	set<string>gtFiles;
	set<string>::iterator it;
	if (bugFile.is_open())
	{
		while (!bugFile.eof())
		{
			bugFile >> gtFilePath >> jointName;
			gtFiles.insert(gtFilePath);
		}
	}
	else
	{
		cout << "不能打开文件！" << path.c_str() << endl;
	}

	for (it = gtFiles.begin(); it != gtFiles.end();it++)
	{
		vector<int> xVec;
		vector<int> yVec;
		vector<string>joints;
		string filepath = *it;
		string pointcloud = filepath.substr(0, 80);//截取字符串 路径前缀
		string frame = filepath.substr(91, filepath.size() - 4 - 91);
		string action = filepath.substr(70, 1);
		string people = filepath.substr(78, 1);

		pointcloud = pointcloud + "output"+ frame + ".txt";



		vector<Data> src;		
		Loading Load;
		ImageShow ShowTool;
		Mat photo(HEIGHT, WIDTH, CV_8UC3);
		Mat depthData(HEIGHT, WIDTH, CV_64FC3);
		//将稠密点云数据读入向量src中
		Load.DownLoad_Info(pointcloud, src, 1);
		ShowTool.ShowPhoto(photo, src, depthData);//保存点云数据到depthData     显示图片

		
		readJointsFile(filepath, xVec, yVec, joints);

		for (int i = 0; i < xVec.size(); i++)
		{		
			circle(photo, Point(xVec[i], yVec[i]), 2, Scalar(0, 0, 255), -1);//guanjie点显示
			putText(photo, joints[i], Point(xVec[i], yVec[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
		}
		imwrite("temp/action"+action+"people"+people+"frame"+frame+".jpg", photo);
	}

}

//将所有虚拟数据的groundtruth都显示一下 排除一下错误。。
void saveAllImage(int action, int people, int frame)
{
	char jointsPath[128];
	sprintf_s(jointsPath, "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action%d/people%d/groundTruth%d.txt", action, people, frame);
	
	char inputPath[128];
	sprintf_s(inputPath, "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action%d/people%d/output%d.txt", action, people, frame);
	cout << "------------------------" << inputPath << endl;
	
	vector<Data> src;
	Loading Load;
	ImageShow ShowTool;
	Mat photo(HEIGHT, WIDTH, CV_8UC3);
	Mat depthData(HEIGHT, WIDTH, CV_64FC3);
	//将稠密点云数据读入向量src中
	Load.DownLoad_Info(inputPath, src, 1);
	ShowTool.ShowPhoto(photo, src, depthData);//保存点云数据到depthData     显示图片
	vector<int> xVec;
	vector<int> yVec;
	vector<string>joints;
	
	readJointsFile(jointsPath, xVec, yVec, joints);

	for (int i = 0; i < xVec.size(); i++)
	{
		circle(photo, Point(xVec[i], yVec[i]), 2, Scalar(0, 0, 255), -1);//guanjie点显示
		putText(photo, joints[i], Point(xVec[i], yVec[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	}
	char imgpath[256];
	sprintf_s(imgpath, "imgwithgroundtruth/action%dpeople%dframe%dframe.jpg", action, people, frame);	
	imwrite(imgpath, photo);
}