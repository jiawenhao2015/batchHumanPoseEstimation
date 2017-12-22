/*
1.������������ ���ɴ�3ά���� groundtruth
2.����������̬ʶ�������
*/

#include <iostream>
#include <iomanip>
#include<string>
#include <vector>
#include <set>
#include <fstream>
#include<map>
#include <sstream>
#include "common.h"
#include"highgui.h"

#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
using namespace std;
using namespace cv;



map<int, int>indexmp;//��¼ѵ���������֡������ ��Ӧʵ�ʵ�Ӳ���ϵ�λ�� ����ѵ���������0������Ӳ���ϵ�һ����̬��
map<int, int>indexmptest;//��¼ѵ���������֡������

//����һ��txt groundtruth 3ά����� ����һ������
//2017.11.27
void ReadmidGT(string matrixPath, vector<vector<float>>& vec)
{
	ifstream matrixFile(matrixPath);	
	float a, b, c;

	vector<float>aVec, bVec, cVec;
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> a >> b >> c;			
			aVec.push_back(a), bVec.push_back(b), cVec.push_back(c);
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << matrixPath.c_str() << endl;
		return;
	}
	 aVec.pop_back(), bVec.pop_back(), cVec.pop_back();
	
	vector<float>mpvec;
	for (int i = 0; i < aVec.size(); i++)
	{		
		mpvec.push_back(aVec[i]), mpvec.push_back(bVec[i]), mpvec.push_back(cVec[i]);
		vec.push_back(mpvec);
		mpvec.clear();
	}
	return;
}

vector<string> JOINTS = { "hip", "chest", "neck", "lShldr", "lForeArm", "lHand", "rShldr", "rForeArm", "rHand", "lThigh", "lShin", "lFoot", "rThigh", "rShin", "rFoot" };

//дһ��groundtruth
void Write2GT(string matrixPath, vector<vector<int>>& vec)
{
	ofstream matrixFile(matrixPath);
	
	 
	for (int i = 0; i < vec.size();i++)
	{
		matrixFile << vec[i][0] << " " << vec[i][1] << " " << "0 " << JOINTS[i] << endl;
	}
	matrixFile.close();
	return;
}

//д����������
//20171217
void Write2Fea(string matrixPath, vector<vector<int>>& vec)
{
	ofstream matrixFile(matrixPath);

	for (int i = 0; i < vec.size(); i++)
	{
		matrixFile << vec[i][0] << " " << vec[i][1] <<endl;
	}
	matrixFile.close();
	return;
}

//����һ��txt groundtruth ��ά����� ����һ������
//2017.11.27
void Read2GT(string matrixPath,vector<vector<int>>& vec)
{
	ifstream matrixFile(matrixPath);
	int x,y,t;
	string joint;
	 
	vector<int>xVec;//������������
	vector<int>yVec;//������������
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> x>>y>>t>>joint;
			xVec.push_back(x);
			yVec.push_back(y);
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << matrixPath.c_str() << endl;
		return;
	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);
	 
	vector<int>temp;
	for (int i = 0; i < xVec.size(); i++)
	{		
		temp.push_back(xVec[i]);
		temp.push_back(yVec[i]);
		vec.push_back(temp);
		temp.clear();
	}
	return;
}



//����һ��txt featurepoint  ���������� ��ά�����   ����һ������
//2017.12.01
void Read2Fea(string matrixPath, vector<vector<int>>& vec)
{
	ifstream matrixFile(matrixPath);
	int x, y;
	
	vector<int>xVec;//������������
	vector<int>yVec;//������������
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> x >> y;
			xVec.push_back(x);
			yVec.push_back(y);
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << matrixPath.c_str() << endl;
		return;
	}
	xVec.erase(xVec.end() - 1);
	yVec.erase(yVec.end() - 1);

	vector<int>temp;
	for (int i = 0; i < xVec.size(); i++)
	{
		temp.push_back(xVec[i]);
		temp.push_back(yVec[i]);
		vec.push_back(temp);
		temp.clear();
	}
	return;
}

