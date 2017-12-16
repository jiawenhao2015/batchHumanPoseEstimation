#include "Dijkstra.h"
#include <iostream>
#include <queue>
#include <stack>
#include<windows.h>
#include<winbase.h>

using namespace std;

#ifndef getmax
#define getmax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef getmin
#define getmin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//判断几何上是否满足八邻域的关系
bool Dijkstra::Graph(Data one, Data two)//未采样
{
	
	if (abs(one.x - two.x)>1)
		return false;
	else if (abs(one.y - two.y)>1)
		return false;
	else return true;

}
bool Dijkstra::Sample_Graph(Data one, Data two)//采样
{

	if (abs(one.x - two.x)>2) //////////////////load里面对4取余的时候取5 原来为2
		return false;
	else if (abs(one.y - two.y)>2)
		return false;
	else
		return true;
}
//下采样 使用    
bool Dijkstra::Sample_Graph(Data one, Data two, int scale)
{
	if (abs(one.x - two.x) > scale)
		return false;
	else if (abs(one.y - two.y) > scale)
		return false;
	else
		return true;
}
bool Dijkstra::SampleFourNeighbors(Data one, Data two, int scale)
{
	int x = abs(one.x - two.x);//0 or scale
	int y = abs(one.y - two.y);

	// x y的乘积不为0的话说明不是4邻域
	if (x*y == 0 && x <= scale  && y <= scale)
	{
		return true;
	}
	return false;
}
//尝试灵活取样 躯干整体少取点  手臂等小部位 多取点   先针对4邻域方向 左右上下
bool SampleAdaptive(Data one, Data two, int scale)
{

	int x = abs(one.x - two.x);//0 or scale
	int y = abs(one.y - two.y);

	// x y的乘积不为0的话说明不是4邻域方向
	if (x*y == 0 && x <= scale  && y <= scale)
	{
		return true;
	}
	return false;


}

//两个点是否构成边
//取决于视觉上是否邻接  即两个像素 横坐标差小于一定长度 纵坐标差小于一定长度
//
bool Dijkstra::IsAdjacent(Data one, Data two,int STEP)
{
	int X = abs(one.x - two.x);
	int Y = abs(one.y - two.y);
	if ((X <1.5*STEP)&& (Y<1.5*STEP))//待调整
	{
		return true;
	}
	return false;

}


float Dijkstra::PointDistance(Data p1, Data p2)  //两点间距离公式
{
	float result = 0;
	float temp = 0;
	temp = (p1.Tx - p2.Tx)*(p1.Tx - p2.Tx) + (p1.Ty - p2.Ty)*(p1.Ty - p2.Ty) + (p1.Tz - p2.Tz)*(p1.Tz - p2.Tz);
	result = sqrt(temp);
	return result;
}


Dijkstra::Dijkstra(int Num, int CenterX, int CenterY, int scale, int step)
{
	lefthand = 0;
	leftfoot = 0;
	righthand = 0;
	rightfoot = 0;
	distance = 0;

	downSampling = scale;
	STEP = step;

	Pitch = 1;/////、、、、、、、、、、、、、、、、、、
	for (int i = 0; i<Num; i++)
	{
		Connect.push_back(Pitch);/////、、、、、、、、、、、、、、、
	}
	Vertex[0][0] = CenterX;
	Vertex[0][1] = CenterY;
	Mat temp(Num, Num, CV_32FC1);//////CV_32FC1 代表32位数据
	temp.copyTo(Dj);
	for (int i = 0; i<Num; i++)
	for (int j = 0; j<Num; j++)
	{
		if (i == j)
			Dj.at<float>(i, j) = 0;
		else Dj.at<float>(i, j) = 10000;
	}


	Mat tempp(480, 640, CV_32FC3);
	tempp.copyTo(distanceMap);

	for (int i = 0; i < 480; i++)				////初始化map
	{
		for (int j = 0; j < 640; j++)
		{
			/*	map.at<Vec3b>(i, j)[0] = 255;
			map.at<Vec3b>(i, j)[1] = 255;
			map.at<Vec3b>(i, j)[2] = 255;*/

			distanceMap.at<Vec3f>(i, j)[0] = 0.0;
		}
	}


	Threshold = 6.967909;///38390
	//Threshold=0.0375;
	//Threshold=0.057909;/////虚拟数据时候师兄使用的阈值  调试一下使用Kinect真实数据时候的值
	//Threshold=0.0567909;
	//Threshold=0.0367909;
	count = Num;
	for (int j = 0; j<6; j++)
	{
		Dist[j] = new float[count];
	}

	Pre = new int[Num];
	final = new bool[Num];
	dis = new float[Num];
	distemp = new float[Num];

	nodearray = new node[Num];


}
Dijkstra::~Dijkstra()
{
	for (int i = 0; i<6; i++)
	{
		delete Dist[i];
	}
	//delete Dist;
	delete Pre;
	delete final;
	delete dis;
	delete distemp;

}
////////// 优化 省掉一半时间
void Dijkstra::Make_Matrix(vector<Data> &src)
{
	int num = 0;
	int PointNum = src.size();
	for (int x = 0; x<PointNum-1; x++)
	{
		for (int y = x+1; y<PointNum; y++)
		{
			if (IsAdjacent(src[x], src[y],STEP))
			{
				if (PointDistance(src[x], src[y]) <= Threshold)///只验证了欧式距离 将来可以在深度上也约束一下
				{
					Dj.at<float>(x, y) = PointDistance(src[x], src[y]);
					Dj.at<float>(y, x) = Dj.at<float>(x, y);
					num++;
				}
			}
		}
	}
	cout << "make_matrix: num:" << num << "/ PointNum*PointNum= " << PointNum << "*" << PointNum << "=" << PointNum*PointNum << endl;
}

