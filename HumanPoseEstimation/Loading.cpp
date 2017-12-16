#include "Loading.h"


void Loading::Load_Info(string infoName, vector<Data>&src_data)
{
	{
		printf("The path file is opened...Loading...\n");//
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

			if ((temp.x % 2 == 0) && (temp.y % 2 == 0))//内存不够，下采样  //师兄只放入了像素为偶数的数据 对2取余 只有原来的1/4
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
}

//scale 取2 4 6 8等偶数
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

		if ((temp.x % scale == 0) && (temp.y % scale == 0))//内存不够，下采样  //师兄只放入了像素为偶数的数据 对2取余 只有原来的1/4
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
//将文件中的超像素点保存到vector中，并且构造一个种子点与其标号一一对应的map。
void Loading::Load_Info(string infoName, vector<Data>&src_data, int newmethod)
{
	printf("The path file is opened...Loading...\n");
	ifstream fp(infoName);

	int num = 0;
	while (!fp.eof())
	{
		Data temp;
		fp >> temp.x;
		fp >> temp.y;
		fp >> temp.Tx;
		fp >> temp.Ty;
		fp >> temp.Tz;
		src_data.push_back(temp);
		seedsmap[SeedPoint(temp.x,temp.y)] = num++;//保存对应标号
	}
	//eof函数，当读入无数据时赋值FALSE，因此最后一行数据多读了一遍
	src_data.pop_back();
	printf("Load Point Finished %d  success\n", num - 1);
	TotalPoints = num - 1;
	fp.close();
}
///centerx centery 可能不在src里 即没有在超像素处理过的body图片中点里面
//在body图片中找离centerx centery最近的点  当前中心点为中心的一个窗口内找
//窗口大小为
void Loading::Generate_Center_Point(vector<Data> &src_data, int &CenterX, int &CenterY, int windowSize, Mat slicBody)
{
	int srcN = src_data.size();
	CenterX = 0; CenterY = 0;
	double x=0.0, y=0.0;
	window = windowSize;

	for (int i = 0; i < srcN; i++)
	{
		x = x + src_data[i].x;
		y = y + src_data[i].y;
	}
	CenterX = (int)(x / srcN);
	CenterY = (int)(y / srcN);

	//窗口中寻找最近点
	int distance =10000;
	int minX = CenterX, minY = CenterY;
	for(int x = CenterX - windowSize; x <= CenterX + windowSize;x++)
	{
		for (int y = CenterY - windowSize; y <= CenterY + windowSize;y++)
		{
			if (slicBody.at<Vec3b>(y, x)[0] == 0)//第一个分量不为255 不为蓝色 为0说明为种子点
			{
				//cout << x << " " <<y<< endl;
				if (distance > (x - CenterX)*(x - CenterX) + (y - CenterY)*(y - CenterY))//没必要开方 只是比较大小
				{
					distance = (x - CenterX)*(x - CenterX) + (y - CenterY)*(y - CenterY);
					minX = x;//记录此时坐标
					minY = y;
				}
			}
		}
	}

	CenterX = minX;
	CenterY = minY;
	cout <<"center:"<< CenterX << " " << CenterY << endl;

}

void Loading::Generate_Center_Point(vector<Data> &src_data, int &CenterX, int &CenterY)
{
	int srcN = src_data.size();
	CenterX = 0; CenterY = 0;
	double x = 0.0, y = 0.0;
	

	for (int i = 0; i < srcN; i++)
	{
		x = x + src_data[i].x;
		y = y + src_data[i].y;
	}
	CenterX = (int)(x / srcN);
	CenterY = (int)(y / srcN);

	
	cout << "center:" << CenterX << " " << CenterY << endl;

}
bool Loading::Test(int CenterX, int CenterY, vector<Data> &src_data)
{
	bool sign = false;
	int srcN = src_data.size();
	for (int i = 0; i < srcN; i++)
	{
		if ((src_data[i].x == CenterX) && (src_data[i].y == CenterY))
		{
			sign = true;
			break;
		}
	}
	if (!sign)
		cout << "The center point missed..." << endl;
	return sign;

}

//不用src这个vector了 ，直接用一个3通道的photo来存储数据 相当于数组，直接访问X Y位置的值
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
///在人体所有像素点中查找骨架点 index
int Loading::Search_Adjust(int &x, int &y, vector<Data> src)
{
	Loading Load;
	bool sign = Load.Test(x, y, src);//判断几何中心点是否存在
	if (!sign)//采样导致点缺失
	{
		cout << "...Adjusting the  point..." << endl;
		if (x % 2 == 0);
		else x = x + 1;
		if (y % 2 == 0);
		else y = y + 1;
		sign = true;
	}

	int index = Load.Search(x, y, src);
	while (index == -1)
	{
		x -= 2;
		index = Load.Search(x, y, src);
	}


	return index;
}

int Loading::Get_Depth_Info(Data &data, vector<Data> src_data, int index, MyPoint &point)///得到chest 左右肩 左右髋 的深度 //数据有太多冗余 待优化
{

	data.Tx = src_data[index].Tx;
	data.Ty = src_data[index].Ty;
	data.Tz = src_data[index].Tz;
	//////又保存一遍 待优化

	point.x = data.Tx;
	point.y = data.Ty;
	point.z = data.Tz;

	cout << "Tx:" << point.x << " Ty:" << point.y << " Tz:" << point.z << endl;////很粗略显示一下结果。。。不知道是chest 左右肩 左右髋 到时候优化 
	return 1;
}
//将文件中的骨架点数据保存到向量skeleton中 首先根据像素坐标位置 然后在src中找到该点 得到x y z值
void Loading::LoadStandardSkeleton(string path, vector<MyPoint> &skeleton, vector<Data> src)
{
	if (_access(path.c_str(), 0) == -1)
	{
		cout << "LoadStandardSkeleton path is wrong!" << endl;
		return;
	}
	cout << "The path file is opened...Loading...\n" << endl;//
	ifstream fp(path);
	int num = 0;
	int index;
	while (!fp.eof())//feof()返回非零表示到达文件尾
	{
		int x, y, z;
		fp >> x;
		fp >> y;
		fp >> z;
		index = Search_Adjust(x, y, src);
		MyPoint temp;
		temp.x = src[index].Tx;
		temp.y = src[index].Ty;
		temp.z = src[index].Tz;
		skeleton.push_back(temp);

		cout << num << "skeleton point:" << skeleton[num].x << " " << skeleton[num].y << " " << skeleton[num].z << endl;

		num++;
	}
	//eof函数，当读入无数据时赋值FALSE，因此最后一行数据多读了一遍
	skeleton.pop_back();
	printf("Load Point Finished %d  success\n", num - 1);
	fp.close();

}

//在超像素种子点里找距离（x,y）最近的点的标号 body里保存着种子点的位置信息 是一个photo
int Loading::SearchInSlic(int X, int Y, Mat& slicseeds)
{
	//窗口中寻找最近点
	int distance = 10000,win = 60;
	int minX = X, minY = Y;
	int matHeight = slicseeds.rows, matWidth = slicseeds.cols;
	for (int x = X - win; x <= X + win; x++)
	{
		if (x < 0 || x >= matWidth)continue;
		for (int y = Y - win; y <= Y + win; y++)
		{
			if (y < 0 || y >= matHeight)continue;
			if (slicseeds.at<Vec3b>(y, x)[0] == 0)//第一个分量不为255 不为蓝色 为0说明为种子点
			{
				if (distance > (x - X)*(x - X) + (y - Y)*(y - Y))//没必要开方 只是比较大小
				{
					distance = (x - X)*(x -X) + (y - Y)*(y - Y);
					minX = x;//记录此时坐标
					minY = y;
				}
			}
		}
	}
	return seedsmap[SeedPoint(minX,minY)];
}