#include <iostream>
#include <iomanip>
#include<string>
#include <vector>
#include <set>
#include <fstream>
#include<map>
#include <sstream>
#include"highgui.h"

#include "posemeasure.h"

#include "opencv2/nonfree/nonfree.hpp"    
#include "opencv2/legacy/legacy.hpp"   
using namespace std;
using namespace cv;

/*EVAL���ݼ��Ͻ�������*/
int main()
{
	PoseMeasure pm;
	FileTool ft;

//	pm.creatGroundTruthFeatureDianxianmian(0, 7, 0, 450);//43ά

//	ft.jiaozheng();
//	pm.gather3dGTFeature(0, 5, 100, 250);

//	pm.gather3dFeature(0, 5, 100, 250);
//	pm.creatClusterFeatureDianxianmian(0,7,0, 450);//102weidu
	int k = 5;
	int testindex = 71065;

//	pm.testknn(true, k, testindex, 0, 0, 100, 250, "eval0-5bsm_julei_feature100-250.txt", 102, 1);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
	pm.testknn(false, k, testindex, 1, 1, 100, 250, "eval0-5bsm_joint_feature100-250.txt", 43, 1);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
	
//	pm.testknn(false, k, testindex, 0, 7, 0, 200, "evalGT0-7dianxianmianrank10.txt", 43, 1);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
	
	//pm.testknn(true, k, testindex, 7, 9, 1, 1, 0, 299, "W_bsm7-9juleiLianxian.txt", 108, 5, 4);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά

	
//	ft.resultPicMerge(testindex);
	
//	pm.get3dFea(7, 7,50, 450);//������3ά����  ���һ�������Ǵ����Ǽ��������� ��������� Ĭ��3��
//	pm.get3dGT(2, 7,0, 450);//�ؽڵ�3ά����
	system("PAUSE");
	return 0;
}