void Dijkstra::init_dis(int center)
{
	for (int i = 0; i<count; i++)        //////初始化一下 dis 数组
	{
		dis[i] = Dj.at<float>(center, i);///v0为中心点
	}
}

int Dijkstra::SetPath(int v0)
{
	for (int i = 0; i<count; i++)
	{
		if (Dj.at<float>(v0, i)<10000)
			Pre[i] = v0;
		else Pre[i] = -1;
	}
	return 0;
}
/*
输出DJ矩阵中v0那行值，发现矩阵中v0那行一直也没有改变，因为师兄一直也没有操作？
*/

int Dijkstra::MakeDJ(int v0, bool kaiguan, vector<Data> &src)
{
	int num = 0;

	SetPath(v0);
	int dingdian;
	int w, v;
	int x0 = 0, y0 = 0;
	bool sign = false;
	float min, max;
	for (v = 0; v<count; ++v)
	{
		final[v] = false;
		dis[v] = Dj.at<float>(v0, v);
	}

	dis[v0] = 0;
	final[v0] = true;
	for (int i = 1; i<count; ++i)
	{
		min = 10000;
		for (w = 0; w<count; w++)///找到最近点v
		{
			if (!final[w])
			if (dis[w]<min)
			{
				v = w;
				min = dis[w];
			}
		}
		final[v] = true;
		for (w = 0; w<count; ++w)
		{
			if (!final[w] && (min + Dj.at<float>(v, w)<dis[w]))
			{
				num++;
				dis[w] = min + Dj.at<float>(v, w);
				Pre[w] = v;
			}
		}
	}
	cout << "松弛操作次数：" << num << endl;

	max = 0;
	float max2 = 0;
	for (int xx = 0; xx<count; xx++)
	{
		if ((dis[xx] != 10000) && (dis[xx]>max))
		{
			max = dis[xx];
			y0 = src[xx].y;
			x0 = src[xx].x;
			dingdian = xx;
			sign = true;
		}

		if (dis[xx]>10000)
		{
			if (dis[xx]>max2)
			{
				max2 = dis[xx];
				cout << max2 << endl;
			}
		}
	}
	if (sign && kaiguan){
		cout << x0 << " " << y0 << endl;
		Xx = x0;
		Yy = y0;
	}
	if (kaiguan)
	{
		while (Pre[dingdian] != v0)
		{
			Dj.at<float>(Pre[dingdian], dingdian) = 0;
			dingdian = Pre[dingdian];
		}
		Dj.at<float>(v0, dingdian) = 0;
	}
	return 0;
}

void Dijkstra::testDijkstra(int start, vector<Data> &src, Mat &map)
{
	int v0 = start;
	SetPath(v0);
	int dingdian;
	int w, v;
	int x0 = 0, y0 = 0;
	bool sign = false;
	float min, max;
	for (v = 0; v<count; ++v)
	{
		final[v] = false;
		dis[v] = Dj.at<float>(v0, v);
	}

	dis[v0] = 0;
	final[v0] = true;
	for (int i = 1; i<count; ++i)
	{
		min = 10000;
		for (w = 0; w<count; w++)///找到最近点v
		{
			if (!final[w])
			if (dis[w]<min)
			{
				v = w;
				min = dis[w];
			}
		}
		final[v] = true;
		for (w = 0; w<count; ++w)
		if (!final[w] && (min + Dj.at<float>(v, w)<dis[w])){
			dis[w] = min + Dj.at<float>(v, w);
			Pre[w] = v;
		}
	}

	max = 0.0, min = 100.0;
	for (int xx = 0; xx<count; xx++)
	{
		max = getmax(max, dis[xx]);
		min = getmin(min, dis[xx]);
	}
	cout << "maxxxxxxxxxxx:" << max << " " << min << endl;////max  0.496

	this->showDistenceMap(map, src, v0, max);
}

//针对下采样的情况
void Dijkstra::testDijkstra(int start, vector<Data> &src, Mat& map, bool flag)
{
	int v0 = start;
	SetPath(v0);
	int dingdian;
	int w, v;
	int x0 = 0, y0 = 0;
	bool sign = false;
	float min, max;
	for (v = 0; v < count; ++v)
	{
		final[v] = false;
		dis[v] = Dj.at<float>(v0, v);
	}

	dis[v0] = 0;
	final[v0] = true;
	for (int i = 1; i < count; ++i)
	{
		min = 10000;
		for (w = 0; w < count; w++)///找到最近点v
		{
			if (!final[w])
			if (dis[w] < min)
			{
				v = w;
				min = dis[w];
			}
		}
		final[v] = true;
		for (w = 0; w < count; ++w)
		if (!final[w] && (min + Dj.at<float>(v, w) < dis[w])){
			dis[w] = min + Dj.at<float>(v, w);
			Pre[w] = v;
		}
	}

	max = 0.0, min = 100.0;
	for (int xx = 0; xx < count; xx++)
	{
		max = getmax(max, dis[xx]);
		min = getmin(min, dis[xx]);
	}
	cout << "maxxxxxxxxxxx:" << max << " " << min << endl;////max  0.496



	SYSTEMTIME sys;
	GetLocalTime(&sys); ////计算一下用时
	cout << "distancemap:" << sys.wMinute << "-" << sys.wSecond << "-" << sys.wMilliseconds << endl;

	short int minute = sys.wMinute;
	short int second = sys.wSecond;
	short int millisecond = sys.wMilliseconds;

	this->showDistenceMap(map, src, v0, max, true);

	GetLocalTime(&sys);
	cout << "distancemap:" << sys.wMinute << "-" << sys.wSecond << "-" << sys.wMilliseconds << endl;
	int runningtime = (sys.wMinute - minute) * 60 * 1000 + (sys.wSecond - second) * 1000 + sys.wMilliseconds - millisecond;///
	cout << "running time:" << runningtime << "ms" << endl;

}

