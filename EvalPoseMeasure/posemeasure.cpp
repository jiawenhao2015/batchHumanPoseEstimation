/*
1.处理eval数据 生成纯3维坐标 groundtruth
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
			y = 240 - y;
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
//读入5维groundtruth 查找对应3维空间坐标。得到3维groundtruth
//20180119
int PoseMeasure::get3dGT(int actionBegin, int actionEnd,int indexBegin, int indexEnd)
{
	string jointprefix = "D:\\EVAL20170704\\EVAL\\joints\\";
	
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int index = indexBegin; index <= indexEnd; index++)
		{			
			stringstream ss1,ss3;						

			ss1 << jointprefix <<"joint"<< action << "_" << index << ".txt";
			ss3 << jointprefix << "joint3d" << action << "_" << index << ".txt";
		
			string p1 = ss1.str(), p3 = ss3.str();

			ifstream ifexist(p1);
			if (!ifexist.is_open())	continue;

			cout << p1 << endl;
			

			ifstream matrixFile(p1);
			int x, y;
			float a, b, c;
			
			vector<float>aVec, bVec, cVec;
			while (!matrixFile.eof())
			{
				matrixFile >> x >> y >> a >> b >> c;			
				aVec.push_back(a), bVec.push_back(b), cVec.push_back(c);
			}			
			aVec.pop_back(), bVec.pop_back(), cVec.pop_back();
			

			ofstream of(p3);
			for (int i = 0; i < aVec.size(); i++)
			{
				of << aVec[i] << " " << bVec[i] << " "<<cVec[i] <<endl;
			}
			of.close();
		}
	}
	system("PAUSE");
	return 0;
}

//读入2维图像坐标 读入5维点云 查找对应3维空间坐标。得到特征点在3维空间中的位置
//20180119
int PoseMeasure::get3dFea(int actionBegin, int actionEnd,int indexBegin, int indexEnd,int dim)
{
	string feaprefix = "D:\\EVAL20170704\\EVAL\\depth\\";
	string pointclocdprefix = "D:\\EVAL20170704\\EVAL\\depth\\";
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int index = indexBegin; index <= indexEnd; index++)
		{
			vector<vector<int>> gt2;
			map <vector<int>, vector<float>> mp;
			stringstream ss1, ss2, ss3;

			if (dim == 4)
			{
				//ss1 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePointsAdjust.txt";
				//ss2 << output << "\\action" << action << "\\people" << people << "\\output" << index << ".txt";
				//ss3 << prefix << "\\action" << action << "\\people" << people << "\\newframe" << index << "\\featurePoints3d.txt";
			}
			else//dim==3
			{
				ss1 << feaprefix << action << "\\" << index << "\\featurePoints.txt";
				ss2 << pointclocdprefix << "\\depth" << action << "_" << index << ".txt";
				ss3 << feaprefix << action << "\\" << index <<"\\featurePoints3d.txt";
			}

			string p1 = ss1.str();
			string p2 = ss2.str();
			string p3 = ss3.str();


			ifstream ifexist(p1);			
			if (!ifexist.is_open())	continue;

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
	system("PAUSE");
	return 0;
}
//20180120
void  PoseMeasure::creatGroundTruthFeatureDianxianmian(int actionBegin, int actionEnd,	int indexBegin, int indexEnd)
{
	string prefix = "D:\\EVAL20170704\\EVAL\\joints\\";

	stringstream  ss;
	ss << prefix << actionBegin << "-" << actionEnd << "bsm_joint_featurenew.txt";//最后一维是标签
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int index = indexBegin; index <= indexEnd; index++)
		{
			vector<vector<float>> gt2;
			stringstream  ss2, ss3;
			ss3 << prefix << "joint3d" << action << "_" << index << ".txt";			
			ss2 << prefix << "3dfeaturejoint" << action << "_" << index << ".txt";

			string p3 = ss3.str(), p2 = ss2.str();

			ifstream ifexist(p3);
			if (!ifexist.is_open())	continue;

			cout << p3 << endl;
			filetool.ReadmidGT(p3, gt2);

			ofstream of(p2);
			int label = action;

			for (int i = 0; i < Newline_Num; i++)//连线距离
			{
				of << EucDis(gt2[newline[i * 2]], gt2[newline[i * 2 + 1]]) << endl;
				fea << EucDis(gt2[newline[i * 2]], gt2[newline[i * 2 + 1]]) << " ";
			}
			for (int i = 0; i < Newline_Num; i++)//连线方向
			{
				vector<float>temp(3, 0), norm;
				for (int k = 0; k < 3; k++)temp[k] = gt2[newline[i * 2]][k] - gt2[newline[i * 2 + 1]][k];
				norm = NormalizationUnit(temp);
				for (int k = 0; k < 3; k++){ fea << norm[k] << " "; of << norm[k] << endl; }
			}

			//点到四肢部分 直线距离
			for (int i = 0; i < guanjielianXian.size() / 2; i++)
			{
				S_Point lineEnd1(gt2[guanjielianXian[2 * i]]), lineEnd2(gt2[guanjielianXian[2 * i + 1]]);//直线两端
				S_Point pt1(gt2[guanjiexuyaoqiujulidedian[i][0]]);
				double dis1 = DistanceOfPointToLine(&lineEnd1, &lineEnd2, &pt1);

				fea << dis1 << " "; of << dis1 << endl;
			}
			//面的相关信息。。 2个平面法向量 之间方向
			//首先求2个平面法向量
			vector<vector<float>>norm(4);
			for (int i = 0; i < 2; i++)
			{
				vector<vector<float>> part;//每个部位里面3个点
				for (int j = 0; j < 3; j++)
				{
					part.push_back(gt2[guanjiepart[i][j]]);
				}
				norm[i] = getPlaneNorm(part);
			}
			//求法向量之间夹角
			for (int i = 0; i < 2; i++)
			{
				for (int j = i + 1; j < 2; j++)
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
	fea.close();
}
//20180124 
void  PoseMeasure::creatGroundTruthFeatureDianxianmian222(int actionBegin, int actionEnd, int indexBegin, int indexEnd)
{
	string prefix = "D:\\EVAL20170704\\EVAL\\joints\\";

	stringstream  ss;
	ss << prefix << actionBegin << "-" << actionEnd << "bsm_joint_feature222.txt";//最后一维是标签
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int index = indexBegin; index <= indexEnd; index++)
		{
			vector<vector<float>> gt2;
			stringstream  ss2, ss3;
			ss3 << prefix << "joint3d" << action << "_" << index << ".txt";
			ss2 << prefix << "3d222featurejoint" << action << "_" << index << ".txt";

			string p3 = ss3.str(), p2 = ss2.str();

			ifstream ifexist(p3);
			if (!ifexist.is_open())	continue;

			cout << p3 << endl;
			filetool.ReadmidGT(p3, gt2);

			ofstream of(p2);
			int label = action;

			for (int i = 0; i < Newline_Num; i++)//连线距离
			{
				of << EucDis(gt2[newline[i * 2]], gt2[newline[i * 2 + 1]]) << endl;
				fea << EucDis(gt2[newline[i * 2]], gt2[newline[i * 2 + 1]]) << " ";
			}
			for (int i = 0; i < Newline_Num; i++)//连线方向
			{
				vector<float>temp(3, 0), norm;
				for (int k = 0; k < 3; k++)temp[k] = gt2[newline[i * 2]][k] - gt2[newline[i * 2 + 1]][k];
				norm = NormalizationUnit(temp);
				for (int k = 0; k < 3; k++){ fea << norm[k] << " "; of << norm[k] << endl; }
			}

			//点到四肢部分 直线距离
			for (int i = 0; i < guanjielianXian.size() / 2; i++)
			{
				S_Point lineEnd1(gt2[guanjielianXian[2 * i]]), lineEnd2(gt2[guanjielianXian[2 * i + 1]]);//直线两端
				S_Point pt1(gt2[guanjiexuyaoqiujulidedian[i][0]]);
				double dis1 = DistanceOfPointToLine(&lineEnd1, &lineEnd2, &pt1);

				fea << dis1 << " "; of << dis1 << endl;
			}
			//面的相关信息。。 2个平面法向量 之间方向
			//首先求2个平面法向量
			vector<vector<float>>norm(4);
			for (int i = 0; i < 2; i++)
			{
				vector<vector<float>> part;//每个部位里面3个点
				for (int j = 0; j < 3; j++)
				{
					part.push_back(gt2[guanjiepart[i][j]]);
				}
				norm[i] = getPlaneNorm(part);
			}
			//求法向量之间夹角
			for (int i = 0; i < 2; i++)
			{
				for (int j = i + 1; j < 2; j++)
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
	fea.close();
}
//将每一帧的3d关节点特征   汇总成一个文件
//20180123
void PoseMeasure::gather3dGTFeature(int actionBegin, int actionEnd, int indexBegin, int indexEnd)
{
	string prefix = "D:\\EVAL20170704\\EVAL\\joints\\";

	stringstream  ss;
	ss << prefix << actionBegin << "-" << actionEnd << "bsm_joint_feature100-250.txt";//最后一维是标签
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int index = indexBegin; index <= indexEnd; index++)
		{
			vector<float> gt2;
			stringstream  ss2;

			ss2 << prefix << "3dfeaturejoint" << action << "_" << index << ".txt";

			string  p2 = ss2.str();

			ifstream ifexist(p2);
			if (!ifexist.is_open())	continue;

			cout << p2 << endl;
			filetool.ReadFile(p2, gt2);

			for (int i = 0; i < gt2.size(); i++)
			{
				if (i == 0)fea << gt2[i];
				else fea << " " << gt2[i];
			}
			fea << endl;
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
//20180121
void PoseMeasure::getTrainAndTestData(vector<Mat>& trainSample, vector<Mat>& testSample,
	vector<int>& trainLabel, vector<int>& testLabel,string prefix, int row, int col, bool isjulei,
	int actionBegin, int actionEnd,	int indexBegin, int indexEnd, int jiange,int dim)
{
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		/*if (action == 2 || action == 6 || action == 7 || 
			action == 5 || action == 3 || action == 4)continue;*/
		for (int index = indexBegin; index <= indexEnd; index++)
		{
			int label;
			Mat sample;
			int m = row, n = col;
			stringstream ss;

			if (isjulei)
			{
				if (dim == 3)ss << prefix << "\\" << action << "\\"<< index << "\\3dfeature.txt";
				else ss << prefix << "\\" << action << "\\" << index << "\\3dfeature4个点.txt";
			}
			else//关节点
			{
				ss << prefix << "\\3dfeaturejoint" << action << "_" <<index << ".txt";
			}
			string path = ss.str();
			ifstream ifexist(path);
			if (!ifexist.is_open())	continue;
			sample = filetool.InitMat(path, m, n, true, label);
			cout << path << endl;
			normalize(sample, sample, 1.0, 0.0, NORM_MINMAX);//归一化

			if (index % 5 == 0)//取样做为测试
			{
				indexmptest[testSample.size()] = action * 10000 + index;

				testLabel.push_back(label);
				testSample.push_back(sample);
			}
			//else //train
			if (index % jiange == 0)//test全取  任取一帧作为测试，，求与train中的样本的距离
			{
				indexmp[trainSample.size()] = action * 10000 + index;

				trainLabel.push_back(label);
				trainSample.push_back(sample);
			}
		}		
	}
}

