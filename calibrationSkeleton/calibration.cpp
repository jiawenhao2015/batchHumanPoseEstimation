/* 真实数据标定骨架点程序
1.首先没有groundtruth，只有之前产生的特征点
2.每一帧frame的特征点F乘以之前随便训练好的映射矩阵M
得到一个初步需要调整的骨架点信息S = F*M 保存起来
3.将每一个关节点编号1-15 连线 然后调整位置
4.将调整后的关节点保存 更新
*/
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <fstream>
#include "ImageShow.h"
#include "Loading.h"
#include "public.h"



using namespace std;
using namespace cv;

//1首先load点云 显示出来
//2计算关节点位置 显示出来
//3调节关节点位置 保存 进入下一帧

vector<float>jointsX;
vector<float>jointsY;
Public tools;
int selectIndex = 0;

//读取关节点坐标存入 jointsX jointsY
void loadGroundTruth(string filePath)
{
	ifstream infile(filePath);
	jointsX.clear();
	jointsY.clear();
	if (infile.is_open())
	{
		float x, y;
		while (!infile.eof())
		{
			infile >> x >> y;
			jointsX.push_back(x);
			jointsY.push_back(y);
		}
	}
	else cout << "不能打开文件！" << endl;
	jointsX.pop_back();
	jointsY.pop_back();
}
//将jointsX jointsY   保存
void saveGroundTruth(string filePath)
{
	ofstream outfile(filePath);
	for (int i = 0; i < JOINT_NUM; i++)
	{
		outfile << jointsX[i] << " " << jointsY[i] << endl;
	}
	outfile.close();
}


void onMouse(int event, int x, int y, int flags, void* param)
{
	static bool isMouseDown = false;
	//static int selectIndex = 0;
	//Mat *photo = (Mat*)param;
	//Mat  temp = photo->clone();
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		for (int i = 0; i < JOINT_NUM; i++)//选中某个关节
		{
			if (abs(jointsX[i] - x) < 10 && abs(jointsY[i] - y) < 10)
			{
				cout << "选中关节：" << i << endl;
				selectIndex = i;
				isMouseDown = true;
				break;
			}
		}
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		isMouseDown = false;
	}
	if (event == CV_EVENT_MOUSEMOVE)
	{
		if (isMouseDown)
		{
			jointsX[selectIndex] = x;
			jointsY[selectIndex] = y;

			tools.DrawSkeleton(*(Mat *)param, jointsX, jointsY);//更新画面		
		}
	}
	return;

}
Mat InitMat(string matrixPath, int m, int n);
//利用已经有的特征点 乘以 映射矩阵 生成初步估计的关节点 
void generateJoints()
{
	Mat projectMat = tools.InitMat("E:/MatrixT.txt", 32, 30);
	char  featurePath[128];
	ofstream errlog("output/errlog.txt", ios_base::app);
	for (int model = 1; model <= 6; model++)
	{
		for (int action = 1; action <= 14; action++)
		{
			for (int frame = 0; frame < 125; frame++)
			{
				sprintf_s(featurePath, "E:/laboratory/dataset/realdataresults/model%d/action%d/%d/clusterpoint.txt", model, action, frame);
				cout << featurePath << endl;
				ifstream isexist(featurePath);
				if (!isexist.is_open())//当前不存在 
				{
					continue;
				}
				Mat featrueMat = tools.InitMat(featurePath, 1, 32);
				if (featrueMat.empty())//说明为空
				{
					errlog << featurePath << " 不存在" << endl;
					cout << featurePath << " 不存在" << endl;
					errlog.close();
					continue;
				}
				Mat guessJoints = featrueMat * projectMat;
				char temp[128];
				sprintf_s(temp, "E:/laboratory/dataset/realdataresults/model%d/action%d/%d/guessJoints.txt", model, action, frame);
				tools.WriteMat(guessJoints, temp);
			}
		}
	}

}

//读入关节点位置 并标号 连线  调整位置 更新
void calibration()
{	
	char  pointCloud[128];
	char joints[128];
	char preFrameJoints[128];//上一帧关节点位置
	bool preFrame = false;//判断是否加载上一帧还是加载这一帧
	for (int model = 999; model <= 999; model++)
	{
		for (int action = 1; action <= 1; action++)
		{
			for (int frame = 56; frame <= 56; frame++)
			{
				sprintf_s(pointCloud, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/%04dpointscloud.txt", model, action, frame);
				ifstream isexist(pointCloud);
				if (!isexist.is_open())continue;//当前不存在
				cout << pointCloud << endl;

				ImageShow ShowTool;
				Mat photo(HEIGHT, WIDTH, CV_8UC3);
				vector<Data> src;
				Loading Load;
				Load.DownLoad_XuNiData(pointCloud, src, 1);
				photo = ShowTool.getPhoto(src);// 加载点云



				sprintf_s(joints, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/%04dbiaoding.txt", model, action, frame);
				if (frame != 0)sprintf_s(preFrameJoints, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/%04dbiaoding.txt", model, action, frame - 1);

				if (preFrame)////加载上一帧已经标定好的关节 作为这一帧的初始
				{
					ifstream isexist2(preFrameJoints);
					if (!isexist2.is_open()) loadGroundTruth(joints);//关节点
					else
					{
						cout << preFrameJoints << endl;
						loadGroundTruth(preFrameJoints);//关节点
					}
				}
				else //加载这一帧
				{
					ifstream isexist2(joints);					
					if (!isexist2.is_open())continue;//当前不存在
					cout << joints << endl;
					loadGroundTruth(joints);//关节点
				}

				namedWindow("calibration");
				setMouseCallback("calibration", onMouse, &photo);
				tools.frame = frame;
				tools.DrawSkeleton(photo, jointsX, jointsY);

				int keyValue = 0;
				bool processFlag = true;
				while (processFlag)
				{
					keyValue = waitKey(0);//没有这句话会卡住不动。。。

					switch (keyValue)
					{
					case 'p':	//重新加载上一帧
						if (frame >= 0)
						{
							if (frame == 0)frame = -1;
							else frame -= 2;
							processFlag = false;
							preFrame = false;//
						}
						break;
					case 's'://save
					case 'S':
						saveGroundTruth(joints);
						cout << "success save" << endl;
						break;
					case 'n':
					case 'N'://next frame
						processFlag = false;
						preFrame = true;
						break;
					case 2424832:////left

						jointsX[selectIndex] -= 1;
						break;

					case 2490368://38://up
						jointsY[selectIndex] -= 1;
						break;

					case 2555904://39://right						
						jointsX[selectIndex] += 1;
						break;

					case 2621440://40://down						
						jointsY[selectIndex] += 1;
						break;
					default:
						break;
					}

					tools.DrawSkeleton(photo, jointsX, jointsY);
				}
			}
		}
	}
}

int main()
{


	//generateJoints();

	calibration();

	getchar();
	return 0;
}