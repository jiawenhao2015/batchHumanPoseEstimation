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

//	pm.creatGroundTruthFeatureDianxianmian(7, 9, 1, 1, 0, 299);//90ά
	//pm.adjustClusterPoint();
	//pm.jiaozhun();
	//pm.creatClusterFeature2(7,7,1,1,0,299);//����������� ʵ�������� ����ʹ 
	//pm.creatClusterFeature(7, 7, 1, 1, 0, 299,4);
	
	
//	pm.creatClusterFeature3(7, 9, 1, 1, 0, 299,4);//  //840ά
//	pm.creatClusterFeature4(7, 9, 1, 1, 0, 299, 4);//108ά

//	pm.creatClusterFeatureDianxianmian(7, 9, 1, 1, 0, 299,4);//138ά
	int k = 5;
	int testindex = 71065;
//	pm.testknn(true, k, testindex, 7, 9, 1, 1, 0, 299,"W_bsm7-9dianxianmianrank10.txt",138,1, 4);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
//	pm.testknn(false, k, testindex, 7, 9, 1, 1, 0, 299, "W_bsm7-9guanjiedianxianmianrank10.txt", 90,1, 4);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
	
	
	//pm.testknn(true, k, testindex, 7, 9, 1, 1, 0, 299, "W_bsm7-9juleiLianxian.txt", 108, 5, 4);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά

	
//	ft.resultPicMerge(testindex);
	
	//	pm.get3dFea(7, 7,50, 450);//������3ά����  ���һ�������Ǵ����Ǽ��������� ��������� Ĭ��3��
	pm.get3dGT(2, 7,0, 450);//�ؽڵ�3ά����
	system("PAUSE");
	return 0;
}
