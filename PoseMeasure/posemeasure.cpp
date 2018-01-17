/*
1.处理虚拟数据 生成纯3维坐标 groundtruth
2.构造用于姿态识别的特征
3.增加姿态特征维度。
*/


#include "posemeasure.h"
#include "common.h"

//读入一个txt groundtruth 3维坐标的 返回一个数组
//2017.11.27
void FileTool::ReadmidGT(string matrixPath, vector<vector<float>>& vec)
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
		cout << "不能打开文件！" << matrixPath.c_str() << endl;
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

//写一个groundtruth
void FileTool::Write2GT(string matrixPath, vector<vector<int>>& vec)
{
	ofstream matrixFile(matrixPath);


	for (int i = 0; i < vec.size(); i++)
	{
		matrixFile << vec[i][0] << " " << vec[i][1] << " " << "0 " << JOINTS[i] << endl;
	}
	matrixFile.close();
	return;
}

//写聚类特征点
//20171217
void FileTool::Write2Fea(string matrixPath, vector<vector<int>>& vec)
{
	ofstream matrixFile(matrixPath);

	for (int i = 0; i < vec.size(); i++)
	{
		matrixFile << vec[i][0] << " " << vec[i][1] << endl;
	}
	matrixFile.close();
	return;
}

//读入一个txt groundtruth 二维坐标的 返回一个数组
//2017.11.27
void FileTool::Read2GT(string matrixPath, vector<vector<int>>& vec)
{
	ifstream matrixFile(matrixPath);
	int x, y, t;
	string joint;

	vector<int>xVec;//保存所有坐标
	vector<int>yVec;//保存所有坐标
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> x >> y >> t >> joint;
			xVec.push_back(x);
			yVec.push_back(y);
		}
	}
	else
	{
		cout << "不能打开文件！" << matrixPath.c_str() << endl;
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

//读入一个txt featurepoint  聚类特征点 二维坐标的   返回一个数组
//2017.12.01
void FileTool::Read2Fea(string matrixPath, vector<vector<int>>& vec)
{
	ifstream matrixFile(matrixPath);
	int x, y;

	vector<int>xVec;//保存所有坐标
	vector<int>yVec;//保存所有坐标
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
		cout << "不能打开文件！" << matrixPath.c_str() << endl;
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

//读入一个txt groundtruth 五维坐标的 返回一个map 二维坐标对应的3维坐标
//2017.11.27
void FileTool::Read3GT(string matrixPath, map<vector<int>, vector<float>>& mp)
{
	ifstream matrixFile(matrixPath);
	int x, y;
	float a, b, c;

	vector<int>xVec, yVec;
	vector<float>aVec, bVec, cVec;
	if (matrixFile.is_open())
	{
		while (!matrixFile.eof())
		{
			matrixFile >> x >> y >> a >> b >> c;
			xVec.push_back(x), yVec.push_back(y);
			aVec.push_back(a), bVec.push_back(b), cVec.push_back(c);
		}
	}
	else
	{
		cout << "不能打开文件！" << matrixPath.c_str() << endl;
		return;
	}
	xVec.pop_back(), yVec.pop_back(), aVec.pop_back(), bVec.pop_back(), cVec.pop_back();

	vector<int>temp;
	vector<float>mpvec;
	for (int i = 0; i < xVec.size(); i++)
	{
		temp.push_back(xVec[i]), temp.push_back(yVec[i]);
		mpvec.push_back(aVec[i]), mpvec.push_back(bVec[i]), mpvec.push_back(cVec[i]);
		mp[temp] = mpvec;
		temp.clear(), mpvec.clear();
	}
	return;
}
//读入2维图像坐标 读入5维groundtruth 查找对应3维空间坐标。得到3维groundtruth
//2017.11.27
int PoseMeasure::get3dGT(int actionBegin, int actionEnd,
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
				stringstream ss1, ss2, ss3;

				ss1 << prefix << "\\action" << action << "\\people" << people << "\\groundTruth" << index << ".txt";
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
				ss3 << prefix << "\\action" << action << "\\people" << people << "\\groundTruth3d" << index << ".txt";

				string p1 = ss1.str();
				string p2 = ss2.str();
				string p3 = ss3.str();


				cout << p1 << endl;
				filetool.Read2GT(p1, gt2);
				filetool.Read3GT(p2, mp);

				ofstream of(p3);
				for (int i = 0; i < gt2.size(); i++)
				{
					vector<int>tt = gt2[i];
					vector<float> gt3 = mp[tt];
					if (gt3.size() == 0)//如果不在点云里面说明关节点不在人体上
					{
						cout << action << "-----" << people << "-----" << i << "------------不在人体上!" << endl;

						tt[0] += 2;//往右平移2个像素
						gt3 = mp[tt];
						if (gt3.size() == 0)
						{
							tt[0] -= 4;//不行就往左两个像素
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



//读入2维图像坐标 读入5维点云 查找对应3维空间坐标。得到特征点在3维空间中的位置
//2017.12.01   17.12.16添加一个参数，代表特征点维数
int PoseMeasure::get3dFea(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd,
	int dim)
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
				filetool.Read2Fea(p1, gt2);
				filetool.Read3GT(p2, mp);

				ofstream of(p3);
				for (int i = 0; i < gt2.size(); i++)
				{
					vector<int>tt = gt2[i];
					vector<float> gt3 = mp[tt];

					if (gt3.size() == 0)//如果不在点云里面说明 特征点不在人体上
					{
						cout << i << "------------不在人体上!" << endl;

						tt[0] += 2;//往右平移2个像素
						gt3 = mp[tt];
						if (gt3.size() == 0)
						{
							tt[0] -= 4;//不行就往左两个像素
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



//构造groundtruth姿态识别特征  一共20*3=60维
//输出20行 前3维是3维特征 最后一是label

void  PoseMeasure::creatGroundTruthFeature(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd)
{
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_feature.txt";//最后一维是标签
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
				filetool.ReadmidGT(p3, gt2);

				ofstream of(p2);
				int label = SetLabel(action, people, index);
				for (int i = 0; i < XUNI_line_Num; i++)
				{
					of << gt2[xuni_line[i * 2]][0] - gt2[xuni_line[i * 2 + 1]][0] << " "
						<< gt2[xuni_line[i * 2]][1] - gt2[xuni_line[i * 2 + 1]][1] << " "
						<< gt2[xuni_line[i * 2]][2] - gt2[xuni_line[i * 2 + 1]][2] << " ";

					fea << gt2[xuni_line[i * 2]][0] - gt2[xuni_line[i * 2 + 1]][0] << " "
						<< gt2[xuni_line[i * 2]][1] - gt2[xuni_line[i * 2 + 1]][1] << " "
						<< gt2[xuni_line[i * 2]][2] - gt2[xuni_line[i * 2 + 1]][2] << " ";

				}
				of << label << endl;
				fea << label << endl;
				of.close();
			}
		}
	}

	fea.close();
}
//构造groundtruth姿态识别特征   
// 增加点线面

void  PoseMeasure::creatGroundTruthFeatureDianxianmian(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd)
{
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_featurenew.txt";//最后一维是标签
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
				filetool.ReadmidGT(p3, gt2);

				ofstream of(p2);
				int label = SetLabel(action, people, index);
				for (int i = 0; i < XUNI_line_Num; i++)//连线距离
				{
					of << EucDis(gt2[xuni_line[i * 2]], gt2[xuni_line[i * 2 + 1]]) << " ";				
					fea << EucDis(gt2[xuni_line[i * 2]], gt2[xuni_line[i * 2 + 1]]) << " ";
				}
				for (int i = 0; i < XUNI_line_Num; i++)//连线方向
				{
					vector<float>temp(3, 0), norm;
					for (int k = 0; k < 3; k++)temp[k] = gt2[xuni_line[i * 2]][k] - gt2[xuni_line[i * 2 + 1]][k];
					norm = NormalizationUnit(temp);
					for (int k = 0; k < 3; k++){ fea << norm[k] << " ";of << norm[k] << endl; }
				}

				//点到四肢部分 直线距离
				for (int i = 0; i < guanjielianXian.size() / 2; i++)
				{
					S_Point lineEnd1(gt2[guanjielianXian[2 * i]]), lineEnd2(gt2[guanjielianXian[2 * i + 1]]);//直线两端
					S_Point pt1(gt2[guanjiexuyaoqiujulidedian[i][0]]);
					double dis1 = DistanceOfPointToLine(&lineEnd1, &lineEnd2, &pt1);					

					fea << dis1 << " "; of << dis1 << endl; 				
				}

				//面的相关信息。。 4个平面法向量 之间方向
				//首先求4个平面法向量
				vector<vector<float>>norm(4);
				for (int i = 0; i < 4; i++)
				{
					vector<vector<float>> part;//每个部位里面3个点
					for (int j = 0; j < 3; j++)
					{
						part.push_back(gt2[guanjiepart[i][j]]);
					}
					norm[i] = getPlaneNorm(part);
				}
				//求法向量之间夹角

				for (int i = 0; i < 4; i++)
				{
					for (int j = i + 1; j < 4; j++)
					{
						float angle = getTwoNormalAngle(norm[i], norm[j]);
						fea << angle << " "; of << angle << endl;
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
//构造聚类特征点的   姿态识别特征  一共22*3=66维
//输出22行 前3维是3维特征 最后一是label
//20171128
//
void  PoseMeasure::creatClusterFeature(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd,
	int dim)
{
	//string prefix = "E:\\laboratory\\dataset\\synthesisdata\\finalskeletonFEATUREPOINT\\results";
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_feature4.txt";//最后一维是标签
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
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\3dfeature.txt";

				string p3 = ss3.str();
				string p2 = ss2.str();

				cout << p3 << endl;
				filetool.ReadmidGT(p3, gt2);

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
				if (dim == 4)
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

//构造聚类特征点的   姿态识别特征   
// 前3维是3维特征 最后一是label  3*(5+10) = 45
//20171211
//新特征 每一个部位三个聚类中心在求中心 作为该部位特征
void  PoseMeasure::creatClusterFeature2(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd)
{
	//string prefix = "E:\\laboratory\\dataset\\synthesisdata\\finalskeletonFEATUREPOINT\\results";
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_feature2.txt";//最后一维是标签
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
				filetool.ReadmidGT(p3, gt2);

				//首先构造特征

				vector<vector<float>> abcdeo(6, vector<float>(3, 0));//o和5个部位3维坐标	再考虑用5个肢端！怎么样	
				abcdeo[0] = gt2[0];
				for (int i = 0; i < 3; i++)
				{
					abcdeo[1][i] = (gt2[1][i] + gt2[2][i] + gt2[3][i]) / 3;
					abcdeo[2][i] = (gt2[4][i] + gt2[5][i] + gt2[6][i]) / 3;
					abcdeo[3][i] = (gt2[7][i] + gt2[8][i] + gt2[9][i]) / 3;
					abcdeo[4][i] = (gt2[10][i] + gt2[11][i] + gt2[12][i]) / 3;
					abcdeo[5][i] = (gt2[13][i] + gt2[14][i] + gt2[15][i]) / 3;
				}

				for (int i = 1; i <= 5; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						fea << abcdeo[i][j] - abcdeo[0][j] << " ";//4肢和头 与中心点的距离 
					}
				}
				for (int i = 1; i <= 5; i++)
				{
					for (int j = i + 1; j <= 5; j++)
					{
						for (int k = 0; k < 3; k++)
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


//读取一个txt 返回一个m*n的矩阵 训练数据里面包含label 最后一个数字是label
Mat FileTool::InitMat(string matrixPath, int m, int n, bool containlabel, int& label)
{
	ifstream matrixFile(matrixPath);
	float temp;
	Mat mat(m, n, CV_32F);
	vector<float>xVec;//保存所有坐标
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
		cout << "不能打开文件！" << matrixPath.c_str() << endl;
	}

	if (containlabel)
	{
		label = xVec.back();
		xVec.pop_back();//最后一个数字是label		
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



//特征向量归一化
//20171201
void PoseMeasure::Normalization(Mat& mat)
{
	int m = mat.rows, n = mat.cols;
	float maxNum = 0.0, minNum = 9999.0;

	normalize(mat, mat, 1.0, 0.0, NORM_MINMAX);
}

//knn操作 
//读入一帧测试帧 去训练集里面求距离
/*
1、计算已知类别数据集合汇总的点与当前点的距离
2、按照距离递增次序排序
3、选取与当前点距离最近的K个点
4、确定距离最近的前K个点所在类别的出现频率
5、返回距离最近的前K个点中频率最高的类别作为当前点的预测分类
*/
//test mat 输入是 1*60的或者1*n的   matrix需要平方。。才是马氏距离矩阵
//20171129
int PoseMeasure::knn(vector<Mat>&trainSample, vector<int>&trainLabel, Mat &test, int testindex,
	Mat& matrix, int k, string prefix, int actionBegin, int actionEnd, string matrixName)
{
	int label=0, n = trainSample.size();
	map<float, vector<int>>mp;//记录距离与训练集的索引 距离从小到大排列

	map<int, float>indexMpDis;//记录索引帧对应的距离。。。


	ofstream of(prefix + "\\" + to_string(actionBegin) + "-" + to_string(actionEnd) + matrixName+"distance.txt");

	for (int i = 0; i < n; i++)
	{
		Mat diff = test - trainSample[i];

		Mat dis = diff * matrix * matrix.t() * diff.t();//不开方了

		//cout << i << "距离---" << " " << dis << indexmp[i] << endl;
		float distance = dis.at<float>(0, 0);
		if (distance == 0)continue;//距离=0说明是本身 去除
		of << distance << " ";
		mp[distance].push_back(i);//距离对应的索引
		indexMpDis[indexmp[i]] = distance;//索引对应的距离
	}
	of << endl;
	of.close();


	//将测试结果保存到txt文件里面
	ofstream indexfile("E:\\laboratory\\batchHumanPoseEstimation\\getTestResults\\index.txt", ios::app);

	cout << "测试帧索引：" << indexmptest[testindex] << endl;

	//输出前k个距离最小的帧
	int backupK = k;
	for (auto it = mp.begin(); it != mp.end() && backupK > 0; it++)
	{
		for (int j = 0; j < it->second.size() && backupK>0; j++)
		{
			cout << indexmp[it->second[j]] << "---" << indexMpDis[indexmp[it->second[j]]]
				<< "---label:" << trainLabel[it->second[j]] << endl;
			backupK--;
			indexfile << indexmp[it->second[j]] << " ";
		}
	}
	indexfile << endl;
	indexfile.close();
	map<int, int>testLabel;//统计label出现次数 
	for (auto it = mp.begin(); it != mp.end() && k > 0; it++)
	{
		for (int j = 0; j < it->second.size() && k>0; j++)
		{
			testLabel[trainLabel[it->second[j]]]++;
			k--;
		}
	}
	int temp = 0;//找到出现次数最多的label作为测试的标签
	for (auto it = testLabel.begin(); it != testLabel.end(); it++)
	{
		if (temp < it->second)
		{
			label = it->first;
			temp = it->second;
		}
	}
	return label;
}
//读取knn训练样本的数据 bool isjulei代表是否是聚类点 因为俩路径不一样
//20171130
void PoseMeasure::getTrainAndTestData(vector<Mat>& trainSample, vector<Mat>& testSample,
	vector<int>& trainLabel, vector<int>& testLabel,
	string prefix, int row, int col, bool isjulei,
	int actionBegin, int actionEnd,	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd, int jiange,int dim)
{
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int people = peopleBegin; people <= peopleEnd; people++)
		{
			if (people == 3) continue;

			for (int index = indexBegin; index <= indexEnd; index++)
			{
				if (action == 8 || action == 9){ if (index >= 200)continue; }

				int label;
				Mat sample;
				int m = row, n = col;//虚拟数据groundtruth m = 20 , n=3
				stringstream ss;

				if (isjulei)
				{
					if (dim == 3)ss << prefix << "\\action" << action << "\\people" << people << "\\frame" << index << "\\3dfeature.txt";
					else ss << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\3dfeature.txt";
				}
				else//关节点
				{
					ss << prefix << "\\action" << action << "\\people" << people << "\\3dfeature" << index << ".txt";
				}

				string path = ss.str();
				sample = filetool.InitMat(path, m, n, true, label);
				cout << path << endl;
				normalize(sample, sample, 1.0, 0.0, NORM_MINMAX);//归一化

				if (index % 3 == 0)//取样做为测试
				{
					indexmptest[testSample.size()] = action * 10000 + people * 1000 + index;

					testLabel.push_back(label);
					testSample.push_back(sample);
				}
				//else //train
				if (index % jiange == 0)//test全取  任取一帧作为测试，，求与train中的样本的距离 train不全取以免全相似
				{

					indexmp[trainSample.size()] = action * 10000 + people * 1000 + index;

					trainLabel.push_back(label);
					trainSample.push_back(sample);
				}
				//cout << sample << endl;
				//cout << label << endl;
			}
		}
	}
}

//测试一下knn是否跑通 跑正确 int jiange 代表多少帧采样 没个10帧还是5帧之类的
//20171130
void PoseMeasure::testknn(bool isjulei, int k, int startindex,
	int actionBegin, int actionEnd,int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd, string matrixName,int col,int jiange,int dim)
{
	vector<Mat> trainSample, testSample;
	vector<int> trainLabel, testLabel;
	string prefix;
	int row=1;

	int label, correct = 0;
	Mat matrix;

	if (isjulei)//聚类特征点
	{
		prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";
		if (dim == 3)
		{
			//row = 1, col = 60;//groundtruth是60=20*3列  聚类特征是22*3=66
			matrix = filetool.InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\"+ matrixName, col, 5, false, label);
			getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, true, actionBegin, actionEnd, peopleBegin, peopleEnd, indexBegin, indexEnd,jiange);
		}
		if (dim == 4)
		{
			//row = 1, col = 27 * 3;//groundtruth是60=20*3列  聚类特征是22*3=66 
			matrix = filetool.InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\" + matrixName, col, 5, false, label);
			getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, true, actionBegin, actionEnd, peopleBegin, peopleEnd, indexBegin, indexEnd, jiange, 4);
		}
	}
	else
	{
		prefix = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion";
		//row = 1, col = 60;
		matrix = filetool.InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\"+ matrixName, col, 5, false, label);
		getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, false, actionBegin, actionEnd, peopleBegin, peopleEnd, indexBegin, indexEnd, jiange);
	}


	//startindex 是指数组下标索引  现在需要转换一下 比如输入测试帧直接是图片名称而不是在数组中的下标了

	/*int indexInVec = 0;
	for (auto it = indexmptest.begin(); it != indexmptest.end(); it++)
	{
		if (it->second == startindex)
		{
			indexInVec = it->first;
		}
	}
	label = knn(trainSample, trainLabel, testSample[indexInVec], indexInVec, matrix, k, prefix, actionBegin, actionEnd, matrixName);
	cout << "label:"<<label << endl;

	if (label == testLabel[indexInVec])
	{
		correct++;
		cout << "correct!" << endl;
	}*/
	cout << "开始测试：" << endl; correct = 0;
	for (int i = 0; i < testSample.size();i++)
	{
		
		label = knn(trainSample, trainLabel, testSample[i], i, matrix, k, prefix, actionBegin, actionEnd, matrixName);
		cout << "label:" << label << endl;

		if (label == testLabel[i])
		{
			correct++;
			cout << "correct!" << endl;
		}
	}
	cout << "正确率：" << correct << "/" << testSample.size() << "=" << ((float)correct / (float)testSample.size()) << endl;

}


//校准groundtruth 将关节点y坐标更改
//20171206
void PoseMeasure::jiaozhun()
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
				filetool.Read2GT(p1, gt2);
				gt2[5][1] += 2;//6号关节点偏上 往下调整一下
				filetool.Write2GT(p1, gt2);
			}
		}
	}
}
//调整头部聚类特征点位置 
//20171217    81116缺好几个聚类点 91145缺1个聚类点 标号为2
int PoseMeasure::minDistance(vector<int>&input, vector<vector<int>>& gt2, vector<bool>&flag)
{
	//返回距离input最近的点  下标
	float mindis = 9999999.0;
	int minIndex = 2;
	for (int i = 2; i <= 4; i++)
	{
		if (flag[i] == false)//未更改
		{
			int t = (gt2[i][0] - input[0])*(gt2[i][0] - input[0]) + (gt2[i][1] - input[1])*(gt2[i][1] - input[1]);

			if (t < mindis)
			{
				mindis = t;
				minIndex = i;
			}
		}
	}
	flag[minIndex] = true;//用过 标记
	return minIndex;
}
void PoseMeasure::adjustClusterPoint(vector<vector<int>>& gt2)
{
	vector<vector<int>> temp = gt2;
	vector<bool>flag(5, false);
	int nearIndex = 2;
	nearIndex = minDistance(gt2[8], gt2, flag);//距离8最近的作为3
	temp[3] = gt2[nearIndex];

	nearIndex = minDistance(gt2[12], gt2, flag);//距离12最近的作为4
	temp[4] = gt2[nearIndex];

	for (int i = 2; i <= 4; i++)if (flag[i] == false)nearIndex = i;
	temp[2] = gt2[nearIndex];//另外一个作为2;

	gt2 = temp;
}
void PoseMeasure::adjustClusterPoint()
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
				filetool.Read2Fea(p1, gt2);

				adjustClusterPoint(gt2);

				ss1.str("");
				ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";
				filetool.Write2Fea(ss1.str(), gt2);
			}
		}
	}

}


