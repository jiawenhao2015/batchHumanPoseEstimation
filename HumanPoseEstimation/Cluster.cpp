#include "Cluster.h"
#define CLASSNUM 6
Cluster::~Cluster()
{
	if (label)
	{
	//	delete[] label;
	}

}
bool Cluster::isHand(int t)
{
	if (t == hand[0])return true;
	else if (t == hand[1])return true;
	else return false;
}
bool Cluster::isFoot(int t)
{
	if (t == foot[0] || t == foot[1])
		return true;
	else
		return false;
}
void Cluster::Chek(int Vertex[6][2])//hand用于存放手部肢端点标签（1,2,3,4中的两个）
{
	bool hand_foot[4];
	memset(hand_foot, 0, sizeof(hand_foot));
	for (int j = 1; j < 3; j++)
	{
		int Ty = -1, sign = 0;
		for (int i = 1; i<5; i++)
		{
			if ((Vertex[i][1]>Ty) && (!hand_foot[i - 1]))
			{
				sign = i;
				Ty = Vertex[i][1];//Y坐标最大的两个作为了脚
			}
		}
		hand_foot[sign - 1] = 1;
		foot[j - 1] = sign;
	}


	int x = 0;
	for (int i = 0; i < 4; i++) //除了脚 另外两个就作为手
	{
		if (hand_foot[i]);
		else
		{
			hand[x] = i + 1;
			hand_foot[i] = 1;
			x++;
		}
	}

}
//回头整合 优化成一个函数 下边太啰嗦了
void Cluster::DisClassify(float** Dist, int PointNum, vector<Data>src, int v0)
{
	int ClassX = 0, ClassY = 0;
	float hand_dist = 0, foot_dist = 0, head_dist = 0;
	
	for (int i = 0; i < 6; i++)
	{
		if (isHand(i))hand_dist = hand_dist + Dist[i][v0];
		if (isFoot(i))foot_dist = foot_dist + Dist[i][v0];
	}
	head_dist = 0.5*Dist[5][v0];//0.4
	hand_dist = 0.6*(hand_dist / 2.0);//0.7
	foot_dist = 0.9*(foot_dist / 2.0);

	bool *sign = new bool[PointNum];
	for (int j = 0; j < PointNum; j++)sign[j] = false;

	ofstream outfile[CLASSNUM];//定义一个数组 用来存放输出对象 简洁
	char path[128];
	for (int i = 0; i < CLASSNUM; i++)
	{
		sprintf_s(path, "%s/Class%d.txt",outputPath.c_str(), i);
		outfile[i].open(path, ios::out);
	}	
	for (int m = 0; m < PointNum; m++)
	{
		if ((Dist[hand[0]][m] < hand_dist) && (!sign[m]))savePixels(outfile[hand[0]], src[m].x, src[m].y); //outfile[hand[0]] << src[m].x << " " << src[m].y << endl;
		if ((Dist[hand[1]][m] < hand_dist) && (!sign[m]))savePixels(outfile[hand[1]], src[m].x, src[m].y); //outfile[hand[1]] << src[m].x << " " << src[m].y << endl;		
		if ((Dist[foot[0]][m] < foot_dist) && (!sign[m]))savePixels(outfile[foot[0]], src[m].x, src[m].y); //outfile[foot[0]] << src[m].x << " " << src[m].y << endl;		
		if ((Dist[foot[1]][m] < foot_dist) && (!sign[m]))savePixels(outfile[foot[1]], src[m].x, src[m].y); //outfile[foot[1]] << src[m].x << " " << src[m].y << endl;		
		if ((Dist[5][m] < head_dist) && (!sign[m]))savePixels(outfile[5], src[m].x, src[m].y); //outfile[5] << src[m].x << " " << src[m].y << endl;
		if (!sign[m])savePixels(outfile[0], src[m].x, src[m].y); //outfile[0] << src[m].x << " " << src[m].y << endl;		
		sign[m] = true;
	}
	delete sign;
	for (int i = 0; i < CLASSNUM; i++) outfile[i].close();	
}

//将属于part部位的所有像素点保存 其中x与y是种子点的坐标，同时将属于种子点的像素保存
void Cluster::savePixels(ofstream& outPart, int x, int y)
{//左上角为坐标原点
	int indexOfSeed = label[y * 640 + x];//种子点的索引 比如 第一个种子 第i个种子
	int size = pixelBelongToSeedX[indexOfSeed].size();
	for (int i = 0; i < size;i++)
	{
		outPart << pixelBelongToSeedX[indexOfSeed][i] << " "
			    << pixelBelongToSeedY[indexOfSeed][i] << endl;
	}
}
void Cluster::Classify(float** Dist, int PointNum, vector<Data>src)
{
	//应该在这里边想办法 将种子点周围像素保存
	ofstream outfile[CLASSNUM];//定义一个数组 用来存放输出对象 简洁
	char path[128];
	for (int i = 0; i < CLASSNUM; i++)
	{
		sprintf_s(path, "output/Class%d.txt", i);
		outfile[i].open(path, ios::out);
	}

	int   Class = 0;
	float MinNum = 10000, Nummin;
	float temp;

	for (int k = 0; k < PointNum; k++)
	{
		Nummin = Dist[0][k];
		for (int g = 0; g < 6; g++)/////////判断点离哪个肢端点最近
		{
			if (isHand(g))
				temp = Dist[g][k];
			else if (isFoot(g))
				temp = Dist[g][k];//////////////////xiugai
			//else if(g==5)//实际数据用
			//	temp=0.65*Dist[g][k];/////////////////
			else temp = Dist[g][k];

			if (temp <= Nummin){
				Nummin = temp;
				Class = g;
			}
		}
		if (Dist[Class][k] < MinNum)savePixels(outfile[Class], src[k].x, src[k].y);		
	}
	for (int i = 0; i < CLASSNUM; i++) outfile[i].close();	
}
