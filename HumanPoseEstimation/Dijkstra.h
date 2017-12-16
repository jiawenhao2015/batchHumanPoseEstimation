#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#include <highgui.h>
#include "Data.h"
#include "math.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;


struct node     //顶点节点，保存id和到源顶点的估算距离，优先队列需要的类型  
{
	int id;     //源顶点id和估算距离  
	float weight;
	friend bool operator<(node a, node b)   //因要实现最小堆，按升序排列，因而需要重载运算符，重定义优先级，以小为先  
	{
		return a.weight > b.weight;
	}
};


class Dijkstra
{

public:

	int downSampling;//用于记录下采样规模
	int STEP;//用于记录超像素种子间步长 
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
	vector<int>Connect;//用于检测联通的点，分割联通块、、、、、、
	int Pitch;//用于记录分割联通块的个数、、、、、、、、
	vector<int>Con_Point;

	void Reflash(int i);
	bool Graph(Data one, Data two);
	bool Sample_Graph(Data one, Data two);
	bool Sample_Graph(Data one, Data two, int scale);//点云下采样时候调用
	bool SampleFourNeighbors(Data one, Data two, int scale);//构建稀疏图 测试一下只取4邻域
	bool SampleAdaptive(Data one, Data two, int scale);//灵活调整取样 躯干点多的地方少取  手臂点少的不少取

	bool IsAdjacent(Data one, Data two, int STEP);

	Dijkstra(int Num, int CenterX, int CenterY, int scale, int step);
	~Dijkstra();

	//	void Make_Matrix(vector<Data> &src);

	void Make_Matrix(vector<Data> &src);///jwh 改的师兄的程序 

	float PointDistance(Data one, Data two);
	int SetPath(int v0);
	int MakeDJ(int v0, bool kaiguan, vector<Data> &src);
	void WritePath(int i, int v0, vector<Data>src, int dingdian);
	void Check(const vector<Data>&src);//、、、、、、
	int Search(int CenterX, int CenterY, const vector<Data> &src_data);//、、、、、、、、、、
	void Adjust();

	void _Check(int Vertex[6][2]);

	int MakeDJ2(int v0, bool kaiguan, vector<Data> &src, int center);
	int MakeDJ3(int v0, bool kaiguan, vector<Data> &src);
	void init_dis(int center);

	void testDijkstra(int start, vector<Data> &src, Mat &map);
	void showDistenceMap(Mat &map, vector<Data> &src, int v0, float max);
	//得到分割出来的集合
	void getIsolatedSet(Mat &map);


	//flag 为是否下采样的标记 默认此时为true 也就是下采样的情况
	void testDijkstra(int start, vector<Data> &src, Mat &map, bool flag);
	void showDistenceMap(Mat &map, vector<Data> &src, int v0, float max, bool flag);
	void getIsolatedSet(Mat &map, bool flag);


	node *nodearray;

	Mat distanceMap;

	//Mat testmap(480,640,CV_8UC3); 为什么不能直接声明




};

#endif