void Dijkstra::showDistenceMap(Mat &map, vector<Data> &src, int v0, float max)
{
	Mat temp(480, 640, CV_32FC3);/////
	temp.copyTo(distanceMap);

	for (int i = 0; i<480; i++)				////初始化map
	{
		for (int j = 0; j<640; j++)
		{
			map.at<Vec3b>(i, j)[0] = 255;
			map.at<Vec3b>(i, j)[1] = 255;
			map.at<Vec3b>(i, j)[2] = 255;

			distanceMap.at<Vec3f>(i, j)[0] = 0.0;
		}
	}

	CvScalar green = CV_RGB(0, 255, 0);  //green r g b 
	CvScalar red = CV_RGB(255, 0, 0);  //red
	double weightB = (red.val[0] - green.val[0]) / max;
	double weightG = (red.val[1] - green.val[1]) / max;
	double weightR = (red.val[2] - green.val[2]) / max;


	//在图片上显示距离颜色
	for (int xx = 0; xx<count; xx++)
	{
		int i = src[xx].x;
		int j = src[xx].y;

		//每个点的右边 下边 右下边都显示 看上去饱满些		 但此时效率慢 需要修改!!!

		//将每个像素和他相邻的像素之间的空隙填充 右边 和下边 两个方向
		for (int m = 0; m < downSampling; m++)
		{
			for (int n = 0; n < downSampling; n++)
			{
				map.at<Vec3b>(j + m, i + n)[0] = cvRound(weightB*dis[xx] + 256); //b
				map.at<Vec3b>(j + m, i + n)[1] = cvRound(weightG*dis[xx] + 256); //g
				map.at<Vec3b>(j + m, i + n)[2] = cvRound(weightR*dis[xx] + 256); //r

				distanceMap.at<Vec3f>(j + m, i + n)[0] = dis[xx];
			}
		}
	}

	////一个是左上角为坐标原点 一个为左下角  怎么区分记忆
	///作为图片时候是左上角为原点  作为数组时 按照行列来记忆 第几行 第几列

	cv::Point position(src[v0].x, src[v0].y);
	putText(map, "O", position, CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));

	map.at<Vec3b>(src[v0].y, src[v0].x)[0] = 0;
	map.at<Vec3b>(src[v0].y, src[v0].x)[1] = 0;//圆心
	map.at<Vec3b>(src[v0].y, src[v0].x)[2] = 0;


	namedWindow("map");
	imshow("map", map);

	int radius = 102;///先测试一下
	circle(map, position, radius, Scalar(255, 255, 255), -1);///将半径之内置为白色


	getIsolatedSet(map);

	waitKey(1);

}

//针对下采样的情况
void Dijkstra::showDistenceMap(Mat &map, vector<Data> &src, int v0, float max, bool flag)
{



	SYSTEMTIME sys;
	GetLocalTime(&sys); ////计算一下用时
	cout << "for for:" << sys.wMinute << "-" << sys.wSecond << "-" << sys.wMilliseconds << endl;

	short int minute = sys.wMinute;
	short int second = sys.wSecond;
	short int millisecond = sys.wMilliseconds;





	CvScalar green = CV_RGB(0, 255, 0);  //green r g b 
	CvScalar red = CV_RGB(255, 0, 0);  //red
	double weightB = (red.val[0] - green.val[0]) / max;
	double weightG = (red.val[1] - green.val[1]) / max;
	double weightR = (red.val[2] - green.val[2]) / max;




	//在图片上显示距离颜色
	for (int xx = 0; xx < count; xx++)
	{
		int i = src[xx].x;
		int j = src[xx].y;

		//将每个像素和他相邻的像素之间的空隙  不填充！！

		map.at<Vec3b>(j, i)[0] = cvRound(weightB*dis[xx] + 256); //b
		map.at<Vec3b>(j, i)[1] = cvRound(weightG*dis[xx] + 256); //g
		map.at<Vec3b>(j, i)[2] = cvRound(weightR*dis[xx] + 256); //r

		distanceMap.at<Vec3f>(j, i)[0] = dis[xx];

	}




	////一个是左上角为坐标原点 一个为左下角  怎么区分记忆
	///作为图片时候是左上角为原点  作为数组时 按照行列来记忆 第几行 第几列

	cv::Point position(src[v0].x, src[v0].y);
	putText(map, "O", position, CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0));

	map.at<Vec3b>(src[v0].y, src[v0].x)[0] = 0;
	map.at<Vec3b>(src[v0].y, src[v0].x)[1] = 0;//圆心
	map.at<Vec3b>(src[v0].y, src[v0].x)[2] = 0;




	//namedWindow("map");  先不显示 太浪费时间了！
	//imshow("map", map);



	int radius = 100;///先测试一下
	circle(map, position, radius, Scalar(255, 255, 255), -1);///将半径之内置为白色



	getIsolatedSet(map, true); ///90ms

	GetLocalTime(&sys);
	cout << "getIsolatedSet:" << sys.wMinute << "-" << sys.wSecond << "-" << sys.wMilliseconds << endl;
	int runningtime = (sys.wMinute - minute) * 60 * 1000 + (sys.wSecond - second) * 1000 + sys.wMilliseconds - millisecond;///
	cout << "running time:" << runningtime << "ms" << endl;

	//waitKey(1);
}
//map 图片里每一个像素都有一个位置 和标记 来表示是否访问过
struct Pixel
{
	Point position;
};