//����һ��txt groundtruth ��ά����� ����һ��map ��ά�����Ӧ��3ά����
//2017.11.27
void Read3GT(string matrixPath, map<vector<int>, vector<float>>& mp)
{
	ifstream matrixFile(matrixPath);
	int x, y;
	float a, b, c;	 

	vector<int>xVec,yVec;
	vector<float>aVec,bVec,cVec;
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> x >> y >> a >> b>>c;
			xVec.push_back(x),yVec.push_back(y);
			aVec.push_back(a), bVec.push_back(b), cVec.push_back(c);
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << matrixPath.c_str() << endl;
		return;
	}
	xVec.pop_back(), yVec.pop_back(), aVec.pop_back(), bVec.pop_back(), cVec.pop_back();
	
	vector<int>temp;
	vector<float>mpvec;
	for (int i = 0; i < xVec.size(); i++)
	{
		temp.push_back(xVec[i]),temp.push_back(yVec[i]);
		mpvec.push_back(aVec[i]), mpvec.push_back(bVec[i]), mpvec.push_back(cVec[i]);
		mp[temp] = mpvec;
		temp.clear(),mpvec.clear();
	}
	return;
}
//����2άͼ������ ����5άgroundtruth ���Ҷ�Ӧ3ά�ռ����ꡣ�õ�3άgroundtruth
//2017.11.27
int get3dGT(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd)
{	
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int people = peopleBegin; people <= peopleEnd; people++)
		{
			for (int index = indexBegin; index <= indexEnd; index++)
			{
				if (action == 8 || action == 9){ if (index >= 200)continue; }
				vector<vector<int>> gt2;
				map <vector<int>, vector<float>> mp;
				stringstream ss1,ss2,ss3;

				ss1 << prefix << "\\action" << action << "\\people" << people << "\\groundTruth" << index << ".txt";
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
				ss3 << prefix << "\\action" << action << "\\people" << people << "\\groundTruth3d" << index << ".txt";

				string p1 = ss1.str();
				string p2 = ss2.str();
				string p3 = ss3.str();


				cout << p1 << endl;
				Read2GT(p1, gt2);
				Read3GT(p2, mp);

				ofstream of(p3);
				for (int i = 0; i < gt2.size(); i++)
				{
					vector<int>tt = gt2[i];
					vector<float> gt3 = mp[tt];
					if (gt3.size() == 0)//������ڵ�������˵���ؽڵ㲻��������
					{
						cout<<action<<"-----"<<people<<"-----"<< i << "------------����������!" << endl;

						tt[0] += 2;//����ƽ��2������
						gt3 = mp[tt];
						if (gt3.size() == 0)
						{
							tt[0] -= 4;//���о�������������
							gt3 = mp[tt];
						}
					}
					for (int j = 0; j < gt3.size(); j++)
					{
						of << gt3[j] << " ";
					}
					of << endl;
				}
				of.close();
			}
		}
	}
	system("PAUSE");
	return 0;
}