//20171221需要一个判断聚类点是否在人体上的函数

void PoseMeasure::adjustNotOnBody(vector<vector<int>>& gt2)
{
	vector<vector<int>> temp = gt2;



	gt2 = temp;
}
void PoseMeasure::adjustNotOnBody()
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
				filetool.Read2Fea(p1, gt2);

				adjustNotOnBody(gt2);

				ss1.str("");
				ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";
				filetool.Write2Fea(ss1.str(), gt2);
			}
		}
	}
}


//求两个三维点欧式距离
float PoseMeasure::EucDis(vector<float>&a, vector<float>&b)
{
	float ans =0;
	for (int i = 0; i < 3;i++)ans += abs(a[i] - b[i])*abs(a[i] - b[i]);	
	return ans;
}
//单位化向量
vector<float>  PoseMeasure::NormalizationUnit(vector<float>&a)
{
	vector<float>ret(3,0);
	float length = 0;
	for (int i = 0; i < 3;i++)length += a[i] * a[i];
	for (int i = 0; i < 3; i++)ret[i] = a[i] / sqrt(length);	
	return ret;
}
//构造聚类特征点的   姿态识别特征   
// 前3维是3维特征 最后一是label   
//20171227
//构造聚类特征点的   姿态识别特征  一共22*3=66 +  =   维
// 前3维是3维特征 最后一个数是label

