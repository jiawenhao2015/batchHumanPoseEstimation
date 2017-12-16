#include "Loading.h"
#include "public.h"
void Loading::DownLoad_XuNiData(string infoName, vector<Data>&src_data, int scale)
{

	printf("The path file is opened...Loading...\n");
	ifstream fp(infoName);

	int num = 0;
	while (!fp.eof())//feof()返回非零表示到达文件尾
	{
		Data temp;
		fp >> temp.x;
		fp >> temp.y;
		fp >> temp.Tx;
		fp >> temp.Ty;
		fp >> temp.Tz;		
		if ((temp.x % scale == 0) && (temp.y % scale == 0))
		{
			src_data.push_back(temp);
			num++;
		}
	}
	//eof函数，当读入无数据时赋值FALSE，因此最后一行数据多读了一遍
	src_data.pop_back();
	printf("Load Point Finished %d  success\n", num - 1);
	TotalPoints = num - 1;
	fp.close();

}
void Loading::DownLoad_Info(string infoName, vector<Data>&src_data, int scale)
{

	printf("The path file is opened...Loading...\n");
	ifstream fp(infoName);

	int num = 0;
	while (!fp.eof())//feof()返回非零表示到达文件尾
	{
		Data temp;
		fp >> temp.x;
		fp >> temp.y;
		fp >> temp.Tx;
		fp >> temp.Ty;
		fp >> temp.Tz;
		temp.y = 240 - temp.y;
		if ((temp.x % scale == 0) && (temp.y % scale == 0))
		{
			src_data.push_back(temp);
			num++;
		}
	}
	//eof函数，当读入无数据时赋值FALSE，因此最后一行数据多读了一遍
	src_data.pop_back();
	printf("Load Point Finished %d  success\n", num - 1);
	TotalPoints = num - 1;
	fp.close();

}

int Loading::Search(int CenterX, int CenterY, vector<Data> src_data)
{
	int srcN = src_data.size();
	for (int i = 0; i < srcN; i++)
	{
		if ((src_data[i].x == CenterX) && (src_data[i].y == CenterY))
			return i;
	}
	return -1;
}

// 映射回去的关节点可能不在人体身上，没在点云里面，找最近的点云里的点
int window = 50;
pair<int,int> Loading::SearchInNeighbor(int X, int Y, Mat& photo)
{
	//窗口中寻找最近点
	int distance = 10000;
	int minX = X, minY = Y;
	for (int x = X - window; x <= X + window; x++)
	{
		if (x<0||x>=320)continue;	 
		for (int y = Y - window; y <= Y + window; y++)
		{
			if (y<0 || y>=240)continue;
			if (photo.at<Vec3b>(y, x)[0] != 255)//第一个分量不为255 
			{
				if (distance > (x - X)*(x - X) + (y - Y)*(y - Y))//没必要开方 只是比较大小
				{
					distance = (x - X)*(x - X) + (y - Y)*(y - Y);
					minX = x;//记录此时坐标
					minY = y;
				}
			}
		}
	}
	
	return  make_pair(minX, minY);
}