//����2άͼ������ ����5ά���� ���Ҷ�Ӧ3ά�ռ����ꡣ�õ���������3ά�ռ��е�λ��
//2017.12.01   17.12.16���һ������������������ά��
int get3dFea(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd,
	int dim=3)
{
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";
	string output = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int people = peopleBegin; people <= peopleEnd; people++)
		{
			for (int index = indexBegin; index <= indexEnd; index++)
			{
				vector<vector<int>> gt2;
				map <vector<int>, vector<float>> mp;
				stringstream ss1, ss2, ss3;

				if (dim == 4)
				{
					ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";
					ss2 << output << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
					ss3 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePoints3d.txt";
				}
				else//dim==3
				{
					ss1 << prefix << "\\action" << action << "\\people" << people << "\\frame" << index << "\\featurePoints.txt";
					ss2 << output << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
					ss3 << prefix << "\\action" << action << "\\people" << people << "\\frame" << index << "\\featurePoints3d.txt";
				}

				string p1 = ss1.str();
				string p2 = ss2.str();
				string p3 = ss3.str();

				cout << p1 << endl;
				Read2Fea(p1, gt2);
				Read3GT(p2, mp);

				ofstream of(p3);
				for (int i = 0; i < gt2.size(); i++)
				{
					vector<int>tt = gt2[i];
					vector<float> gt3 = mp[tt];

					if (gt3.size()==0)//������ڵ�������˵�� �����㲻��������
					{
						cout << i << "------------����������!" << endl;
						
						tt[0] += 2;//����ƽ��2������
						gt3 = mp[tt];
						if (gt3.size()==0)
						{
							tt[0] -= 4;//���о�������������
							gt3 = mp[tt];
						}
					}					
					for (int j = 0; j < gt3.size(); j++)
					{
						of << gt3[j] << " ";
					}
					
					of << endl;
				}
				of.close();
			}
		}
	}
	system("PAUSE");
	return 0;
}



//����groundtruth��̬ʶ������  һ��20*3=60ά
//���20�� ǰ3ά��3ά���� ���һ��label

void  creatGroundTruthFeature(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd)
{
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_feature.txt";//���һά�Ǳ�ǩ
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int people = peopleBegin; people <= peopleEnd; people++)
		{
			if (people == 3) continue;
			for (int index = indexBegin; index <= indexEnd; index++)
			{
				if (action == 8 || action == 9){ if (index >= 200)continue; }

				vector<vector<float>> gt2;				
				stringstream  ss2, ss3;

				ss3 << prefix << "\\action" << action << "\\people" << people << "\\groundTruth3d" << index << ".txt";
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\3dfeature" << index << ".txt";

				string p3 = ss3.str();
				string p2 = ss2.str();				

				cout << p3 << endl;
				ReadmidGT(p3, gt2);
				
				ofstream of(p2);
				int label = action;
				for (int i = 0; i < XUNI_line_Num;i++)
				{
					of << gt2[xuni_line[i * 2]][0] - gt2[xuni_line[i * 2 + 1]][0] << " "
						<< gt2[xuni_line[i * 2]][1] - gt2[xuni_line[i * 2 + 1]][1] << " "
						<< gt2[xuni_line[i * 2]][2] - gt2[xuni_line[i * 2 + 1]][2] << " ";

					fea << gt2[xuni_line[i * 2]][0] - gt2[xuni_line[i * 2 + 1]][0] << " "
						<< gt2[xuni_line[i * 2]][1] - gt2[xuni_line[i * 2 + 1]][1] << " "
						<< gt2[xuni_line[i * 2]][2] - gt2[xuni_line[i * 2 + 1]][2] << " ";
						
				}
				of <<label<< endl;
				fea << label << endl;
				of.close();
			}
		}		
	}

	fea.close();
}