//测试一下knn是否跑通 跑正确 int jiange 代表多少帧采样 没个10帧还是5帧之类的
//20171130
void PoseMeasure::testknn(bool isjulei, int k, int startindex,
	int actionBegin, int actionEnd,	int indexBegin, int indexEnd, string matrixName,int col,int jiange,int dim)
{
	vector<Mat> trainSample, testSample;
	vector<int> trainLabel, testLabel;
	string prefix;
	int label, correct = 0,row=1;
	Mat matrix;

	if (isjulei)//聚类特征点
	{
		prefix = "D:\\EVAL20170704\\EVAL\\depth";
		if (dim == 3)
		{			
			matrix = filetool.InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\"+ matrixName, col, 10, false, label);
			getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, true, actionBegin, actionEnd, indexBegin, indexEnd,jiange);
		}
		if (dim == 4)
		{			
			matrix = filetool.InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\" + matrixName, col, 10, false, label);
			getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, true, actionBegin, actionEnd, indexBegin, indexEnd, jiange, 4);
		}
	}
	else
	{
		prefix = "D:\\EVAL20170704\\EVAL\\joints";		
		matrix = filetool.InitMat("E:\\xinyongjiacode\\code_bsm\\bsm\\"+ matrixName, col, 10, false, label);
		getTrainAndTestData(trainSample, testSample, trainLabel, testLabel, prefix, row, col, false, actionBegin, actionEnd, indexBegin, indexEnd, jiange);
	}
	//startindex 是指数组下标索引  现在需要转换一下 比如输入测试帧直接是图片名称而不是在数组中的下标了
		
	//cout << "开始测试：" << endl; correct = 0;
	//for (int i = 0; i < testSample.size();i++)
	//{
	//	label = knn(trainSample, trainLabel, testSample[i], i, matrix, k, prefix, actionBegin, actionEnd, matrixName);
	//	cout << "label:" << label << endl;

	//	if (label == testLabel[i])
	//	{
	//		correct++;
	//		cout << "correct!" << endl;
	//	}
	//}
	//cout << "正确率：" << correct << "/" << testSample.size() << "=" << ((float)correct / (float)testSample.size()) << endl;


	/*新的评价方法*/
	cout << "开始测试：" << endl; correct = 0;
	for (int i = 0; i < testSample.size(); i++)
	{
		label = knn(trainSample, trainLabel, testSample[i], i, matrix, k, prefix, actionBegin, actionEnd, matrixName);
		cout << "label:" << label << endl;
		cout << indexmptest[i] << "---------" << endl;
		if (evaluatePrecision(indexmptest[i], knnresult, 3))
		{
			correct++;
			cout << "correct!" << endl;
		}
		knnresult.clear();
	}
	cout << "正确率：" << correct << "/" << testSample.size() << "=" << ((float)correct / (float)testSample.size()) << endl;
}
/*
20180126重新定义评价精度方法
类似于一个窗口
输入i 输出结果 如果为 i+1,i+2,i+3...为正确
如果为 i-1,i-2,i-3...也为正确
i+1 i-1 i+2 i-2 也为正确
但是不能有逆序数 或者逆序数为1个算正确  或者2个算正确 统计一下不同情况的正确率 threshold
（单独用一个序列做判断,或者其他序列如果在的话 不计算）
*/
bool PoseMeasure::evaluatePrecision(int testindex, vector<int>& result, int threshold)
{
	vector<int>left, right;
	for (auto r : result)
	{
		if (r < testindex)left.push_back(r);
		else right.push_back(r);
	}
	int inverseNumLeft = 0, inverseNumRight = 0;//记录左右两边的逆序数

	for (int i = 0; i < left.size(); i++)
	{
		cout << left[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < right.size(); i++)
	{
		cout << right[i] << " ";
	}
	cout << endl;

	//右边的数字顺序应该是从小到大 否则就是逆序
	//左边相反
	for (int i = 0; i < right.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (right[j]>right[i])
			{
				inverseNumRight++;
				cout << right[j] << ">" << right[i] << endl;
			}
		}
	}
	for (int i = 0; i < left.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (left[j] < left[i])inverseNumLeft++;
		}
	}
	cout << "inverseNumLeft:" << inverseNumLeft << "----inverseNumRight:" << inverseNumRight << endl;

	if (inverseNumLeft + inverseNumRight > threshold)return false;
	else return true;
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
构造点线面相关的姿态特征。
20180119
*/
void  PoseMeasure::creatClusterFeatureDianxianmian(int actionBegin, int actionEnd,int indexBegin, int indexEnd,int dim)
{
	string prefix = "D:\\EVAL20170704\\EVAL\\depth\\";

	stringstream  ss;
	ss << prefix <<  actionBegin << "-" << actionEnd << "bsm_all_featurenewpart.txt";//最后一维是标签
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int index = indexBegin; index <= indexEnd; index++)
		{
			vector<vector<float>> gt2;
			stringstream  ss2, ss3;
			ss3 << prefix << action << "\\" << index << "\\featurePoints3d.txt";			
			ss2 << prefix << action << "\\" << index << "\\3dfeature.txt";

			string p3 = ss3.str(), p2 = ss2.str();

			ifstream ifexist(p3);
			if (!ifexist.is_open())	continue;

			cout << p3 << endl;
			filetool.ReadmidGT(p3, gt2);

			ofstream of(p2);
			int label = action;
			for (int i = 0; i < JULEI_line_Num; i++)//连线距离
			{
				of << EucDis(gt2[julei_line[i * 2]], gt2[julei_line[i * 2 + 1]]) << endl;
				fea << EucDis(gt2[julei_line[i * 2]], gt2[julei_line[i * 2 + 1]]) << " ";
			}
			for (int i = 0; i < JULEI_line_Num; i++)//连线方向
			{
				vector<float>temp(3, 0), norm;
				for (int k = 0; k < 3; k++)temp[k] = gt2[julei_line[i * 2]][k] - gt2[julei_line[i * 2 + 1]][k];
				norm = NormalizationUnit(temp);
				for (int k = 0; k < 3; k++){ fea << norm[k] << " "; of << norm[k] << endl; }
			}

			//点到四肢部分 直线距离
			for (int i = 0; i < juleilianXian.size() / 2; i++)
			{
				S_Point lineEnd1(gt2[juleilianXian[2 * i]]), lineEnd2(gt2[juleilianXian[2 * i + 1]]);//直线两端
				S_Point pt1(gt2[juleixuyaoqiujulidedian[i][0]]);
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
					part.push_back(gt2[juleipart[i][j]]);
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
	fea.close();
}

//将每一帧的3d聚类特征   汇总成一个文件
//20180123
void PoseMeasure::gather3dFeature(int actionBegin, int actionEnd, int indexBegin, int indexEnd)
{
	string prefix = "D:\\EVAL20170704\\EVAL\\depth\\";

	stringstream  ss;
	ss << prefix << actionBegin << "-" << actionEnd << "bsm_all_feature100-250.txt";//最后一维是标签
	string p1 = ss.str();
	ofstream fea(p1);
	for (int action = actionBegin; action <= actionEnd; action++)
	{
		for (int index = indexBegin; index <= indexEnd; index++)
		{
			vector<float> gt2;
			stringstream  ss2;
		
			ss2 << prefix << action << "\\" << index << "\\3dfeature.txt";

			string  p2 = ss2.str();

			ifstream ifexist(p2);
			if (!ifexist.is_open())	continue;

			cout << p2<< endl;
			filetool.ReadFile(p2, gt2);

			for (int i = 0; i < gt2.size();i++)
			{
				if (i == 0)fea << gt2[i];
				else fea << " " << gt2[i];
			}			 	
			fea << endl;			
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

//有缺失的行 使用上一帧数据
void FileTool::jiaozheng()
{
	string buff;
	string prefix = "D:\\EVAL20170704\\EVAL\\depth\\";
	for (int action = 0; action <= 7; action++)
	{
		for (int index = 0; index <= 450; index++)
		{
			vector<vector<float>> gt2;
			stringstream  ss3;
			ss3 << prefix << action << "\\" << index << "\\featurePoints3d.txt";

			ifstream infile(ss3.str());
			if (!infile.is_open())	{ cout << ss3.str() << endl; continue; }
			
			int linenum = 0;
			vector <int> queshao;
			while (getline(infile, buff))//按行读取
			{
				if (buff.size() == 0)//说明缺少这一行
				{					
					queshao.push_back(linenum);
					gt2.push_back({ 0, 0, 0 });//占位；
				}
				else
				{					
					stringstream bs(buff);
					vector<float> temp(3);
					bs >> temp[0] >> temp[1] >> temp[2];
					gt2.push_back(temp);
				}
				linenum++;
			}			 
			if (queshao.size()!=0)//读取上一帧
			{				
				vector<vector<float>> gt;
				stringstream  sss;
				sss << prefix << action << "\\" << index-1 << "\\featurePoints3d.txt";				 
				
				cout << "读取上一帧:" << sss.str()<< endl;
				ReadmidGT(sss.str(), gt);

				for (int i = 0; i < queshao.size();i++)gt2[queshao[i]] = gt[queshao[i]];//将上一帧对应的特征赋值给这一帧					 
				
				ofstream outfile(ss3.str());//重新输出调整后的
				for (int i = 0; i < gt2.size(); i++)
				{
					for (int j = 0; j < 3; j++)
					{
						outfile << gt2[i][j] << " ";
					}
					outfile << endl;
				}
				outfile.close();
			}
		}
	}		 
}