void Dijkstra::getIsolatedSet(Mat &map)
{
	Mat tempmap(480, 640, CV_32FC3);/////
	map.copyTo(tempmap);
	float maxDistence = 0.0;

	int xPosition, yPosition;

	for (int i = 0; i<480; i++)
	{
		for (int j = 0; j<640; j++)
		{
			if (map.at<Vec3b>(i, j)[0] == 0)///r g b中的b分量为0 背景颜色为白色 三分量均为255 //找到第一个区域
			{
				cout << "found~~" << j << " " << i << endl;

				xPosition = j;
				yPosition = i;
				//i = 479; //跳出循环
				//break;				

				Pixel pixel;
				pixel.position = Point(xPosition, yPosition);
				map.at<Vec3b>(yPosition, xPosition)[0] = 50;/////////b分量定义为50 作为标记 //标记 访问过

				stack<Pixel> set;
				set.push(pixel); //进栈

				while (!set.empty())
				{
					int i, j;
					Pixel currentPixel;
					currentPixel = set.top();
					set.pop();

					i = currentPixel.position.y;
					j = currentPixel.position.x;

					map.at<Vec3b>(i, j)[0] = 50;/////////b分量定义为50 作为标记 //标记 访问过		

					for (int m = -1; m <= 1; m++) //⑧邻域
					{
						for (int n = -1; n <= 1; n++)
						{
							if (!(m == 0 && n == 0))//m n不同时为0  m!=0 || n!= 0
							{
								if (map.at<Vec3b>(i + m, j + n)[0] == 0)
								{
									map.at<Vec3b>(i + m, j + n)[0] = 50;/////标记
									map.at<Vec3b>(i + m, j + n)[1] = 50;
									map.at<Vec3b>(i + m, j + n)[2] = 50;
									Pixel element;
									element.position.x = j + n;
									element.position.y = i + m;
									set.push(element);

									float currentDistance = distanceMap.at<Vec3f>(i + m, j + n)[0];

									if (currentDistance>maxDistence)  ///找到当前最大的距离
									{
										maxDistence = currentDistance;
										xPosition = j + n;
										yPosition = i + m;
									}
								}
							}
						}
					}
					//	cout<<"size:"<<set.size()<<endl;			
				}
				circle(tempmap, Point(xPosition, yPosition), 5, Scalar(255, 0, 0), 2);///画出点
				circle(tempmap, Point(xPosition, yPosition), 2, Scalar(0, 0, 0), 2);///画出点
				cout << xPosition << " " << yPosition << endl;
				cout << maxDistence << endl;
				maxDistence = 0.0;
			}
		}
	}
	namedWindow("tempmap");
	imshow("tempmap", tempmap);


	namedWindow("IsolatedSet");
	imshow("IsolatedSet", map);
	waitKey(1);
}

//针对下采样的情况
void Dijkstra::getIsolatedSet(Mat &map, bool flag)
{
	Mat tempmap(480, 640, CV_32FC3);/////
	map.copyTo(tempmap);
	float maxDistence = 0.0;

	int xPosition, yPosition;


	for (int i = 0; i < 480; i = i + downSampling)
	{

		for (int j = 0; j < 640; j = j + downSampling)
		{
			if (map.at<Vec3b>(i, j)[0] == 0)///r g b中的b分量为0 背景颜色为白色 三分量均为255 //找到第一个区域
			{
				cout << "found~~" << j << " " << i << endl;

				xPosition = j;
				yPosition = i;
				//i = 479; //跳出循环
				//break;				

				Pixel pixel;
				pixel.position = Point(xPosition, yPosition);
				map.at<Vec3b>(yPosition, xPosition)[0] = 50;/////////b分量定义为50 作为标记 //标记 访问过

				stack<Pixel> set;
				set.push(pixel); //进栈

				while (!set.empty())
				{
					int i, j;
					Pixel currentPixel;
					currentPixel = set.top();
					set.pop();

					i = currentPixel.position.y;
					j = currentPixel.position.x;

					map.at<Vec3b>(i, j)[0] = 50;/////////b分量定义为50 作为标记 //标记 访问过		

					for (int m = -downSampling; m <= downSampling; m = m + downSampling) //⑧邻域 此时下采样了 根据采样程度决定8邻域坐标
					{
						for (int n = -downSampling; n <= downSampling; n = n + downSampling)
						{
							if (!(m == 0 && n == 0))//m n不同时为0  m!=0 || n!= 0
							{
								if (map.at<Vec3b>(i + m, j + n)[0] == 0)
								{
									map.at<Vec3b>(i + m, j + n)[0] = 50;/////标记
									map.at<Vec3b>(i + m, j + n)[1] = 50;
									map.at<Vec3b>(i + m, j + n)[2] = 50;
									Pixel element;
									element.position.x = j + n;
									element.position.y = i + m;
									set.push(element);

									float currentDistance = distanceMap.at<Vec3f>(i + m, j + n)[0];

									if (currentDistance > maxDistence)  ///找到当前最大的距离
									{
										maxDistence = currentDistance;
										xPosition = j + n;
										yPosition = i + m;
									}
								}
							}
						}
					}
				}
				circle(tempmap, Point(xPosition, yPosition), 5, Scalar(255, 0, 0), 2);///画出点
				circle(tempmap, Point(xPosition, yPosition), 2, Scalar(0, 0, 0), 2);///画出点
				cout << xPosition << " " << yPosition << endl;
				cout << maxDistence << endl;
				maxDistence = 0.0;
			}
		}
	}
	//namedWindow("tempmap");  // 太费时间了！！
	//imshow("tempmap", tempmap);


	//namedWindow("IsolatedSet");
	//imshow("IsolatedSet", map);
	//waitKey(1);
}
//////**********************************