//��������������   ��̬ʶ������  һ��22*3=66ά
//���22�� ǰ3ά��3ά���� ���һ��label
//20171128
//
void  creatClusterFeature(int actionBegin, int actionEnd, 
	int peopleBegin, int peopleEnd,
	int indexBegin,int indexEnd,
	int dim=3)
{
	//string prefix = "E:\\laboratory\\dataset\\synthesisdata\\finalskeletonFEATUREPOINT\\results";
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin<<"-"<<actionEnd << "bsm_all_feature4.txt";//���һά�Ǳ�ǩ
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int people = peopleBegin; people <= peopleEnd; people++)
		{
			if (people == 3) continue;
			for (int index = indexBegin; index <= indexEnd; index++)
			{
				if (action == 8 || action == 9){ if (index >= 200)continue; }

				vector<vector<float>> gt2;
				stringstream  ss2, ss3;

				ss3 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePoints3d.txt";				
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index <<"\\3dfeature.txt";

				string p3 = ss3.str();
				string p2 = ss2.str();

				cout << p3 << endl;
				ReadmidGT(p3, gt2);

				ofstream of(p2);
				int label = action;

				if (dim == 3)
				{
					for (int i = 0; i < JULEI_line_Num; i++)
					{
						of << gt2[julei_line[i * 2]][0] - gt2[julei_line[i * 2 + 1]][0] << " "
							<< gt2[julei_line[i * 2]][1] - gt2[julei_line[i * 2 + 1]][1] << " "
							<< gt2[julei_line[i * 2]][2] - gt2[julei_line[i * 2 + 1]][2] << endl;

						fea << gt2[julei_line[i * 2]][0] - gt2[julei_line[i * 2 + 1]][0] << " "
							<< gt2[julei_line[i * 2]][1] - gt2[julei_line[i * 2 + 1]][1] << " "
							<< gt2[julei_line[i * 2]][2] - gt2[julei_line[i * 2 + 1]][2] << " ";
					}
				}
				if (dim ==4)
				{
					for (int i = 0; i < JULEI4_line_Num; i++)
					{
						of << gt2[julei4_line[i * 2]][0] - gt2[julei4_line[i * 2 + 1]][0] << " "
							<< gt2[julei4_line[i * 2]][1] - gt2[julei4_line[i * 2 + 1]][1] << " "
							<< gt2[julei4_line[i * 2]][2] - gt2[julei4_line[i * 2 + 1]][2] << endl;

						fea << gt2[julei4_line[i * 2]][0] - gt2[julei4_line[i * 2 + 1]][0] << " "
							<< gt2[julei4_line[i * 2]][1] - gt2[julei4_line[i * 2 + 1]][1] << " "
							<< gt2[julei4_line[i * 2]][2] - gt2[julei4_line[i * 2 + 1]][2] << " ";
					}
				}
				of << label << endl;
				fea << label << endl;
				of.close();
			}
		}
	}
	fea.close();
}

//��������������   ��̬ʶ������   
// ǰ3ά��3ά���� ���һ��label  3*(5+10) = 45
//20171211
//������ ÿһ����λ�������������������� ��Ϊ�ò�λ����
void  creatClusterFeature2(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd)
{
	//string prefix = "E:\\laboratory\\dataset\\synthesisdata\\finalskeletonFEATUREPOINT\\results";
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_feature2.txt";//���һά�Ǳ�ǩ
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int people = peopleBegin; people <= peopleEnd; people++)
		{
			if (people == 3) continue;
			for (int index = indexBegin; index <= indexEnd; index++)
			{
				vector<vector<float>> gt2;

				stringstream  ss2, ss3;

				ss3 << prefix << "\\action" << action << "\\people" << people << "\\frame" << index << "\\featurePoints3d.txt";
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\frame" << index << "\\3dfeature2.txt";

				string p3 = ss3.str();
				string p2 = ss2.str();

				cout << p3 << endl;
				ReadmidGT(p3, gt2);

				//���ȹ�������

				vector<vector<float>> abcdeo(6,vector<float>(3,0));//o��5����λ3ά����	�ٿ�����5��֫�ˣ���ô��	
				abcdeo[0] = gt2[0];
				for (int i = 0; i < 3;i++)
				{
					abcdeo[1][i] = (gt2[1][i] + gt2[2][i] + gt2[3][i]) / 3;
					abcdeo[2][i] = (gt2[4][i] + gt2[5][i] + gt2[6][i]) / 3;
					abcdeo[3][i] = (gt2[7][i] + gt2[8][i] + gt2[9][i]) / 3;
					abcdeo[4][i] = (gt2[10][i] + gt2[11][i] + gt2[12][i]) / 3;
					abcdeo[5][i] = (gt2[13][i] + gt2[14][i] + gt2[15][i]) / 3;
				}
				
				for (int i = 1; i <=5;i++)
				{
					for (int j = 0; j < 3;j++)
					{
						fea<< abcdeo[i][j] - abcdeo[0][j]<<" ";//4֫��ͷ �����ĵ�ľ��� 
					}
				}				 
				for (int i = 1; i <= 5;i++)
				{
					for (int j = i + 1; j <= 5; j++)
					{
						for (int k = 0; k < 3;k++)
						{
							fea << abcdeo[i][k] - abcdeo[j][k] << " ";
						}
					}
				}
				
				int label = action;
				
				fea << label << endl;
				
			}
		}
	}
	fea.close();
}


