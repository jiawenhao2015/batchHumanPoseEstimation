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


int main()
{
	PoseMeasure pm;

	//pm.adjustClusterPoint();
	//pm.jiaozhun();
	//pm.creatClusterFeature2(7,7,1,1,0,299);//����������� ʵ�������� ����ʹ 
	//pm.creatClusterFeature(7, 7, 1, 1, 0, 299,4);
	
	
//	pm.creatClusterFeature3(7, 9, 1, 1, 0, 299,4);//  //840ά
//	pm.creatClusterFeature4(7, 9, 1, 1, 0, 299, 4);//108ά

	//	pm.creatGroundTruthFeature(7, 9, 1, 1, 0, 299);
	int k = 11;
	int testindex = 81120;
	pm.testknn(true, k, testindex, 7, 9, 1, 1, 0, 299,"W_bsm7-9juleiLianxian.txt",108,5, 4);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
//	pm.testknn(false, k, testindex, 7, 9, 1, 1, 0, 299, "W_bsm7-9guanjie.txt", 60,10, 4);//true�Ǿ������� ���һ�������Ǵ����Ǽ��������� Ĭ��3ά

	pm.testknn(true, k, testindex, 7, 9, 1, 1, 0, 299, "W_bsm7-9juleiNew.txt", 840, 5, 4);
	//	pm.get3dFea(9, 9, 1, 1, 0, 199,4);//������3ά����  ���һ�������Ǵ����Ǽ��������� Ĭ��3ά
	//	pm.get3dGT(9, 9, 1, 1, 0, 199);//�ؽڵ�3ά����
	system("PAUSE");
	return 0;
}