#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include <highgui.h>
#include "Data.h"
#include "math.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;


struct node     //����ڵ㣬����id�͵�Դ����Ĺ�����룬���ȶ�����Ҫ������  
{
	int id;     //Դ����id�͹������  
	float weight;
	friend bool operator<(node a, node b)   //��Ҫʵ����С�ѣ����������У������Ҫ������������ض������ȼ�����СΪ��  
	{
		return a.weight > b.weight;
	}
};


class Dijkstra
{

public:

	int downSampling;//���ڼ�¼�²�����ģ
	int STEP;//���ڼ�¼���������Ӽ䲽�� 
	float distance;
	float Threshold;
	Mat Dj;
	int Vertex[6][2];
	//vector<vector<int>> Dist;
	float*Dist[6];
	float* dis;

	float* distemp;

	int* Pre;
	bool*final;
	int count;
	int Xx, Yy;
	int disconnectPoint;
	int lefthand, leftfoot, righthand, rightfoot;//////////'''''
	vector<int>Connect;//���ڼ����ͨ�ĵ㣬�ָ���ͨ�顢����������
	int Pitch;//���ڼ�¼�ָ���ͨ��ĸ�������������������
	vector<int>Con_Point;

	void Reflash(int i);
	bool Graph(Data one, Data two);
	bool Sample_Graph(Data one, Data two);
	bool Sample_Graph(Data one, Data two, int scale);//�����²���ʱ�����
	bool SampleFourNeighbors(Data one, Data two, int scale);//����ϡ��ͼ ����һ��ֻȡ4����
	bool SampleAdaptive(Data one, Data two, int scale);//������ȡ�� ���ɵ��ĵط���ȡ  �ֱ۵��ٵĲ���ȡ

	bool IsAdjacent(Data one, Data two, int STEP);

	Dijkstra(int Num, int CenterX, int CenterY, int scale, int step);
	~Dijkstra();

	//	void Make_Matrix(vector<Data> &src);

	void Make_Matrix(vector<Data> &src);///jwh �ĵ�ʦ�ֵĳ��� 

	float PointDistance(Data one, Data two);
	int SetPath(int v0);
	int MakeDJ(int v0, bool kaiguan, vector<Data> &src);
	void WritePath(int i, int v0, vector<Data>src, int dingdian);
	void Check(const vector<Data>&src);//������������
	int Search(int CenterX, int CenterY, const vector<Data> &src_data);//��������������������
	void Adjust();

	void _Check(int Vertex[6][2]);

	int MakeDJ2(int v0, bool kaiguan, vector<Data> &src, int center);
	int MakeDJ3(int v0, bool kaiguan, vector<Data> &src);
	void init_dis(int center);

	void testDijkstra(int start, vector<Data> &src, Mat &map);
	void showDistenceMap(Mat &map, vector<Data> &src, int v0, float max);
	//�õ��ָ�����ļ���
	void getIsolatedSet(Mat &map);


	//flag Ϊ�Ƿ��²����ı�� Ĭ�ϴ�ʱΪtrue Ҳ�����²��������
	void testDijkstra(int start, vector<Data> &src, Mat &map, bool flag);
	void showDistenceMap(Mat &map, vector<Data> &src, int v0, float max, bool flag);
	void getIsolatedSet(Mat &map, bool flag);


	node *nodearray;

	Mat distanceMap;

	//Mat testmap(480,640,CV_8UC3); Ϊʲô����ֱ������




};

#endif