//每一对聚类点之间距离 每一对聚类点之间方向
void  PoseMeasure::creatClusterFeature3(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd,
	int dim)
{
	int count = 0;
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_featureNew.txt";//最后一维是标签
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
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\3dfeature.txt";
				string p3 = ss3.str();
				string p2 = ss2.str();

				cout << p3 << endl;
				filetool.ReadmidGT(p3, gt2);

				ofstream of(p2);
				int label = action;

				if (dim == 3)
				{
					for (int i = 0; i < JULEI_line_Num; i++)
					{
						fea << gt2[julei_line[i * 2]][0] - gt2[julei_line[i * 2 + 1]][0] << " "
							<< gt2[julei_line[i * 2]][1] - gt2[julei_line[i * 2 + 1]][1] << " "
							<< gt2[julei_line[i * 2]][2] - gt2[julei_line[i * 2 + 1]][2] << " ";
						
						of << gt2[julei_line[i * 2]][0] - gt2[julei_line[i * 2 + 1]][0] << " "
							<< gt2[julei_line[i * 2]][1] - gt2[julei_line[i * 2 + 1]][1] << " "
							<< gt2[julei_line[i * 2]][2] - gt2[julei_line[i * 2 + 1]][2] << endl;
					}
				}
				if (dim == 4)
				{					
					for (int i = 0; i < gt2.size();i++)//每一对聚类点之间距离
					{
						for (int j = i + 1; j < gt2.size();j++)
						{
							fea << EucDis(gt2[i], gt2[j]) << " ";
							count++;
							of << EucDis(gt2[i], gt2[j]) <<endl;
						}
					}
					
					for (int i = 0; i < gt2.size(); i++ )//每一对聚类点之间方向
					{
						for (int j = i + 1; j < gt2.size(); j++)
						{
							vector<float>temp(3,0),norm;
							for (int k = 0; k < 3;k++)temp[k] = gt2[i][k] - gt2[j][k];								
							norm = NormalizationUnit(temp);
							for (int k = 0; k < 3; k++){ fea << norm[k] << " "; count++; of << norm[k] <<endl; }
						}
					}
				}
				of << label << endl;
				of.close();
				fea << label << endl;
				count++;
				//cout << "----------------------------" << count << "----------------------------" << endl;
				count = 0;
			}
		}
	}
	
	fea.close();
}


