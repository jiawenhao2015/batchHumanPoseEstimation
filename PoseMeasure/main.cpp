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
	FileTool ft;
//	pm.creatGroundTruthFeatureDianxianmian(7, 9, 1, 1, 0, 299);//90维
	//pm.adjustClusterPoint();
	//pm.jiaozhun();
	//pm.creatClusterFeature2(7,7,1,1,0,299);//这个是新特征 实验结果不好 不好使 
	//pm.creatClusterFeature(7, 7, 1, 1, 0, 299,4);
	
	
//	pm.creatClusterFeature3(7, 9, 1, 1, 0, 299,4);//  //840维
//	pm.creatClusterFeature4(7, 9, 1, 1, 0, 299, 4);//108维

//	pm.creatClusterFeatureDianxianmian(7, 9, 1, 1, 0, 299,4);//138维
	int k = 5;
	int testindex = 71065;
//	pm.testknn(true, k, testindex, 7, 7, 1, 1, 0, 299,"W_bsm7-9dianxianmianrank10.txt",138,1, 4);//true是聚类特征 最后一个参数是代表是几个特征点 默认3维
	pm.testknn(false, k, testindex, 9, 9, 1, 1, 0, 299, "W_bsm7-9guanjiedianxianmianrank10.txt", 90,1, 4);//true是聚类特征 最后一个参数是代表是几个特征点 默认3维
	
	
	//pm.testknn(true, k, testindex, 7, 9, 1, 1, 0, 299, "W_bsm7-9juleiLianxian.txt", 108, 5, 4);//true是聚类特征 最后一个参数是代表是几个特征点 默认3维

	
//	ft.resultPicMerge(testindex);
	
	//	pm.get3dFea(9, 9, 1, 1, 0, 199,4);//特征点3维坐标  最后一个参数是代表是几个特征点 默认3维
	//	pm.get3dGT(9, 9, 1, 1, 0, 199);//关节点3维坐标
	system("PAUSE");
	return 0;
}
