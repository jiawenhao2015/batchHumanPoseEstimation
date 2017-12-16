#include "Kmeans.h"

Kmeans::Kmeans(int m, int sampling)
{
	k = m;
	downSampling = sampling;
}
//待优化 不需要求出 sqrt的值 浪费时间
float Kmeans::getDistXY(CvPoint2D32f a, CvPoint2D32f b)
{//计算两个元组之间的欧几里距离
	//return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}
int Kmeans::clusterOfPoint(CvPoint2D32f means[], CvPoint2D32f newPoint)
{//根据质心，决定当前元组属于哪个簇
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
{//获取当前簇的均值（质心）
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
{//获得给定簇集的平方误差
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
//初始聚类点 应该影响效率  尝试更改

void Kmeans::K_solve(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num)
{
	vector<CvPoint2D32f>clusters[3];
	CvPoint2D32f means[3];
	int i = 0;
	//默认前k个元组初始化k个簇的质心-均值
	for (i = 0; i<k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
		cout << means[i].x << means[i].y << endl;
	}
	int lable = 0;//根据默认的质心给簇赋值
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}

	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止  
	{
		for (i = 0; i<k; i++) //更新每个簇的中心点  
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //计算新的准则函数值  
		for (i = 0; i<k; i++) //清空每个簇  
		{
			clusters[i].clear();
		}
		//根据新的质心获得新的簇  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}
	//聚类结果需要微调，聚类中心点不一定落在类内的像素上              
	for (int m = 0; m<k; m++)
	{
		int x = means[m].x;
		int y = means[m].y;
		if (x % 2 == 0);
		else x += 1;
		if (y % 2 == 0);
		else y += 1;

		bool sign = false;

		//程序有问题需要修改，这是一个bug
		/*
		师兄的程序 只找了当前点的4邻域  更新测略是找了上下左右4个像素 如果不满足 再往外拓展一圈 也是上下左右
		呈十字状 但是如果位于左上角永远不能找到。
		更改策略找八邻域
		*/
		while (sign == false)
		{
			vector<CvPoint> neighbors = getNeighbors(x, y);//得到八邻域
			cout << x << " " << y << endl;
			//首先判断是否在类内
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
			//没在类内 去八邻域找
			for (int jishu = 0; jishu<neighbors.size(); jishu++)
			{
				for (int n = 0; (n<clusters[m].size()) && (sign == false); n++)
				{
					if ((neighbors[jishu].x == int(clusters[m][n].x)) && ((int(clusters[m][n].y)) == neighbors[jishu].y))
					{
						sign = true;
						means[m].x = neighbors[jishu].x;
						means[m].y = neighbors[jishu].y;
						cout << "在8邻域内" << endl;
						break;
					}
				}
			}
			if (sign == false)
			{
				cout << "8邻域也没有找到" << endl;
				x = x - 2; ///先向左移动试试	
			}
		}
		//将聚类结果写入文件  ***************************师兄之前的for(int n=0;n<4;n++)
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
//针对下采样的k-means
void Kmeans::K_solve(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag)
{
	vector<CvPoint2D32f>clusters[3];
	CvPoint2D32f means[3];
	int i = 0;
	//默认前k个元组初始化k个簇的质心-均值
	for (i = 0; i<k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
		cout << means[i].x << means[i].y << endl;
	}
	int lable = 0;//根据默认的质心给簇赋值
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止  
	{
		for (i = 0; i<k; i++) //更新每个簇的中心点
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //计算新的准则函数值  
		for (i = 0; i<k; i++) //清空每个簇  
		{
			clusters[i].clear();
		}
		//根据新的质心获得新的簇  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}
	//聚类结果需要微调，聚类中心点不一定落在类内的像素上              
	for (int m = 0; m<k; m++)
	{
		int x = means[m].x;
		int y = means[m].y;

		x = x - (x % downSampling);
		y = y - (y % downSampling);

		bool sign = false;

		//程序有问题需要修改，这是一个bug
		/*
		师兄的程序 只找了当前点的4邻域  更新测略是找了上下左右4个像素 如果不满足 再往外拓展一圈 也是上下左右
		呈十字状 但是如果位于左上角永远不能找到。
		更改策略找八邻域
		*/
		while (sign == false)
		{
			vector<CvPoint> neighbors = getNeighbors(x, y);//得到八邻域
			cout << x << " " << y << endl;
			//首先判断是否在类内
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
			//没在类内 去八邻域找
			for (int jishu = 0; jishu<neighbors.size(); jishu++)
			{
				for (int n = 0; (n<clusters[m].size()) && (sign == false); n++)
				{
					if ((neighbors[jishu].x == int(clusters[m][n].x)) && ((int(clusters[m][n].y)) == neighbors[jishu].y))
					{
						sign = true;
						means[m].x = neighbors[jishu].x;
						means[m].y = neighbors[jishu].y;
						cout << "在8邻域内" << endl;
						break;
					}
				}
			}
			if (sign == false)
			{
				cout << "8邻域也没有找到" << endl;
				x = x - downSampling; ///先向左移动试试	
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
//kmeans 超像素版本
void Kmeans::K_solveSuperPixel(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag)
{
	vector<CvPoint2D32f>clusters[3];
	CvPoint2D32f means[3];
	int i = 0;
	//默认前k个元组初始化k个簇的质心-均值
	for (i = 0; i<k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
	}
	int lable = 0;//根据默认的质心给簇赋值
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止  
	{
		for (i = 0; i<k; i++) //更新每个簇的中心点
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //计算新的准则函数值  
		for (i = 0; i<k; i++) //清空每个簇  
		{
			clusters[i].clear();
		}
		//根据新的质心获得新的簇  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}

	//聚类中心点不一定落在 超像素上   暂时不处理           
	
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
//尝试聚更多类
void Kmeans::K_solveSuperPixel4(vector<CvPoint2D32f>tuples, int class_k, CvPoint2D32f gujia[], const int Num, bool flag)
{
	vector<CvPoint2D32f>clusters[4];
	CvPoint2D32f means[4];
	int i = 0;
	//默认前k个元组初始化k个簇的质心-均值
	for (i = 0; i < k; i++)
	{
		means[i].x = tuples[i].x;
		means[i].y = tuples[i].y;
	}
	int lable = 0;//根据默认的质心给簇赋值
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 1) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止  
	{
		for (i = 0; i < k; i++) //更新每个簇的中心点
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //计算新的准则函数值  
		for (i = 0; i < k; i++) //清空每个簇  
		{
			clusters[i].clear();
		}
		//根据新的质心获得新的簇  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
	}

	//聚类中心点不一定落在 超像素上   暂时不处理           

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

	//前k个元组初始化k个簇的质心-均值
	for (i = 0; i<20; i++)
	{
		int j = rand() % (tuples.size());
		means[i].x = tuples[j].x;
		means[i].y = tuples[j].y;
	}


	int lable = 0;//根据默认的质心给簇赋值
	for (i = 0; i != tuples.size(); ++i)
	{
		lable = clusterOfPoint(means, tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}

	float oldVar = -1;
	float newVar = getVar(clusters, means);
	while (abs(newVar - oldVar) >= 0.5) //当新旧函数值相差不到1即准则函数值不发生明显变化时，算法终止  1->0.5
	{
		for (i = 0; i<20; i++) //更新每个簇的中心点  
		{
			means[i] = getMeans(clusters[i]);
			//cout<<i<<"*"<<means[i].x<<"*"<<means[i].y<<endl;///////////////////////////
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //计算新的准则函数值  
		for (i = 0; i<20; i++) //清空每个簇  
		{
			clusters[i].clear();
		}
		//根据新的质心获得新的簇  
		for (i = 0; i != tuples.size(); ++i)
		{
			lable = clusterOfPoint(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}

	}





	//将聚类结果写入文件
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