//之前求的特征 只用到了坐标差 没用到距离
//构造聚类特征点的   姿态识别特征   
// 前3维是3维特征 最后一是label   
//20171228
//构造聚类特征点的   姿态识别特征  一共22*3=66 +  =   维
// 前3维是3维特征 最后一个数是label

//关节点连线之间方向，关节点之间距离
void  PoseMeasure::creatClusterFeature4(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,
	int indexBegin, int indexEnd,
	int dim)
{
	int count = 0;
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_featureLianxian.txt";//最后一维是标签
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
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\3dfeature.txt";

				string p3 = ss3.str();
				string p2 = ss2.str();

				cout << p3 << endl;
				filetool.ReadmidGT(p3, gt2);

				ofstream of(p2);
				int label = action;

				if (dim == 3)
				{
					for (int i = 0; i < JULEI_line_Num; i++)
					{
						fea << gt2[julei_line[i * 2]][0] - gt2[julei_line[i * 2 + 1]][0] << " "
							<< gt2[julei_line[i * 2]][1] - gt2[julei_line[i * 2 + 1]][1] << " "
							<< gt2[julei_line[i * 2]][2] - gt2[julei_line[i * 2 + 1]][2] << " ";

						of << gt2[julei_line[i * 2]][0] - gt2[julei_line[i * 2 + 1]][0] << " "
							<< gt2[julei_line[i * 2]][1] - gt2[julei_line[i * 2 + 1]][1] << " "
							<< gt2[julei_line[i * 2]][2] - gt2[julei_line[i * 2 + 1]][2] << endl;
					}
				}
				if (dim == 4)
				{
					for (int i = 0; i < JULEI4_line_Num; i++)//连线距离
					{
						fea << EucDis(gt2[julei4_line[i * 2]], gt2[julei4_line[i * 2+1]]) << " ";
						count++;
						of << EucDis(gt2[julei4_line[i * 2]], gt2[julei4_line[i * 2 + 1]]) <<endl;
					}
					for (int i = 0; i < JULEI4_line_Num; i++)//连线方向
					{
						
						vector<float>temp(3, 0), norm;
						for (int k = 0; k < 3; k++)temp[k] = gt2[julei4_line[i * 2]][k] - gt2[julei4_line[i * 2 + 1]][k];
						norm = NormalizationUnit(temp);
						for (int k = 0; k < 3; k++){ fea << norm[k] << " "; count++; of << norm[k] <<endl; }
					}
				}

				fea << label << endl;
				of << label << endl;
				of.close();
				count++;
				cout << "----------------------------" << count << "----------------------------" << endl;
				count = 0;
			}
		}
	}

	fea.close();
}