priority_queue<node> myqueue;     //优先队列，最小堆，实现Dijkstra的重要数据结构，用stl实现  
//int parent[count];           //每个顶点的父亲节点，可以用于还原最短路径树  
//bool visited[count];         //用于判断顶点是否已经在最短路径树中，或者说是否已找到最短路径  
//node d[count];               //源点到每个顶点估算距离，最后结果为源点到所有顶点的最短路。 

////尝试使用优先队列 优化dijkstra算法
/*
现在问题是只能执行一遍 找到一个极值点 运行到第二遍的时候 不能跳出while循环

*/
int Dijkstra::MakeDJ3(int v0, bool kaiguan, vector<Data> &src)
{
	int num = 0;


	int dingdian;
	int x0 = 0, y0 = 0;
	int test = 0;
	for (int v = 0; v<count; ++v)
	{
		final[v] = false;
		nodearray[v].id = v;
		nodearray[v].weight = Dj.at<float>(v0, v);
		if (Dj.at<float>(v0, v)<10000)
			Pre[v] = v0;
		else Pre[v] = -1;
	}

	nodearray[v0].weight = 0;      //源点到源点最短路权值为0  	
	final[v0] = true;


	int v = v0, min = 10000;
	for (int w = 0; w<count; w++)///找到最近点v
	{
		if (!final[w])
		if (nodearray[w].weight<min)
		{
			v = w;
			min = nodearray[w].weight;
		}
	}

	myqueue.push(nodearray[v]);

	while (!myqueue.empty())               //算法的核心，队列空说明完成了操作  
	{
		node cd = myqueue.top();          //取最小估算距离顶点  
		myqueue.pop();
		int v = cd.id;

		final[v] = true;
		//松弛操作  
		for (int w = 0; w <count; w++) //找所有与他相邻的顶点，进行松弛操作，更新估算距离，压入队列。  
		{
			if (!final[w] && nodearray[v].weight + Dj.at<float>(v, w) < nodearray[w].weight)
			{
				num++;
				nodearray[w].weight = nodearray[v].weight + Dj.at<float>(v, w);  ///////d[v].weight = d[u].weight+w[u][v]; 原来的语句 
				Pre[w] = v;
				myqueue.push(nodearray[w]);
			}
		}
	}
	cout << "松弛操作次数：" << num << endl;
	float max = 0;
	for (int xx = 0; xx<count; xx++)
	{
		if ((nodearray[xx].weight != 10000) && (nodearray[xx].weight > max))
		{
			max = nodearray[xx].weight;
			y0 = src[xx].y;
			x0 = src[xx].x;
			dingdian = xx;
		}
	}
	cout << x0 << " " << y0 << endl;
	Xx = x0;
	Yy = y0;

	if (kaiguan)
	{
		while (Pre[dingdian] != v0)
		{
			Dj.at<float>(Pre[dingdian], dingdian) = 0;
			dingdian = Pre[dingdian];
		}
		Dj.at<float>(v0, dingdian) = 0;
	}

	return 0;
}


/*
dijkstra 变种 第二趟仅将肢端点置为0 而不是将中心点到肢端点 整条路径置为0
*/
static int ii = 1;
int Dijkstra::MakeDJ2(int v0, bool kaiguan, vector<Data> &src, int center)
{

	SetPath(v0);
	int dingdian;
	int w, v;
	int x0 = 0, y0 = 0;
	bool sign = false;
	float min, max;

	for (v = 0; v<count; ++v)
	{
		final[v] = false;
		//dis[v]=Dj.at<float>(v0,v); ////师兄的		 
	}

	bool exchange;//记录是否发生更新  没有更新则退出
	//dis[v0]=0;
	//final[v0]=true;  ////取消注释则会出错

	for (int i = 1; i<count; ++i)
	{
		exchange = false;
		min = 10000;
		for (w = 0; w<count; w++)///找到没有访问过的最近点v
		{
			if (!final[w])
			{
				if (dis[w] <= min)
				{
					//////找到最小的dis   判断是否为中心点 如果是中心点......找到第二个min==0的点   是否为第一次执行
					if ((dis[w] == 0) && (w == center) && (ii != 1))
					{
						continue;
					}
					v = w;
					min = dis[w];
				}
			}
		}

		final[v] = true;
		for (w = 0; w<count; ++w)
		{
			if (!final[w] && (min + Dj.at<float>(v, w)<dis[w]))
			{
				dis[w] = min + Dj.at<float>(v, w);
				///Pre[w]=v;	 //用不到求路径
				exchange = true;
			}
		}
		if ((!exchange))///////放这儿不合适
		{
			cout << "break i=" << i << " ii=" << ii << endl;
			break;
		}
	}
	ii++;
	max = 0;
	float max2 = 0;
	for (int xx = 0; xx<count; xx++) /////找到最大极值点 
	{
		if ((dis[xx] != 10000) && (dis[xx]>max))
		{
			max = dis[xx];
			y0 = src[xx].y;
			x0 = src[xx].x;
			dingdian = xx;
			sign = true;
		}

	}

	for (int i = 0; i <count; i++) ////更新Dj矩阵 保存顶点到其他点的距离  注释掉没影响？？？？？
	{
		Dj.at<float>(v0, i) = dis[i];
	}

	if (sign && kaiguan)
	{
		cout << x0 << " " << y0 << endl;
		Xx = x0;
		Yy = y0;
	}
	if (kaiguan)
	{
		Dj.at<float>(v0, dingdian) = 0;  ///Dj是对称矩阵
		Dj.at<float>(dingdian, v0) = 0;

		dis[dingdian] = 0; //顶点对应置为0

	}
	return 0;
}

