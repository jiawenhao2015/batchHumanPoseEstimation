/*����ѵ�����ݡ�
ÿһ֡ ÿһ���ؽڵ���������ĵ�����һ����ؾ���
������֡��Ӧ�Ĺؽڵ�Ĳ�ؾ��� ��ƽ��ֵ ��ΪԼ���������Լ����*/

//�������  �����ĵ�v0   ��v0����  ��Dijkstra�㷨 ��ÿһ����Ĳ�ؾ��롣
//15���ؽڵ���֪��Ȼ�󰤸��ҵ���Ӧ�ı�ţ�
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


	if (!dirExists2(outputPath))////·�������ڴ���
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
	Mat  slicResult;//��ų����ش�����ͼ �����ӵ�
	Mat  slicBody;//��ʾ���峬���ص�
	Mat  slicseeds;//�������泬�������ӵ�
	SLIC slic;
	slic.outputPath = outputPath;

	Mat photo(HEIGHT, WIDTH, CV_8UC3);
	Mat depthData(HEIGHT, WIDTH, CV_64FC3);

	const int downSampling = 1;//downsampling=2
	const int clusterNumber = 3; //�������ھ������
	CvPoint2D32f gujiadian[5][clusterNumber];//�洢,�����ֺ����ҽż�ͷ��������������ĵ�3
	int gujiadian2[5][clusterNumber];
	ImageShow ShowTool;
	int CenterX, CenterY;
	vector<Data> src;
	vector<Data> originalsrc;//������ʼ��������
	Loading Load;

	//�����ܵ������ݶ�������src��
	Load.DownLoad_Info(inputPath, src, downSampling);
	ShowTool.ShowPhoto(photo, src, depthData);//����������ݵ�depthData     ��ʾͼƬ
	originalsrc = src;
	
	
	Load.Generate_Center_Point(src, CenterX, CenterY);

	bool sign = Load.Test(CenterX, CenterY, src);//�жϼ������ĵ��Ƿ����
	if (!sign)//�������¼������ĵ�ȱʧ
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

	//��ʹ��ʦ�־ɷ������� 
	int dingdian = v0;
	for (int i = 1; i <= 1; i++)
	{
		dijkstra.MakeDJ(v0, true, src);
		dijkstra.Vertex[i][0] = dijkstra.Xx;
		dijkstra.Vertex[i][1] = dijkstra.Yy;
		//dingdian = Load.Search(dijkstra.Vertex[i][0], dijkstra.Vertex[i][1], src);		
		//circle(photo, Point(dijkstra.Xx, dijkstra.Yy), 4, Scalar(0, 0, 255), -1);//���ӵ���ʾ
		//imwrite("show.jpg", photo);
	}

	vector<int> xVec;
	vector<int> yVec;
	vector<string>jointName;
	readJointsFile(jointsPath, xVec, yVec, jointName);

	char groundTruthGeodistance[128];//grountruth ÿһ���ؽڵ���ϵ����ĵ��ؾ���	
	sprintf_s(groundTruthGeodistance, "E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/action%d/people%d/groundTruth%ddis.txt", action, people, frame);
	ofstream groundTruthGeodistanceFile(groundTruthGeodistance);
	
	//��¼log 
	ofstream buglog("E:/laboratory/dataset/synthesisdata/bvhtransformdepthacquistion/buglog.txt", ios::out | ios::app);
	for (int i = 0; i < xVec.size();i++)
	{
		int index = Load.Search(xVec[i], yVec[i], src);	//̫�� ���Ż�

		if (index == -1)//˵��û���ҵ�  ��bug����������
		{
			//buglog << jointsPath <<" "<< jointName[i] << endl;
			cout << jointName[i] << index << " " << dijkstra.dis[index] << endl;
		}

		cout <<jointName[i]<<index<<"  "<< dijkstra.dis[index] << endl;
		//groundTruthGeodistanceFile << xVec[i] << " " << yVec[i] << " " << jointName[i] << " " << dijkstra.dis[index]<< endl;
		circle(photo, Point(xVec[i], yVec[i]), 2, Scalar(0, 0, 255), -1);//guanjie����ʾ
		putText(photo, jointName[i], Point(xVec[i], yVec[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	}
	imwrite("joints.jpg", photo);

	buglog.close();
	groundTruthGeodistanceFile.close();

}
//��ȡ�ؽڵ������ļ����浽vector��
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
		cout << "���ܴ��ļ���" << path.c_str() << endl;
	}
	//�����һ�����һ�� ɾ�����һ��Ԫ��
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	nameVec.pop_back();	

}

//��ʾ��bug��groundtruth  path�Ǵ������ ��bug��
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
		cout << "���ܴ��ļ���" << path.c_str() << endl;
	}

	for (it = gtFiles.begin(); it != gtFiles.end();it++)
	{
		vector<int> xVec;
		vector<int> yVec;
		vector<string>joints;
		string filepath = *it;
		string pointcloud = filepath.substr(0, 80);//��ȡ�ַ��� ·��ǰ׺
		string frame = filepath.substr(91, filepath.size() - 4 - 91);
		string action = filepath.substr(70, 1);
		string people = filepath.substr(78, 1);

		pointcloud = pointcloud + "output"+ frame + ".txt";



		vector<Data> src;		
		Loading Load;
		ImageShow ShowTool;
		Mat photo(HEIGHT, WIDTH, CV_8UC3);
		Mat depthData(HEIGHT, WIDTH, CV_64FC3);
		//�����ܵ������ݶ�������src��
		Load.DownLoad_Info(pointcloud, src, 1);
		ShowTool.ShowPhoto(photo, src, depthData);//����������ݵ�depthData     ��ʾͼƬ

		
		readJointsFile(filepath, xVec, yVec, joints);

		for (int i = 0; i < xVec.size(); i++)
		{		
			circle(photo, Point(xVec[i], yVec[i]), 2, Scalar(0, 0, 255), -1);//guanjie����ʾ
			putText(photo, joints[i], Point(xVec[i], yVec[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
		}
		imwrite("temp/action"+action+"people"+people+"frame"+frame+".jpg", photo);
	}

}

//�������������ݵ�groundtruth����ʾһ�� �ų�һ�´��󡣡�
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
	//�����ܵ������ݶ�������src��
	Load.DownLoad_Info(inputPath, src, 1);
	ShowTool.ShowPhoto(photo, src, depthData);//����������ݵ�depthData     ��ʾͼƬ
	vector<int> xVec;
	vector<int> yVec;
	vector<string>joints;
	
	readJointsFile(jointsPath, xVec, yVec, joints);

	for (int i = 0; i < xVec.size(); i++)
	{
		circle(photo, Point(xVec[i], yVec[i]), 2, Scalar(0, 0, 255), -1);//guanjie����ʾ
		putText(photo, joints[i], Point(xVec[i], yVec[i]), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));
	}
	char imgpath[256];
	sprintf_s(imgpath, "imgwithgroundtruth/action%dpeople%dframe%dframe.jpg", action, people, frame);	
	imwrite(imgpath, photo);
}