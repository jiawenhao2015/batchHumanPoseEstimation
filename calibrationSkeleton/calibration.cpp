/* ��ʵ���ݱ궨�Ǽܵ����
1.����û��groundtruth��ֻ��֮ǰ������������
2.ÿһ֡frame��������F����֮ǰ���ѵ���õ�ӳ�����M
�õ�һ��������Ҫ�����ĹǼܵ���ϢS = F*M ��������
3.��ÿһ���ؽڵ���1-15 ���� Ȼ�����λ��
4.��������Ĺؽڵ㱣�� ����
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

//1����load���� ��ʾ����
//2����ؽڵ�λ�� ��ʾ����
//3���ڹؽڵ�λ�� ���� ������һ֡

vector<float>jointsX;
vector<float>jointsY;
Public tools;
int selectIndex = 0;

//��ȡ�ؽڵ�������� jointsX jointsY
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
	else cout << "���ܴ��ļ���" << endl;
	jointsX.pop_back();
	jointsY.pop_back();
}
//��jointsX jointsY   ����
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
		for (int i = 0; i < JOINT_NUM; i++)//ѡ��ĳ���ؽ�
		{
			if (abs(jointsX[i] - x) < 10 && abs(jointsY[i] - y) < 10)
			{
				cout << "ѡ�йؽڣ�" << i << endl;
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

			tools.DrawSkeleton(*(Mat *)param, jointsX, jointsY);//���»���		
		}
	}
	return;

}
Mat InitMat(string matrixPath, int m, int n);
//�����Ѿ��е������� ���� ӳ����� ���ɳ������ƵĹؽڵ� 
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
				if (!isexist.is_open())//��ǰ������ 
				{
					continue;
				}
				Mat featrueMat = tools.InitMat(featurePath, 1, 32);
				if (featrueMat.empty())//˵��Ϊ��
				{
					errlog << featurePath << " ������" << endl;
					cout << featurePath << " ������" << endl;
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

//����ؽڵ�λ�� ����� ����  ����λ�� ����
void calibration()
{	
	char  pointCloud[128];
	char joints[128];
	char preFrameJoints[128];//��һ֡�ؽڵ�λ��
	bool preFrame = false;//�ж��Ƿ������һ֡���Ǽ�����һ֡
	for (int model = 999; model <= 999; model++)
	{
		for (int action = 1; action <= 1; action++)
		{
			for (int frame = 56; frame <= 56; frame++)
			{
				sprintf_s(pointCloud, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/%04dpointscloud.txt", model, action, frame);
				ifstream isexist(pointCloud);
				if (!isexist.is_open())continue;//��ǰ������
				cout << pointCloud << endl;

				ImageShow ShowTool;
				Mat photo(HEIGHT, WIDTH, CV_8UC3);
				vector<Data> src;
				Loading Load;
				Load.DownLoad_XuNiData(pointCloud, src, 1);
				photo = ShowTool.getPhoto(src);// ���ص���



				sprintf_s(joints, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/%04dbiaoding.txt", model, action, frame);
				if (frame != 0)sprintf_s(preFrameJoints, "E:/laboratory/RecordProcess/RecordProcess/output/model%d/action%d/%04dbiaoding.txt", model, action, frame - 1);

				if (preFrame)////������һ֡�Ѿ��궨�õĹؽ� ��Ϊ��һ֡�ĳ�ʼ
				{
					ifstream isexist2(preFrameJoints);
					if (!isexist2.is_open()) loadGroundTruth(joints);//�ؽڵ�
					else
					{
						cout << preFrameJoints << endl;
						loadGroundTruth(preFrameJoints);//�ؽڵ�
					}
				}
				else //������һ֡
				{
					ifstream isexist2(joints);					
					if (!isexist2.is_open())continue;//��ǰ������
					cout << joints << endl;
					loadGroundTruth(joints);//�ؽڵ�
				}

				namedWindow("calibration");
				setMouseCallback("calibration", onMouse, &photo);
				tools.frame = frame;
				tools.DrawSkeleton(photo, jointsX, jointsY);

				int keyValue = 0;
				bool processFlag = true;
				while (processFlag)
				{
					keyValue = waitKey(0);//û����仰�Ῠס����������

					switch (keyValue)
					{
					case 'p':	//���¼�����һ֡
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