//20180103
/*
假设给出空间中的三个点：A，B，C，求点C到由点A、B构成的直线的距离。
d = (AB x AC)/|AB|
|AB X AC|/2是三角形ABC的面积，这个三角形的底是|AB|，高就是C到AB的距离。
po代码（C++）,计算点到直线的距离
*/
double PoseMeasure::DistanceOfPointToLine(S_Point* a, S_Point* b, S_Point* s)
{
	double ab = sqrt(pow((a->x - b->x), 2.0) + pow((a->y - b->y), 2.0) + pow((a->z - b->z), 2.0));
	double as = sqrt(pow((a->x - s->x), 2.0) + pow((a->y - s->y), 2.0) + pow((a->z - s->z), 2.0));
	double bs = sqrt(pow((s->x - b->x), 2.0) + pow((s->y - b->y), 2.0) + pow((s->z - b->z), 2.0));
	double cos_A = (pow(as, 2.0) + pow(ab, 2.0) - pow(bs, 2.0)) / (2 * ab*as);
	double sin_A = sqrt(1 - pow(cos_A, 2.0));
	return as*sin_A*as*sin_A;//距离的平方
}
/*
结合论文，构造点线面相关的姿态特征。
20180103
*/

void  PoseMeasure::creatClusterFeatureDianxianmian(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd,int indexBegin, int indexEnd,int dim)
{
	int count = 0;
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_featureDianxianmian.txt";//最后一维是标签
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
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\3dfeature.txt";

				string p3 = ss3.str();
				string p2 = ss2.str();

				cout << p3 << endl;
				filetool.ReadmidGT(p3, gt2);

				ofstream of(p2);
				int label = SetLabel(action,people,index);

				
				if (dim == 4)
				{//关节到四肢上的关节的直线的距离
					//肢端关节连线方向
					
					for (int i = 0; i < lianXian.size()/2; i++)//连线距离
					{
						fea << EucDis(gt2[lianXian[i * 2]], gt2[lianXian[i * 2 + 1]]) << " ";
						count++;
						of << EucDis(gt2[lianXian[i * 2]], gt2[lianXian[i * 2 + 1]]) << endl;
					}
					
					for (int i = 0; i < lianXian.size() / 2; i++)//连线方向
					{
						vector<float>temp(3, 0), norm;
						for (int k = 0; k < 3; k++)temp[k] = gt2[lianXian[i * 2]][k] - gt2[lianXian[i * 2 + 1]][k];
						norm = NormalizationUnit(temp);
						for (int k = 0; k < 3; k++){ fea << norm[k] << " "; count++; of << norm[k] << endl; }
					}
					
					//点到四肢部分 直线距离
					for (int i = 0; i < zhiduanlianXian.size()/2; i++)
					{
						S_Point lineEnd1(gt2[zhiduanlianXian[2 * i]]), lineEnd2(gt2[zhiduanlianXian[2 * i+1]]);//直线两端
						S_Point pt1(gt2[xuyaoqiujulidedian[i][0]]), pt2(gt2[xuyaoqiujulidedian[i][1]]);
						double dis1 = DistanceOfPointToLine(&lineEnd1,&lineEnd2, &pt1);
						double dis2 = DistanceOfPointToLine(&lineEnd1, &lineEnd2, &pt2);

						fea << dis1 << " "; of << dis1 << endl; count++;
						fea << dis2 << " "; of << dis2 << endl; count++;
					}
					//面的相关信息。。 4个平面法向量 之间方向
					//首先求4个平面法向量
					vector<vector<float>>norm(4);
					for (int i = 0; i < 4;i++)
					{
						vector<vector<float>> part;//每个部位里面4个点
						for (int j = 0; j < 4;j++)
						{
							part.push_back(gt2[bodypart[i][j]]);
						}
						norm[i] = getPlaneNorm(part);
					}
					//求法向量之间夹角

					for (int i = 0; i < 4; i++)
					{
						for (int j = i + 1; j < 4;j++)
						{
							float angle = getTwoNormalAngle(norm[i], norm[j]);
							fea << angle << " "; of << angle << endl; count++;
						}
					}
				}

				fea << label << endl;
				of << label << endl;
				of.close();
				count++;
				cout << "----------------------------" << count << "----------------------------" << endl;
				count = 0;
			}
		}
	}
	fea.close();
}

