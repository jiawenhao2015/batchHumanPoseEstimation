#include "Loading.h"
#include "public.h"
void Loading::DownLoad_XuNiData(string infoName, vector<Data>&src_data, int scale)
{

	printf("The path file is opened...Loading...\n");
	ifstream fp(infoName);

	int num = 0;
	while (!fp.eof())//feof()���ط����ʾ�����ļ�β
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
	//eof������������������ʱ��ֵFALSE��������һ�����ݶ����һ��
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
	while (!fp.eof())//feof()���ط����ʾ�����ļ�β
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
	//eof������������������ʱ��ֵFALSE��������һ�����ݶ����һ��
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

// ӳ���ȥ�Ĺؽڵ���ܲ����������ϣ�û�ڵ������棬������ĵ�����ĵ�
int window = 50;
pair<int,int> Loading::SearchInNeighbor(int X, int Y, Mat& photo)
{
	//������Ѱ�������
	int distance = 10000;
	int minX = X, minY = Y;
	for (int x = X - window; x <= X + window; x++)
	{
		if (x<0||x>=320)continue;	 
		for (int y = Y - window; y <= Y + window; y++)
		{
			if (y<0 || y>=240)continue;
			if (photo.at<Vec3b>(y, x)[0] != 255)//��һ��������Ϊ255 
			{
				if (distance > (x - X)*(x - X) + (y - Y)*(y - Y))//û��Ҫ���� ֻ�ǱȽϴ�С
				{
					distance = (x - X)*(x - X) + (y - Y)*(y - Y);
					minX = x;//��¼��ʱ����
					minY = y;
				}
			}
		}
	}
	
	return  make_pair(minX, minY);
}