void Dijkstra::WritePath(int i, int v0, vector<Data>src, int dingdian)//i代表除去中心点后的其余五个端点序号【1-5】，v0代表中心点在data22中的标号
{
	//int dingdian=Search(Vertex[i][0],Vertex[i][1]);
	switch (i){
	case 1:
	{
			  ofstream Cwriter("output/Path1.txt", ios::out);
			  while (Pre[dingdian] != v0)
			  {
				  Cwriter << src[dingdian].x << " ";
				  Cwriter << src[dingdian].y << endl;
				  dingdian = Pre[dingdian];
			  }
			  Cwriter << src[v0].x << " ";
			  Cwriter << src[v0].y << endl;
			  Cwriter.close();
	}; break;
	case 2:
	{
			  ofstream Cwriter("output/Path2.txt", ios::out);
			  while (Pre[dingdian] != v0)
			  {
				  Cwriter << src[dingdian].x << " ";
				  Cwriter << src[dingdian].y << endl;
				  dingdian = Pre[dingdian];
			  }
			  Cwriter << src[v0].x << " ";
			  Cwriter << src[v0].y << endl;
			  Cwriter.close();
	}; break;
	case 3:
	{
			  ofstream Cwriter("output/Path3.txt", ios::out);
			  while (Pre[dingdian] != v0)
			  {
				  Cwriter << src[dingdian].x << " ";
				  Cwriter << src[dingdian].y << endl;
				  dingdian = Pre[dingdian];
			  }
			  Cwriter << src[v0].x << " ";
			  Cwriter << src[v0].y << endl;
			  Cwriter.close();
	}; break;
	case 4:
	{
			  ofstream Cwriter("output/Path4.txt", ios::out);
			  while (Pre[dingdian] != v0)
			  {
				  Cwriter << src[dingdian].x << " ";
				  Cwriter << src[dingdian].y << endl;
				  dingdian = Pre[dingdian];
			  }
			  Cwriter << src[v0].x << " ";
			  Cwriter << src[v0].y << endl;
			  Cwriter.close();
	}; break;
	case 5:
	{
			  ofstream Cwriter("output/Path5.txt", ios::out);
			  while (Pre[dingdian] != v0)
			  {
				  Cwriter << src[dingdian].x << " ";
				  Cwriter << src[dingdian].y << endl;
				  dingdian = Pre[dingdian];
			  }
			  Cwriter << src[v0].x << " ";
			  Cwriter << src[v0].y << endl;
			  Cwriter.close();
	}; break;
	}

}

