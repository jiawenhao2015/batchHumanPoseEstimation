#include "Kmeans.h"

Kmeans::Kmeans(int m, int sampling)
{
	k = m;
	downSampling = sampling;
}
//���Ż� ����Ҫ��� sqrt��ֵ �˷�ʱ��
float Kmeans::getDistXY(CvPoint2D32f a, CvPoint2D32f b)
{//��������Ԫ��֮���ŷ�������
	//return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}
int Kmeans::clusterOfPoint(CvPoint2D32f means[], CvPoint2D32f newPoint)
{//�������ģ�������ǰԪ�������ĸ���
	float dist = getDistXY(means[0], newPoint);
	float tmp;
	int label = 0;
	for (int i = 1; i<k; i++)
	{
		tmp = getDistXY(means[i], newPoint);
		if (tmp<dist)
		{
			dist = tmp;
			label = i;
		}
	}
	return label;
}
CvPoint2D32f Kmeans::getMeans(vector<CvPoint2D32f>cluster)
{//��ȡ��ǰ�صľ�ֵ�����ģ�
	int num = cluster.size();
	double meansX = 0, meansY = 0;
	CvPoint2D32f t;
	for (int i = 0; i<num; i++)
	{
		meansX += cluster[i].x;
		meansY += cluster[i].y;
	}
	t.x = meansX / num;
	t.y = meansY / num;
	return t;
}

float Kmeans::getVar(vector<CvPoint2D32f> clusters[], CvPoint2D32f means[])
{//��ø����ؼ���ƽ�����
	float var = 0;
	for (int i = 0; i<k; i++)
	{
		vector<CvPoint2D32f> t = clusters[i];
		for (int j = 0; j<t.size(); j++)
		{
			var += getDistXY(t[j], means[i]);
		}
	}
	return var;
}

vector<CvPoint> Kmeans::getNeighbors(int x, int y)
{
	vector<CvPoint> neighbors;

	neighbors.push_back(cvPoint(x - downSampling, y - downSampling));
	neighbors.push_back(cvPoint(x, y - downSampling));
	neighbors.push_back(cvPoint(x + downSampling, y - downSampling));
	neighbors.push_back(cvPoint(x - downSampling, y));
	neighbors.push_back(cvPoint(x + downSampling, y));
	neighbors.push_back(cvPoint(x - downSampling, y + downSampling));
	neighbors.push_back(cvPoint(x, y + downSampling));
	neighbors.push_back(cvPoint(x + downSampling, y + downSampling));

	return neighbors;
}
//��ʼ����� Ӧ��Ӱ��Ч��  ���Ը���

void Kmeans::K_solve(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num)
{
	vector<CvPoint2D32f>clusters[3];
	CvPoint2D32f means[3];
	int i = 0;
	//Ĭ��ǰk��Ԫ���ʼ��k���ص�����-��ֵ
	for (i = 0; i<k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
		cout << means[i].x << means[i].y << endl;
	}
	int lable = 0;//����Ĭ�ϵ����ĸ��ظ�ֵ
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}

	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //���¾ɺ���ֵ����1��׼����ֵ���������Ա仯ʱ���㷨��ֹ  
	{
		for (i = 0; i<k; i++) //����ÿ���ص����ĵ�  
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //�����µ�׼����ֵ  
		for (i = 0; i<k; i++) //���ÿ����  
		{
			clusters[i].clear();
		}
		//�����µ����Ļ���µĴ�  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}
	//��������Ҫ΢�����������ĵ㲻һ���������ڵ�������              
	for (int m = 0; m<k; m++)
	{
		int x = means[m].x;
		int y = means[m].y;
		if (x % 2 == 0);
		else x += 1;
		if (y % 2 == 0);
		else y += 1;

		bool sign = false;

		//������������Ҫ�޸ģ�����һ��bug
		/*
		ʦ�ֵĳ��� ֻ���˵�ǰ���4����  ���²�����������������4������ ��������� ��������չһȦ Ҳ����������
		��ʮ��״ �������λ�����Ͻ���Զ�����ҵ���
		���Ĳ����Ұ�����
		*/
		while (sign == false)
		{
			vector<CvPoint> neighbors = getNeighbors(x, y);//�õ�������
			cout << x << " " << y << endl;
			//�����ж��Ƿ�������
			for (int n = 0; (n<clusters[m].size()) && (sign == false); n++)
			{
				if ((x == int(clusters[m][n].x)) && (y == int(clusters[m][n].y)))
				{
					sign = true;
					means[m].x = x;
					means[m].y = y;
					cout << "in the cluster" << endl;
					break;
				}
			}
			//û������ ȥ��������
			for (int jishu = 0; jishu<neighbors.size(); jishu++)
			{
				for (int n = 0; (n<clusters[m].size()) && (sign == false); n++)
				{
					if ((neighbors[jishu].x == int(clusters[m][n].x)) && ((int(clusters[m][n].y)) == neighbors[jishu].y))
					{
						sign = true;
						means[m].x = neighbors[jishu].x;
						means[m].y = neighbors[jishu].y;
						cout << "��8������" << endl;
						break;
					}
				}
			}
			if (sign == false)
			{
				cout << "8����Ҳû���ҵ�" << endl;
				x = x - 2; ///�������ƶ�����	
			}
		}
		//��������д���ļ�  ***************************ʦ��֮ǰ��for(int n=0;n<4;n++)
		for (int n = 0; n<3; n++)
		{
			ofstream Cwriter;
			switch (n)
			{
			case 0:{
					   switch (class_k)
					   {
					   case 0:{Cwriter.open("E:\\Pose\\results\\0\\Cluster0.txt", ios::out); break; }
					   case 1:{Cwriter.open("E:\\Pose\\results\\1\\Cluster0.txt", ios::out); break; }
					   case 2:{Cwriter.open("E:\\Pose\\results\\2\\Cluster0.txt", ios::out); break; }
					   case 3:{Cwriter.open("E:\\Pose\\results\\3\\Cluster0.txt", ios::out); break; }
					   case 4:{Cwriter.open("E:\\Pose\\results\\4\\Cluster0.txt", ios::out); break; }
					   case 5:{Cwriter.open("E:\\Pose\\results\\5\\Cluster0.txt", ios::out); break; }
					   }
					   break;
			}
			case 1:{
					   switch (class_k)
					   {
					   case 0:{Cwriter.open("E:\\Pose\\results\\0\\Cluster1.txt", ios::out); break; }
					   case 1:{Cwriter.open("E:\\Pose\\results\\1\\Cluster1.txt", ios::out); break; }
					   case 2:{Cwriter.open("E:\\Pose\\results\\2\\Cluster1.txt", ios::out); break; }
					   case 3:{Cwriter.open("E:\\Pose\\results\\3\\Cluster1.txt", ios::out); break; }
					   case 4:{Cwriter.open("E:\\Pose\\results\\4\\Cluster1.txt", ios::out); break; }
					   case 5:{Cwriter.open("E:\\Pose\\results\\5\\Cluster1.txt", ios::out); break; }
					   }
					   break;
			}
			case 2:{
					   switch (class_k)
					   {
					   case 0:{Cwriter.open("E:\\Pose\\results\\0\\Cluster2.txt", ios::out); break; }
					   case 1:{Cwriter.open("E:\\Pose\\results\\1\\Cluster2.txt", ios::out); break; }
					   case 2:{Cwriter.open("E:\\Pose\\results\\2\\Cluster2.txt", ios::out); break; }
					   case 3:{Cwriter.open("E:\\Pose\\results\\3\\Cluster2.txt", ios::out); break; }
					   case 4:{Cwriter.open("E:\\Pose\\results\\4\\Cluster2.txt", ios::out); break; }
					   case 5:{Cwriter.open("E:\\Pose\\results\\5\\Cluster2.txt", ios::out); break; }
					   }
					   break;
			}
			case 3:{
					   switch (class_k)
					   {
					   case 0:{Cwriter.open("E:\\Pose\\results\\0\\Cluster3.txt", ios::out); break; }
					   case 1:{Cwriter.open("E:\\Pose\\results\\1\\Cluster3.txt", ios::out); break; }
					   case 2:{Cwriter.open("E:\\Pose\\results\\2\\Cluster3.txt", ios::out); break; }
					   case 3:{Cwriter.open("E:\\Pose\\results\\3\\Cluster3.txt", ios::out); break; }
					   case 4:{Cwriter.open("E:\\Pose\\results\\4\\Cluster3.txt", ios::out); break; }
					   case 5:{Cwriter.open("E:\\Pose\\results\\5\\Cluster3.txt", ios::out); break; }
					   }
					   break;
			}
			}
			for (int m = 0; m<clusters[n].size(); m++)
			{
				Cwriter << clusters[n][m].x << " ";
				Cwriter << clusters[n][m].y << endl;
			}
			Cwriter.close();
		}
		////////////////////////////////////////////////////////
		for (i = 0; i<k; i++)
		{
			gujia[i].x = int(means[i].x);
			gujia[i].y = int(means[i].y);
			if ((int(gujia[i].x) % 2) != 0)
				gujia[i].x += 1;
			if ((int(gujia[i].y) % 2) != 0)
				gujia[i].y += 1;
		}//////////////////////////////////////////////////////

	}
}
//����²�����k-means
void Kmeans::K_solve(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag)
{
	vector<CvPoint2D32f>clusters[3];
	CvPoint2D32f means[3];
	int i = 0;
	//Ĭ��ǰk��Ԫ���ʼ��k���ص�����-��ֵ
	for (i = 0; i<k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
		cout << means[i].x << means[i].y << endl;
	}
	int lable = 0;//����Ĭ�ϵ����ĸ��ظ�ֵ
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //���¾ɺ���ֵ����1��׼����ֵ���������Ա仯ʱ���㷨��ֹ  
	{
		for (i = 0; i<k; i++) //����ÿ���ص����ĵ�
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //�����µ�׼����ֵ  
		for (i = 0; i<k; i++) //���ÿ����  
		{
			clusters[i].clear();
		}
		//�����µ����Ļ���µĴ�  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}
	//��������Ҫ΢�����������ĵ㲻һ���������ڵ�������              
	for (int m = 0; m<k; m++)
	{
		int x = means[m].x;
		int y = means[m].y;

		x = x - (x % downSampling);
		y = y - (y % downSampling);

		bool sign = false;

		//������������Ҫ�޸ģ�����һ��bug
		/*
		ʦ�ֵĳ��� ֻ���˵�ǰ���4����  ���²�����������������4������ ��������� ��������չһȦ Ҳ����������
		��ʮ��״ �������λ�����Ͻ���Զ�����ҵ���
		���Ĳ����Ұ�����
		*/
		while (sign == false)
		{
			vector<CvPoint> neighbors = getNeighbors(x, y);//�õ�������
			cout << x << " " << y << endl;
			//�����ж��Ƿ�������
			for (int n = 0; (n<clusters[m].size()) && (sign == false); n++)
			{
				if ((x == int(clusters[m][n].x)) && (y == int(clusters[m][n].y)))
				{
					sign = true;
					means[m].x = x;
					means[m].y = y;
					cout << "in the cluster" << endl;
					break;
				}
			}
			//û������ ȥ��������
			for (int jishu = 0; jishu<neighbors.size(); jishu++)
			{
				for (int n = 0; (n<clusters[m].size()) && (sign == false); n++)
				{
					if ((neighbors[jishu].x == int(clusters[m][n].x)) && ((int(clusters[m][n].y)) == neighbors[jishu].y))
					{
						sign = true;
						means[m].x = neighbors[jishu].x;
						means[m].y = neighbors[jishu].y;
						cout << "��8������" << endl;
						break;
					}
				}
			}
			if (sign == false)
			{
				cout << "8����Ҳû���ҵ�" << endl;
				x = x - downSampling; ///�������ƶ�����	
			}
		}		
		for (int n = 0; n<3; n++)
		{
			ofstream Cwriter;
			switch (n)
			{
				case 0:{
						   switch (class_k)
						   {
						   case 0:{Cwriter.open("E:\\Pose\\results\\0\\Cluster0.txt", ios::out); break; }
						   case 1:{Cwriter.open("E:\\Pose\\results\\1\\Cluster0.txt", ios::out); break; }
						   case 2:{Cwriter.open("E:\\Pose\\results\\2\\Cluster0.txt", ios::out); break; }
						   case 3:{Cwriter.open("E:\\Pose\\results\\3\\Cluster0.txt", ios::out); break; }
						   case 4:{Cwriter.open("E:\\Pose\\results\\4\\Cluster0.txt", ios::out); break; }
						   case 5:{Cwriter.open("E:\\Pose\\results\\5\\Cluster0.txt", ios::out); break; }
						   }
						   break;
				}
				case 1:{
						   switch (class_k)
						   {
						   case 0:{Cwriter.open("E:\\Pose\\results\\0\\Cluster1.txt", ios::out); break; }
						   case 1:{Cwriter.open("E:\\Pose\\results\\1\\Cluster1.txt", ios::out); break; }
						   case 2:{Cwriter.open("E:\\Pose\\results\\2\\Cluster1.txt", ios::out); break; }
						   case 3:{Cwriter.open("E:\\Pose\\results\\3\\Cluster1.txt", ios::out); break; }
						   case 4:{Cwriter.open("E:\\Pose\\results\\4\\Cluster1.txt", ios::out); break; }
						   case 5:{Cwriter.open("E:\\Pose\\results\\5\\Cluster1.txt", ios::out); break; }
						   }
						   break;
				}
				case 2:{
						   switch (class_k)
						   {
						   case 0:{Cwriter.open("E:\\Pose\\results\\0\\Cluster2.txt", ios::out); break; }
						   case 1:{Cwriter.open("E:\\Pose\\results\\1\\Cluster2.txt", ios::out); break; }
						   case 2:{Cwriter.open("E:\\Pose\\results\\2\\Cluster2.txt", ios::out); break; }
						   case 3:{Cwriter.open("E:\\Pose\\results\\3\\Cluster2.txt", ios::out); break; }
						   case 4:{Cwriter.open("E:\\Pose\\results\\4\\Cluster2.txt", ios::out); break; }
						   case 5:{Cwriter.open("E:\\Pose\\results\\5\\Cluster2.txt", ios::out); break; }
						   }
						   break;
				}
			}
			for (int m = 0; m<clusters[n].size(); m++)
			{
				Cwriter << clusters[n][m].x << " ";
				Cwriter << clusters[n][m].y << endl;
			}
			Cwriter.close();
		}
		////////////////////////////////////////////////////////
		for (i = 0; i<k; i++)
		{
			gujia[i].x = int(means[i].x);
			gujia[i].y = int(means[i].y);
			if ((int(gujia[i].x) % 2) != 0)
				gujia[i].x += 1;
			if ((int(gujia[i].y) % 2) != 0)
				gujia[i].y += 1;
		}
	}
}
//kmeans �����ذ汾
void Kmeans::K_solveSuperPixel(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag)
{
	vector<CvPoint2D32f>clusters[3];
	CvPoint2D32f means[3];
	int i = 0;
	//Ĭ��ǰk��Ԫ���ʼ��k���ص�����-��ֵ
	for (i = 0; i<k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
	}
	int lable = 0;//����Ĭ�ϵ����ĸ��ظ�ֵ
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //���¾ɺ���ֵ����1��׼����ֵ���������Ա仯ʱ���㷨��ֹ  
	{
		for (i = 0; i<k; i++) //����ÿ���ص����ĵ�
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //�����µ�׼����ֵ  
		for (i = 0; i<k; i++) //���ÿ����  
		{
			clusters[i].clear();
		}
		//�����µ����Ļ���µĴ�  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}

	//�������ĵ㲻һ������ ��������   ��ʱ������           
	
		for (int n = 0; n<3; n++)
		{
			ofstream Cwriter;
			char path[128];
			sprintf_s(path, "%s/%d/Cluster%d.txt",outputPath.c_str(), class_k, n);
			Cwriter.open(path, ios::out);

			for (int m = 0; m<clusters[n].size(); m++)
			{
				Cwriter << clusters[n][m].x << " ";
				Cwriter << clusters[n][m].y << endl;
			}
			Cwriter.close();
		}
		////////////////////////////////////////////////////////
		for (i = 0; i<k; i++)
		{
			gujia[i].x = int(means[i].x);
			gujia[i].y = int(means[i].y);
			if ((int(gujia[i].x) % 2) != 0)
				gujia[i].x += 1;
			if ((int(gujia[i].y) % 2) != 0)
				gujia[i].y += 1;
		}
}

//kmeans
//���Ծ۸�����
void Kmeans::K_solveSuperPixel4(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag)
{
	vector<CvPoint2D32f>clusters[4];
	CvPoint2D32f means[4];
	int i = 0;
	//Ĭ��ǰk��Ԫ���ʼ��k���ص�����-��ֵ
	for (i = 0; i < k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
	}
	int lable = 0;//����Ĭ�ϵ����ĸ��ظ�ֵ
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //���¾ɺ���ֵ����1��׼����ֵ���������Ա仯ʱ���㷨��ֹ  
	{
		for (i = 0; i < k; i++) //����ÿ���ص����ĵ�
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //�����µ�׼����ֵ  
		for (i = 0; i < k; i++) //���ÿ����  
		{
			clusters[i].clear();
		}
		//�����µ����Ļ���µĴ�  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}

	//�������ĵ㲻һ������ ��������   ��ʱ������           

	for (int n = 0; n < 4; n++)
	{
		ofstream Cwriter;
		char path[128];
		sprintf_s(path, "%s/%d/Cluster%d.txt", outputPath.c_str(), class_k, n);
		Cwriter.open(path, ios::out);

		for (int m = 0; m < clusters[n].size(); m++)
		{
			Cwriter << clusters[n][m].x << " ";
			Cwriter << clusters[n][m].y << endl;
		}
		Cwriter.close();
	}
	////////////////////////////////////////////////////////
	for (i = 0; i < k; i++)
	{
		gujia[i].x = int(means[i].x);
		gujia[i].y = int(means[i].y);
		if ((int(gujia[i].x) % 2) != 0)
			gujia[i].x += 1;
		if ((int(gujia[i].y) % 2) != 0)
			gujia[i].y += 1;
	}
}


void Kmeans::K_solve(vector<CvPoint2D32f>tuples)
{
	vector<CvPoint2D32f>clusters[20];
	CvPoint2D32f means[20];
	int i = 0;

	//ǰk��Ԫ���ʼ��k���ص�����-��ֵ
	for (i = 0; i<20; i++)
	{
		int j = rand() % (tuples.size());
		means[i].x = tuples[j].x;
		means[i].y = tuples[j].y;
	}


	int lable = 0;//����Ĭ�ϵ����ĸ��ظ�ֵ
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}

	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 0.5) //���¾ɺ���ֵ����1��׼����ֵ���������Ա仯ʱ���㷨��ֹ  1->0.5
	{
		for (i = 0; i<20; i++) //����ÿ���ص����ĵ�  
		{
			means[i] = getMeans(clusters[i]);
			//cout<<i<<"*"<<means[i].x<<"*"<<means[i].y<<endl;///////////////////////////
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //�����µ�׼����ֵ  
		for (i = 0; i<20; i++) //���ÿ����  
		{
			clusters[i].clear();
		}
		//�����µ����Ļ���µĴ�  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}

	}





	//��������д���ļ�
	for (int n = 0; n<20; n++)
	{
		ofstream Cwriter;
		switch (n)
		{
		case 0:{

				   Cwriter.open("E:\\Pose\\results\\0\\Cluster0.txt", ios::app); break;
		}
		case 1:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster1.txt", ios::app); break;
		}
		case 2:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster2.txt", ios::app); break;
		}
		case 3:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster3.txt", ios::app); break;
		}
		case 4:{

				   Cwriter.open("E:\\Pose\\results\\0\\Cluster4.txt", ios::app); break;
		}
		case 5:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster5.txt", ios::app); break;
		}
		case 6:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster6.txt", ios::app); break;
		}
		case 7:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster7.txt", ios::app); break;
		}
		case 8:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster8.txt", ios::app); break;
		}
		case 9:{
				   Cwriter.open("E:\\Pose\\results\\0\\Cluster9.txt", ios::app); break;
		}

		case 10:{

					Cwriter.open("E:\\Pose\\results\\0\\Cluster10.txt", ios::app); break;
		}
		case 11:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster11.txt", ios::app); break;
		}
		case 12:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster12.txt", ios::app); break;
		}
		case 13:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster13.txt", ios::app); break;
		}
		case 14:{

					Cwriter.open("E:\\Pose\\results\\0\\Cluster14.txt", ios::app); break;
		}
		case 15:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster15.txt", ios::app); break;
		}
		case 16:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster16.txt", ios::app); break;
		}
		case 17:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster17.txt", ios::app); break;
		}
		case 18:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster18.txt", ios::app); break;
		}
		case 19:{
					Cwriter.open("E:\\Pose\\results\\0\\Cluster19.txt", ios::app); break;
		}

		}

		for (int m = 0; m<clusters[n].size(); m++)
		{
			Cwriter << clusters[n][m].x << " ";
			Cwriter << clusters[n][m].y << endl;
		}
		Cwriter.close();
	}


}