//��ȡһ��txt ����һ��m*n�ľ��� ѵ�������������label ���һ��������label
Mat InitMat(string matrixPath, int m, int n,bool containlabel,int& label)
{
	ifstream matrixFile(matrixPath);
	float temp;
	Mat mat(m, n, CV_32F);
	vector<float>xVec;//������������
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> temp;
			xVec.push_back(temp);
		}
	}
	else
	{
		cout << "���ܴ��ļ���" << matrixPath.c_str() << endl;
	}

	if (containlabel)
	{
		label = xVec.back();
		xVec.pop_back();//���һ��������label		
	}
	
	xVec.pop_back();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			mat.at<float>(i, j) = xVec[i * n + j];
		}
	}
	return mat;
}



//����������һ��
//20171201
void Normalization(Mat& mat)
{
	int m = mat.rows, n = mat.cols;
	float maxNum = 0.0, minNum = 9999.0;
	
	normalize(mat, mat, 1.0, 0.0, NORM_MINMAX);
}

//knn���� 
//����һ֡����֡ ȥѵ�������������
/*
1��������֪������ݼ��ϻ��ܵĵ��뵱ǰ��ľ���
2�����վ��������������
3��ѡȡ�뵱ǰ����������K����
4��ȷ�����������ǰK�����������ĳ���Ƶ��
5�����ؾ��������ǰK������Ƶ����ߵ������Ϊ��ǰ���Ԥ�����
*/
//test mat ������ 1*60�Ļ���1*n��   matrix��Ҫƽ�������������Ͼ������
//20171129
int knn(vector<Mat>&trainSample, vector<int>&trainLabel, Mat &test,int testindex, 
	Mat& matrix, int k, string prefix,int actionBegin,int actionEnd)
{
	int label,n = trainSample.size();
	map<float, vector<int>>mp;//��¼������ѵ���������� �����С��������

	ofstream of(prefix+"\\"+to_string(actionBegin)+"-"+to_string(actionEnd)+"distance4.txt");

	for (int i = 0; i < n;i++)
	{
		Mat diff = test - trainSample[i];		

		Mat dis = diff * matrix * matrix.t() * diff.t();//��������

		cout << i << "����---" <<" "<< dis << indexmp[i] << endl;
		float distance = dis.at<float>(0, 0);

		of << distance << " ";
		mp[distance].push_back(i);
	}
	of << endl;
	of.close();


	cout << "����֡������"<<indexmptest[testindex] << endl;

	//���ǰk��������С��֡
	for (auto it = mp.begin(); it != mp.end() && k > 0; it++)
	{
		for (int j = 0; j < it->second.size() && k>0; j++)
		{
			cout << indexmp[it->second[j]] << endl;
			k--;
		}		
	}

	map<int,int>testLabel;//ͳ��label���ִ��� 
	for (auto it = mp.begin(); it != mp.end() && k>0;it++)
	{
		for (int j = 0; j < it->second.size() && k>0; j++)
		{
			testLabel[trainLabel[it->second[j]]]++;
			k--;
		}
	}
	int temp = 0;//�ҵ����ִ�������label��Ϊ���Եı�ǩ
	for (auto it = testLabel.begin(); it != testLabel.end();it++)
	{
		if (temp < it->second)
		{
			label = it->first;
			temp = it->second;
		}
	}
	return label;
}
//��ȡknnѵ������������ bool isjulei�����Ƿ��Ǿ���� ��Ϊ��·����һ��
//20171130
void getTrainAndTestData(vector<Mat>& trainSample, vector<Mat>& testSample,
	vector<int>& trainLabel, vector<int>& testLabel,
	string prefix,int row,int col,bool isjulei,
	int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd,
	int dim=3)
{	
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int people = peopleBegin; people <= peopleEnd; people++)
		{
			if (people==3) continue;
			
			for (int index = indexBegin; index <= indexEnd; index++)
			{
				int label;
				Mat sample;
				int m = row, n = col;//��������groundtruth m = 20 , n=3
				stringstream ss;

				if (isjulei)
				{
					if(dim==3)ss << prefix << "\\action" << action << "\\people" << people << "\\frame" << index << "\\3dfeature.txt";
					else ss << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\3dfeature.txt";
				}
				else//�ؽڵ�
				{
					ss << prefix << "\\action" << action << "\\people" << people << "\\3dfeature" << index << ".txt";
				}
				
				
				string path = ss.str();
				sample = InitMat(path, m, n, true, label);
				
				normalize(sample, sample, 1.0, 0.0, NORM_MINMAX);//��һ��

				//if (index % 10 == 0)//ȡ����Ϊ����
				{
					indexmptest[testSample.size()] = action * 10000 + people * 1000 + index;

					testLabel.push_back(label);
					testSample.push_back(sample);
				}
				//else //train
				if (index % 10 == 0)//testȫȡ  ��ȡһ֡��Ϊ���ԣ�������train�е������ľ��� train��ȫȡ����ȫ����
				{

					indexmp[trainSample.size()]=action*10000+people*1000+index;

					trainLabel.push_back(label);
					trainSample.push_back(sample);
				}
				//cout << sample << endl;
				//cout << label << endl;
			}
		}
	}
}