void Dijkstra::Reflash(int i)
{
	for (int j = 0; j<count; j++)
	{
		Dist[i][j] = dis[j];
	}
}
void Dijkstra::Check(const vector<Data>&src)//迪杰斯特拉算法后，用于检测连通性
{
	int num = 0, first;
	bool sign = true;
	for (int i = 0; i<count; i++)
	{
		if (dis[i] == 10000)
		{
			num++;
			Connect[i] = Pitch + 1;
			sign = false;
		}
	}
	cout << "numbers:" << num << endl;
	if (num<30)////////////////////////////////////
	{
		sign = true; num = 0;
	}//////////////////////////////
	if (sign == false)
	{
		for (int i = 0; i<count; i++)
		{
			int x, y;
			x = src[i].x;
			y = src[i].y;
			if (Connect[i] == Pitch + 1)
			{
				if ((Search(x - 2, y - 2, src) != -1) && (Connect[Search(x - 2, y - 2, src)] == Pitch))
				{
					first = Search(x - 2, y - 2, src);
					break;
				}
				if ((Search(x - 2, y, src) != -1) && (Connect[Search(x - 2, y, src)] == Pitch))
				{
					first = Search(x - 2, y, src);
					break;
				}
				if ((Search(x - 2, y + 2, src) != -1) && (Connect[Search(x - 2, y + 2, src)] == Pitch))
				{
					first = Search(x - 2, y + 2, src);
					break;
				}
				if ((Search(x, y - 2, src) != -1) && (Connect[Search(x, y - 2, src)] == Pitch))
				{
					first = Search(x, y - 2, src);
					break;
				}
				if ((Search(x, y + 2, src) != -1) && (Connect[Search(x, y + 2, src)] == Pitch))
				{
					first = Search(x, y + 2, src);
					break;
				}
				if ((Search(x + 2, y - 2, src) != -1) && (Connect[Search(x + 2, y - 2, src)] == Pitch))
				{
					first = Search(x + 2, y - 2, src);
					break;
				}
				if ((Search(x + 2, y, src) != -1) && (Connect[Search(x + 2, y, src)] == Pitch))
				{
					first = Search(x + 2, y, src);
					break;
				}
				if ((Search(x + 2, y + 2, src) != -1) && (Connect[Search(x + 2, y + 2, src)] == Pitch))
				{
					first = Search(x + 2, y + 2, src);
					break;
				}
			}
		}
		cout << first << endl;
	}//end-if

	if (num>0)
		Pitch += 1;
	if (Pitch>1)
	{
		int x, y;
		bool sign = false;
		Data one = src[first];
		x = src[first].x;
		y = src[first].y;
		cout << x << "****" << y << endl;
		cout << Connect[first] << "-" << one.Tz << endl;

		int xl, yl, xu, yu, xr, yr, xd, yd;
		xl = x - 2; yl = y;
		xu = x; yu = y - 2;
		xr = x + 2; yr = y;
		xd = x; yd = y + 2;


		//x-=2;
		cout << x << "****" << y << endl;
		while (Search(xl, yl, src) != -1 || Search(xu, yu, src) != -1 || Search(xr, yr, src) != -1 || Search(xd, yd, src) != -1)
		{
			if (Search(xl, yl, src) != -1)
			{
				cout << "left:" << endl;
				cout << Connect[Search(xl, yl, src)] << "-" << src[Search(xl, yl, src)].Tz << endl;
				if ((Connect[Search(xl, yl, src)] != Connect[first]) && (abs(one.Tz - src[Search(xl, yl, src)].Tz)<0.01))
				{
					cout << "make it" << endl;
					distance = sqrt((x - xl)*(x - xl) + (y - yl)*(y - yl) + (one.Tz - src[Search(xl, yl, src)].Tz)*(one.Tz - src[Search(xl, yl, src)].Tz));//--------------------------------------------
					Con_Point.push_back(first);
					Con_Point.push_back(Search(xl, yl, src));
					cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
					cout << xl << "-----" << yl << endl;
					sign = true;
					break;
				}
				else
				{
					xl = xl - 2;
					yl = yl;
				}
			}
			if (Search(xu, yu, src) != -1)
			{
				cout << "up:" << endl;
				cout << Connect[Search(xu, yu, src)] << "-" << src[Search(xu, yu, src)].Tz << endl;
				if ((Connect[Search(xu, yu, src)] != Connect[first]) && (abs(one.Tz - src[Search(xu, yu, src)].Tz)<0.01))
				{
					cout << "make it" << endl;
					distance = sqrt((x - xu)*(x - xu) + (y - yu)*(y - yu) + (one.Tz - src[Search(xu, yu, src)].Tz)*(one.Tz - src[Search(xu, yu, src)].Tz));
					Con_Point.push_back(first);
					Con_Point.push_back(Search(xu, yu, src));
					cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
					cout << xu << "-----" << yu << endl;
					sign = true;
					break;
				}
				else
				{
					xu = xu;
					yu = yu - 2;
				}
			}
			if (Search(xr, yr, src) != -1)
			{
				cout << "right:" << endl;
				cout << Connect[Search(xr, yr, src)] << "-" << src[Search(xr, yr, src)].Tz << endl;
				if ((Connect[Search(xr, yr, src)] != Connect[first]) && (abs(one.Tz - src[Search(xr, yr, src)].Tz)<0.01))
				{
					cout << "make it" << endl;
					distance = sqrt((x - xr)*(x - xr) + (y - yr)*(y - yr) + (one.Tz - src[Search(xr, yr, src)].Tz)*(one.Tz - src[Search(xr, yr, src)].Tz));
					Con_Point.push_back(first);
					Con_Point.push_back(Search(xr, yr, src));
					cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
					cout << xr << "-----" << yr << endl;
					sign = true;
					break;
				}
				else
				{
					xr = xr + 2;
					yr = yr;
				}
			}
			if (Search(xd, yd, src) != -1)
			{
				cout << "down:" << endl;
				cout << Connect[Search(xd, yd, src)] << "-" << src[Search(xd, yd, src)].Tz << endl;
				if ((Connect[Search(xd, yd, src)] != Connect[first]) && (abs(one.Tz - src[Search(xd, yd, src)].Tz)<0.01))
				{
					cout << "make it" << endl;
					distance = sqrt((x - xd)*(x - xd) + (y - yd)*(y - yd) + (one.Tz - src[Search(xd, yd, src)].Tz)*(one.Tz - src[Search(xd, yd, src)].Tz));
					Con_Point.push_back(first);
					Con_Point.push_back(Search(xd, yd, src));
					cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
					cout << xd << "-----" << yd << endl;
					sign = true;
					break;
				}
				else
				{
					xd = xd;
					yd = yd + 2;
				}
			}
		}
		if (sign == false)
		{
			xl = x - 2; yl = y;
			xu = x; yu = y - 2;
			xr = x + 2; yr = y;
			xd = x; yd = y + 2;
			while (Search(xl, yl, src) != -1 || Search(xu, yu, src) != -1 || Search(xr, yr, src) != -1 || Search(xd, yd, src) != -1)
			{
				if (Search(xl, yl, src) != -1)
				{
					cout << "left:" << endl;
					cout << Connect[Search(xl, yl, src)] << "-" << src[Search(xl, yl, src)].Tz << endl;
					if ((Connect[Search(xl, yl, src)] != Connect[first]))
					{
						cout << "make it" << endl;
						distance = sqrt((x - xl)*(x - xl) + (y - yl)*(y - yl) + (one.Tz - src[Search(xl, yl, src)].Tz)*(one.Tz - src[Search(xl, yl, src)].Tz));
						Con_Point.push_back(first);
						Con_Point.push_back(Search(xl, yl, src));
						cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
						cout << xl << "-----" << yl << endl;
						sign = true;
						break;
					}
					else
					{
						xl = xl - 2;
						yl = yl;
					}
				}
				if (Search(xu, yu, src) != -1)
				{
					cout << "up:" << endl;
					cout << Connect[Search(xu, yu, src)] << "-" << src[Search(xu, yu, src)].Tz << endl;
					if ((Connect[Search(xu, yu, src)] != Connect[first]))
					{
						cout << "make it" << endl;
						distance = sqrt((x - xu)*(x - xu) + (y - yu)*(y - yu) + (one.Tz - src[Search(xu, yu, src)].Tz)*(one.Tz - src[Search(xu, yu, src)].Tz));
						Con_Point.push_back(first);
						Con_Point.push_back(Search(xu, yu, src));
						cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
						cout << xu << "-----" << yu << endl;
						sign = true;
						break;
					}
					else
					{
						xu = xu;
						yu = yu - 2;
					}
				}
				if (Search(xr, yr, src) != -1)
				{
					cout << "right:" << endl;
					cout << Connect[Search(xr, yr, src)] << "-" << src[Search(xr, yr, src)].Tz << endl;
					if ((Connect[Search(xr, yr, src)] != Connect[first]))
					{
						cout << "make it" << endl;
						distance = sqrt((x - xr)*(x - xr) + (y - yr)*(y - yr) + (one.Tz - src[Search(xr, yr, src)].Tz)*(one.Tz - src[Search(xr, yr, src)].Tz));
						Con_Point.push_back(first);
						Con_Point.push_back(Search(xr, yr, src));
						cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
						cout << xr << "-----" << yr << endl;
						sign = true;
						break;
					}
					else
					{
						xr = xr + 2;
						yr = yr;
					}
				}
				if (Search(xd, yd, src) != -1)
				{
					cout << "down:" << endl;
					cout << Connect[Search(xd, yd, src)] << "-" << src[Search(xd, yd, src)].Tz << endl;
					if ((Connect[Search(xd, yd, src)] != Connect[first]))
					{
						cout << "make it" << endl;
						distance = sqrt((x - xd)*(x - xd) + (y - yd)*(y - yd) + (one.Tz - src[Search(xd, yd, src)].Tz)*(one.Tz - src[Search(xd, yd, src)].Tz));
						Con_Point.push_back(first);
						Con_Point.push_back(Search(xd, yd, src));
						cout << src[first].x << "-----" << src[first].y << endl;//、、、、、
						cout << xd << "-----" << yd << endl;
						sign = true;
						break;
					}
					else
					{
						xd = xd;
						yd = yd + 2;
					}
				}
			}
		}

	}
	disconnectPoint = num;
	cout << "Disconected Points:" << num << endl;

	//	x-=2;
	//	cout<<x<<"****"<<y<<endl;
	//	while(Search(x,y,src)!=-1)
	//	{
	//		cout<<Connect[Search(x,y,src)]<<"-"<<src[Search(x,y,src)].Tz<<endl;
	//		if((Connect[Search(x,y,src)]!=Connect[first])  && (abs(one.Tz-src[Search(x,y,src)].Tz)<0.01))
	//		{
	//			Con_Point.push_back(first);
	//			Con_Point.push_back(Search(x,y,src));
	//			cout<<src[first].x<<"-----"<<src[first].y<<endl;//、、、、、
	//			cout<<x<<"-----"<<y<<endl;
	//			break;
	//		}
	//		x-=2;
	//	}
	//
	//}
	//disconnectPoint=num;
	//cout<<"Disconected Points:"<<num<<endl;
}
void Dijkstra::Adjust()
{
	if (disconnectPoint != 0)
	{
		//Dj.at<float>(Con_Point[0],Con_Point[1])=0.000001;//////
		//Dj.at<float>(Con_Point[1],Con_Point[0])=0.000001;
		Dj.at<float>(Con_Point[0], Con_Point[1]) = distance;//////
		Dj.at<float>(Con_Point[1], Con_Point[0]) = distance;
	}
}
int Dijkstra::Search(int CenterX, int CenterY, const vector<Data> &src_data)
{
	int srcN = src_data.size();
	for (int i = 0; i<srcN; i++)
	{
		if ((src_data[i].x == CenterX) && (src_data[i].y == CenterY))
			return i;
	}
	return -1;
}
void Dijkstra::_Check(int Vertex[6][2])
{
	bool sign[6];
	for (int i = 0; i<6; i++)
		sign[i] = false;
	sign[0] = true;//几何中心点
	sign[5] = true;//头部

	int temp = 0;
	int temp_sign = 0;

	int max_y = 0;
	int sec_y = 0;//分出腿
	for (int i = 0; i<2; i++)//--
	{
		for (int j = 0; j<6; j++)
		{
			if (sign[j] == false)
			{
				if (Vertex[j][1]>temp)
				{
					temp = Vertex[j][1];
					temp_sign = j;
				}

			}
		}

		temp = 0;
		sign[temp_sign] = true;
		if (i == 0)
			max_y = temp_sign;
		else
			sec_y = temp_sign;
	}
	if (Vertex[max_y][0]<Vertex[sec_y][0])
	{
		leftfoot = max_y;//图像中左边的脚
		rightfoot = sec_y;//图像中右边的脚
	}
	else{
		leftfoot = sec_y;
		rightfoot = max_y;
	}

	///////////////////////////////////////////////////////
	int max_x = 0;
	int sec_x = 0;//分出手
	for (int i = 0; i<2; i++)//--
	{
		for (int j = 0; j<6; j++)
		{
			if (sign[j] == false)
			{
				if (Vertex[j][0]>temp)
				{
					temp = Vertex[j][0];
					temp_sign = j;
				}

			}
		}

		temp = 0;
		sign[temp_sign] = true;
		if (i == 0)
			max_x = temp_sign;
		else
			sec_x = temp_sign;
	}

	lefthand = sec_x;//图像中左边的手
	righthand = max_x;//图像中右边的手

}