/*
Ax+by+cz=D 拟合平面
http://blog.csdn.net/zhouyelihua/article/details/46122977
调用方法
CvMat*points_mat = cvCreateMat(X_vector.size(), 3, CV_32FC1);//定义用来存储需要拟合点的矩阵
for (int i=0;i < X_vector.size(); ++i)
{
	points_mat->data.fl[i*3+0] = X_vector[i];//矩阵的值进行初始化   X的坐标值
	points_mat->data.fl[i * 3 + 1] = Y_vector[i];//  Y的坐标值
	points_mat->data.fl[i * 3 + 2] = Z_vector[i];
}
float plane12[4] = { 0 };//定义用来储存平面参数的数组
cvFitPlane(points_mat, plane12);//调用方程
*/

void PoseMeasure::cvFitPlane(const CvMat* points, float* plane)
{
	// Estimate geometric centroid.
	int nrows = points->rows;
	int ncols = points->cols;
	int type = points->type;
	CvMat* centroid = cvCreateMat(1, ncols, type);
	cvSet(centroid, cvScalar(0));
	for (int c = 0; c < ncols; c++){
		for (int r = 0; r < nrows; r++)
		{
			centroid->data.fl[c] += points->data.fl[ncols*r + c];
		}
		centroid->data.fl[c] /= nrows;
	}
	// Subtract geometric centroid from each point.
	CvMat* points2 = cvCreateMat(nrows, ncols, type);
	for (int r = 0; r < nrows; r++)
		for (int c = 0; c < ncols; c++)
			points2->data.fl[ncols*r + c] = points->data.fl[ncols*r + c] - centroid->data.fl[c];
	// Evaluate SVD of covariance matrix.
	CvMat* A = cvCreateMat(ncols, ncols, type);
	CvMat* W = cvCreateMat(ncols, ncols, type);
	CvMat* V = cvCreateMat(ncols, ncols, type);
	cvGEMM(points2, points, 1, NULL, 0, A, CV_GEMM_A_T);
	cvSVD(A, W, NULL, V, CV_SVD_V_T);
	// Assign plane coefficients by singular vector corresponding to smallest singular value.
	plane[ncols] = 0;
	for (int c = 0; c < ncols; c++){
		plane[c] = V->data.fl[ncols*(ncols - 1) + c];
		plane[ncols] += plane[c] * centroid->data.fl[c];
	}
	// Release allocated resources.
	cvReleaseMat(&centroid);
	cvReleaseMat(&points2);
	cvReleaseMat(&A);
	cvReleaseMat(&W);
	cvReleaseMat(&V);
}