//����һ��knn�Ƿ���ͨ ����ȷ 
//20171130
void testknn(bool isjulei, int k, int startindex,
	int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd,
	int dim=3)
{
	vector<Mat> trainSample, testSample;
	vector<int> trainLabel, testLabel;
	string prefix;	
	int row , col;
	
	int label,correct=0;
	Mat matrix;

	if (isjulei)//����������
	{
		prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";		
		if (dim==3)
		{
			row = 1, col = 60;//groundtruth��60=20*3��  ����������22*3=66
			matrix = InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\W_bsm7-7julei.txt", col, 5, false, label);
			getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, true, actionBegin, actionEnd, peopleBegin, peopleEnd, indexBegin, indexEnd);
		}
		if (dim == 4)
		{
			row = 1, col = 27*3;//groundtruth��60=20*3��  ����������22*3=66 
			matrix = InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\W_bsm7-7julei4.txt", col, 5, false, label);
			getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, true, actionBegin, actionEnd, peopleBegin, peopleEnd, indexBegin, indexEnd,4);
		}		
	}
	else
	{
		prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";
		row = 1, col = 60;
		matrix = InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\W_bsm7-7guanjie.txt", col, 5, false, label);
		getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, false,actionBegin, actionEnd, peopleBegin, peopleEnd, indexBegin, indexEnd);
	}

	for (int i = startindex; i < testSample.size(); i++)
	{
		label = knn(trainSample, trainLabel, testSample[i],i, matrix, k,prefix,actionBegin,actionEnd);
		if (label == testLabel[i])
		{
			correct++; 
		}
	}
	cout << correct << "/" << testSample.size() << endl;
}


