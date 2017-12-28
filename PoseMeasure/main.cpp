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
	//pm.creatClusterFeature2(7,7,1,1,0,299);//这个是新特征 实验结果不好 不好使 
	//pm.creatClusterFeature3(7, 9, 1, 1, 0, 299,4);//最后一个参数是代表是几个特征点 默认3维
//	pm.creatClusterFeature4(7, 9, 1, 1, 0, 299, 4);//最后一个参数是代表是几个特征点 默认3维

	//	pm.creatGroundTruthFeature(7, 9, 1, 1, 0, 299);
	int k = 11;
	int testindex = 91055;
	pm.testknn(true, k, testindex, 7, 9, 1, 1, 0, 299,"W_bsm7-9juleiNew.txt",841, 4);//true是聚类特征 最后一个参数是代表是几个特征点 默认3维

	//	pm.get3dFea(9, 9, 1, 1, 0, 199,4);//特征点3维坐标  最后一个参数是代表是几个特征点 默认3维
	//	pm.get3dGT(9, 9, 1, 1, 0, 199);//关节点3维坐标
	system("PAUSE");
	return 0;
}