/*
20180103
返回平面的单位化法向量
输入是部位的4个点 返回这4个点拟合的平面法向量
*/
vector<float> PoseMeasure::getPlaneNorm(vector<vector<float>>&part)
{
	CvMat*points_mat = cvCreateMat(part.size(), 3, CV_32FC1);//定义用来存储需要拟合点的矩阵
	for (int i = 0; i < part.size(); ++i)
	{
		points_mat->data.fl[i * 3 + 0] = part[i][0];//矩阵的值进行初始化   X的坐标值
		points_mat->data.fl[i * 3 + 1] = part[i][1];//  Y的坐标值
		points_mat->data.fl[i * 3 + 2] = part[i][2];
	}
	float plane12[4] = { 0 };//定义用来储存平面参数的数组
	cvFitPlane(points_mat, plane12);//调用方程
	vector<float>temp(3),norm;
	temp[0] = plane12[0], temp[1] = plane12[1], temp[2] = plane12[2];
	norm = NormalizationUnit(temp);//单位化
	return norm;
}
/*
20180104
计算两个向量的夹角 弧度
*/
float PoseMeasure::getTwoNormalAngle(vector<float>&normal1, vector<float>&normal2)
{
	
	float cosTheta;
	cosTheta = abs(normal1[0] * normal2[0] + normal1[1] * normal2[1] + normal1[2] * normal2[2]) / (sqrt(normal1[0] * normal1[0] + normal1[1] * normal1[1] + normal1[2] * normal1[2])*sqrt(normal2[0] * normal2[0] + normal2[1] * normal2[1] + normal2[2] * normal2[2]));

	//if (cosTheta >= 1.0)return 0.0;	
	//cout << "cosTheta:" << cosTheta << " " << "acos(cosTheta):" << acos(cosTheta) << endl;
	 
	return cosTheta;
}
/*
20180104
将搜索的结果图拼接起来显示
*/
void FileTool::resultPicMerge(int testindex)
{
	string  path = "E:\\laboratory\\dataset\\synthesisdata\\bvhtransformdepthacquistion\\";

	int N = 22;
	vector<Mat>input(N);

	/*vector<int> picindex = { 71120, 91190, 71150, 71230, 71130, 71110, 81180, 71140, 81190, 81020, 81010,
	71120, 71130, 91110, 71210, 71140, 81020, 71220, 71200, 81190, 91120,91180 };*/
	vector<int> picindex;
	vector<float>floatindex;
	
	ReadFile("E:\\laboratory\\batchHumanPoseEstimation\\getTestResults\\index.txt", floatindex);

	for (auto f : floatindex)picindex.push_back((int)f);


	for (int i = 0; i < N; i++)
	{
		stringstream ss;
		ss << path << "action" << picindex[i] / 10000 << "\\people" << (picindex[i] / 1000) % 10 << "\\" << picindex[i] % 1000 << ".jpg";
		cout << ss.str() << endl;
		input[i] = imread(ss.str());
	}

	Size bigsize(input[0].cols * N / 2, input[0].rows * 2);//合并后图片size 宽*高

	vector<Mat>temp(N);

	Mat mergefinal;
	mergefinal.create(bigsize, CV_MAKETYPE(input[0].depth(), 3));//rgb 3通道
	mergefinal = Scalar::all(0);

	for (int i = 0; i < N; i++)
	{
		if (i<N / 2)temp[i] = mergefinal(Rect(i*input[0].cols, 0, input[0].cols, input[0].rows));
		else temp[i] = mergefinal(Rect((i - N / 2)*input[0].cols, input[0].rows, input[0].cols, input[0].rows));
		input[i].copyTo(temp[i]); //copy图片到对应位置
	}

	imshow("merge", mergefinal);
	stringstream ss;
	ss << "E:\\laboratory\\batchHumanPoseEstimation\\getTestResults\\" << testindex << "138vs108.jpg";
	imwrite(ss.str(), mergefinal);

	waitKey(0);
}
// 读取文件
bool FileTool::ReadFile(string filePath, vector<float>&errorVec)
{
	errorVec.clear();
	ifstream infile(filePath);
	float x;
	if (!infile.is_open())
	{
		cout << "不能打开文件----" << filePath << endl;
		return false;
	}
	while (!infile.eof())
	{
		infile >> x;
		errorVec.push_back(x);
	}
	errorVec.pop_back();
	return true;
}