//У׼groundtruth ���ؽڵ�y�������
//20171206
void jiaozhun()
{
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";
	for (int action = 7; action <= 7; action++)
	{
		for (int people = 1; people <= 1; people++)
		{
			for (int index = 226; index <= 290; index++)
			{
				vector<vector<int>> gt2;				
				stringstream ss1;

				ss1 << prefix << "\\action" << action << "\\people" << people << "\\groundTruth" << index << ".txt";

				string p1 = ss1.str();

				cout << p1 << endl;
				Read2GT(p1, gt2);
				gt2[5][1] += 2;//6�Źؽڵ�ƫ�� ���µ���һ��
				Write2GT(p1, gt2);
			}
		}
	}

}
//����ͷ������������λ�� 
//20171217    81116ȱ�ü�������� 91145ȱ1������� ���Ϊ2
int minDistance(vector<int>&input,vector<vector<int>>& gt2, vector<bool>&flag)
{
//���ؾ���input����ĵ�  �±�
	float mindis = 9999999.0;
	int minIndex = 2;
	for (int i = 2; i <= 4;i++)
	{
		if (flag[i] == false)//δ����
		{
			int t = (gt2[i][0] - input[0])*(gt2[i][0] - input[0]) + (gt2[i][1] - input[1])*(gt2[i][1] - input[1]);

			if (t < mindis)
			{
				mindis = t;
				minIndex = i;
			}
		}
	}
	flag[minIndex] = true;//�ù� ���
	return minIndex;
}
void adjustClusterPoint(vector<vector<int>>& gt2)
{
	vector<vector<int>> temp = gt2;
	vector<bool>flag(5, false);
	int nearIndex=2;
	nearIndex = minDistance(gt2[8], gt2, flag);//����8�������Ϊ3
	temp[3] = gt2[nearIndex];
	
	nearIndex = minDistance(gt2[12], gt2, flag);//����12�������Ϊ4
	temp[4] = gt2[nearIndex];

	for (int i = 2; i <= 4; i++)if (flag[i] == false)nearIndex = i;
	temp[2] = gt2[nearIndex];//����һ����Ϊ2;

	gt2 = temp;
}
void adjustClusterPoint()
{
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";
	for (int action = 8; action <= 8; action++)
	{
		for (int people = 1; people <= 1; people++)
		{
			for (int index = 116; index <= 116; index++)
			{
				//if (index==145)continue;
				
				vector<vector<int>> gt2;				
				stringstream ss1;

				ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePoints.txt";

				string p1 = ss1.str();

				cout << p1 << endl;
				Read2Fea(p1, gt2);
				
				adjustClusterPoint(gt2);

				ss1.str("");
				ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";
				Write2Fea(ss1.str(), gt2);
			}
		}
	}

}


//20171221��Ҫһ���жϾ�����Ƿ��������ϵĺ���

void adjustNotOnBody(vector<vector<int>>& gt2)
{
	vector<vector<int>> temp = gt2;
	

	 
	gt2 = temp;
}
void adjustNotOnBody()
{
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";
	for (int action = 8; action <= 8; action++)
	{
		for (int people = 1; people <= 1; people++)
		{
			for (int index = 116; index <= 116; index++)
			{
				vector<vector<int>> gt2;
				stringstream ss1;

				ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";

				string p1 = ss1.str();

				cout << p1 << endl;
				Read2Fea(p1, gt2);

				adjustNotOnBody(gt2);

				ss1.str("");
				ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";
				Write2Fea(ss1.str(), gt2);
			}
		}
	}
}


int main()
{
	//adjustClusterPoint();
	//jiaozhun();
	//creatClusterFeature2(7,7,1,1,0,299);//����������� ʵ�������� ����ʹ 
	//creatClusterFeature(7, 9, 1, 1, 0, 299,4);//���һ�������Ǵ����Ǽ��������� Ĭ��3ά

//	creatGroundTruthFeature(7, 9, 1, 1, 0, 299);
	int k = 11;
	int testindex = 120;
	testknn(true, k, testindex, 7, 7, 1, 1, 0, 299,4);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
	
//	get3dFea(9, 9, 1, 1, 0, 199,4);//������3ά����  ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
//	get3dGT(9, 9, 1, 1, 0, 199);//�ؽڵ�3ά����
	system("PAUSE");
	return 0;
}
