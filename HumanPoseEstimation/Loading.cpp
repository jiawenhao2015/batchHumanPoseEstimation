#include "Loading.h"


void Loading::Load_Info(string infoName, vector<Data>&src_data)
{
	{
		printf("The path file is opened...Loading...\n");//
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

			if ((temp.x % 2 == 0) && (temp.y % 2 == 0))//�ڴ治�����²���  //ʦ��ֻ����������Ϊż�������� ��2ȡ�� ֻ��ԭ����1/4
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
}

//scale ȡ2 4 6 8��ż��
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

		if ((temp.x % scale == 0) && (temp.y % scale == 0))//�ڴ治�����²���  //ʦ��ֻ����������Ϊż�������� ��2ȡ�� ֻ��ԭ����1/4
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
//���ļ��еĳ����ص㱣�浽vector�У����ҹ���һ�����ӵ�������һһ��Ӧ��map��
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
		seedsmap[SeedPoint(temp.x,temp.y)] = num++;//�����Ӧ���
	}
	//eof������������������ʱ��ֵFALSE��������һ�����ݶ����һ��
	src_data.pop_back();
	printf("Load Point Finished %d  success\n", num - 1);
	TotalPoints = num - 1;
	fp.close();
}
///centerx centery ���ܲ���src�� ��û���ڳ����ش������bodyͼƬ�е�����
//��bodyͼƬ������centerx centery����ĵ�  ��ǰ���ĵ�Ϊ���ĵ�һ����������
//���ڴ�СΪ
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

	//������Ѱ�������
	int distance =10000;
	int minX = CenterX, minY = CenterY;
	for(int x = CenterX - windowSize; x <= CenterX + windowSize;x++)
	{
		for (int y = CenterY - windowSize; y <= CenterY + windowSize;y++)
		{
			if (slicBody.at<Vec3b>(y, x)[0] == 0)//��һ��������Ϊ255 ��Ϊ��ɫ Ϊ0˵��Ϊ���ӵ�
			{
				//cout << x << " " <<y<< endl;
				if (distance > (x - CenterX)*(x - CenterX) + (y - CenterY)*(y - CenterY))//û��Ҫ���� ֻ�ǱȽϴ�С
				{
					distance = (x - CenterX)*(x - CenterX) + (y - CenterY)*(y - CenterY);
					minX = x;//��¼��ʱ����
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

//����src���vector�� ��ֱ����һ��3ͨ����photo���洢���� �൱�����飬ֱ�ӷ���X Yλ�õ�ֵ
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
///�������������ص��в��ҹǼܵ� index
int Loading::Search_Adjust(int &x, int &y, vector<Data> src)
{
	Loading Load;
	bool sign = Load.Test(x, y, src);//�жϼ������ĵ��Ƿ����
	if (!sign)//�������µ�ȱʧ
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

int Loading::Get_Depth_Info(Data &data, vector<Data> src_data, int index, MyPoint &point)///�õ�chest ���Ҽ� ������ ����� //������̫������ ���Ż�
{

	data.Tx = src_data[index].Tx;
	data.Ty = src_data[index].Ty;
	data.Tz = src_data[index].Tz;
	//////�ֱ���һ�� ���Ż�

	point.x = data.Tx;
	point.y = data.Ty;
	point.z = data.Tz;

	cout << "Tx:" << point.x << " Ty:" << point.y << " Tz:" << point.z << endl;////�ܴ�����ʾһ�½����������֪����chest ���Ҽ� ������ ��ʱ���Ż� 
	return 1;
}
//���ļ��еĹǼܵ����ݱ��浽����skeleton�� ���ȸ�����������λ�� Ȼ����src���ҵ��õ� �õ�x y zֵ
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
	while (!fp.eof())//feof()���ط����ʾ�����ļ�β
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
	//eof������������������ʱ��ֵFALSE��������һ�����ݶ����һ��
	skeleton.pop_back();
	printf("Load Point Finished %d  success\n", num - 1);
	fp.close();

}

//�ڳ��������ӵ����Ҿ��루x,y������ĵ�ı�� body�ﱣ�������ӵ��λ����Ϣ ��һ��photo
int Loading::SearchInSlic(int X, int Y, Mat& slicseeds)
{
	//������Ѱ�������
	int distance = 10000,win = 60;
	int minX = X, minY = Y;
	int matHeight = slicseeds.rows, matWidth = slicseeds.cols;
	for (int x = X - win; x <= X + win; x++)
	{
		if (x < 0 || x >= matWidth)continue;
		for (int y = Y - win; y <= Y + win; y++)
		{
			if (y < 0 || y >= matHeight)continue;
			if (slicseeds.at<Vec3b>(y, x)[0] == 0)//��һ��������Ϊ255 ��Ϊ��ɫ Ϊ0˵��Ϊ���ӵ�
			{
				if (distance > (x - X)*(x - X) + (y - Y)*(y - Y))//û��Ҫ���� ֻ�ǱȽϴ�С
				{
					distance = (x - X)*(x -X) + (y - Y)*(y - Y);
					minX = x;//��¼��ʱ����
					minY = y;
				}
			}
		}
	}
	return seedsmap[SeedPoint(minX,minY)];
}