/*
 构造点线面相关的姿态特征 增加 线与面的角度等约束
20180108
*/
void  PoseMeasure::creatClusterFeatureDianxianmianJiao(int actionBegin, int actionEnd,
	int peopleBegin, int peopleEnd, int indexBegin, int indexEnd, int dim)
{
	int count = 0;
	string prefix = "E:\\laboratory\\dataset\\synthesisdata\\mypartresults";

	stringstream  ss;
	ss << prefix << "\\" << actionBegin << "-" << actionEnd << "bsm_all_featureDianxianmian.txt";//最后一维是标签
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
				ss2 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\3dfeature.txt";

				string p3 = ss3.str(), p2 = ss2.str();

				cout << p3 << endl;
				filetool.ReadmidGT(p3, gt2);

				ofstream of(p2);
				int label = action;

				if (dim == 4)
				{//关节到四肢上的关节的直线的距离
					//肢端关节连线方向
					cout << "lianxiansize():" << lianXian.size() << endl;
					for (int i = 0; i < lianXian.size() / 2; i++)//连线距离
					{
						fea << EucDis(gt2[lianXian[i * 2]], gt2[lianXian[i * 2 + 1]]) << " ";
						count++;
						of << EucDis(gt2[lianXian[i * 2]], gt2[lianXian[i * 2 + 1]]) << endl;
					}
					cout << "-----::::::::" << count << endl;
					for (int i = 0; i < lianXian.size() / 2; i++)//连线方向
					{
						vector<float>temp(3, 0), norm;
						for (int k = 0; k < 3; k++)temp[k] = gt2[lianXian[i * 2]][k] - gt2[lianXian[i * 2 + 1]][k];
						norm = NormalizationUnit(temp);
						for (int k = 0; k < 3; k++){ fea << norm[k] << " "; count++; of << norm[k] << endl; }
					}

					cout << "-----::::::::" << count << endl;
					//点到四肢部分 直线距离
					for (int i = 0; i < zhiduanlianXian.size() / 2; i++)
					{
						S_Point lineEnd1(gt2[zhiduanlianXian[2 * i]]), lineEnd2(gt2[zhiduanlianXian[2 * i + 1]]);//直线两端
						S_Point pt1(gt2[xuyaoqiujulidedian[i][0]]), pt2(gt2[xuyaoqiujulidedian[i][1]]);
						double dis1 = DistanceOfPointToLine(&lineEnd1, &lineEnd2, &pt1);
						double dis2 = DistanceOfPointToLine(&lineEnd1, &lineEnd2, &pt2);

						fea << dis1 << " "; of << dis1 << endl; count++;
						fea << dis2 << " "; of << dis2 << endl; count++;
					}
					//面的相关信息。。 4个平面法向量 之间方向
					//首先求4个平面法向量
					vector<vector<float>>norm(4);
					for (int i = 0; i < 4; i++)
					{
						vector<vector<float>> part;//每个部位里面4个点
						for (int j = 0; j < 4; j++)
						{
							part.push_back(gt2[bodypart[i][j]]);
						}
						norm[i] = getPlaneNorm(part);
					}
					//求法向量之间夹角

					for (int i = 0; i < 4; i++)
					{
						for (int j = i + 1; j < 4; j++)
						{
							float angle = getTwoNormalAngle(norm[i], norm[j]);
							fea << angle << " "; of << angle << endl; count++;
						}
					}

				}
				fea << label << endl;
				of << label << endl;
				of.close();
				count++;
				cout << "----------------------------" << count << "----------------------------" << endl;
				count = 0;
			}
		}
	}
	fea.close();
}
/*
标定动作label 20180115
每一个区间一个动作label
*/

vector<vector<int>>startend =
{
	{ 71000 , 71040 },
	{ 71041 , 71090 },
	{ 71091 , 71150 },
	{ 71151 , 71200 },
	{ 71201 , 71240 },
	{ 71241 , 71299 },
	{ 81000, 81050 },
	{ 81051, 81100 },
	{ 81101, 81140 },
	{ 81141, 81199 },
	{91000,91070},
	{91071,91100},
	{91101,91199}
};

int PoseMeasure::SetLabel(int action, int people, int index)
{
	int ind = action * 10000 + people * 1000 + index;
	int i =  0;
	for (;i < startend.size();i++)
	{
		if (ind >= startend[i][0] && ind<= startend[i][1]) break;
	}
	return i;//索引就作为label
}
