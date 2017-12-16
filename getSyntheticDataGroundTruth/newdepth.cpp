//// depth.cpp : 定义控制台应用程序的入口点。
//
////
//
//
//
//#include "stdafx.h"
//
//#include <iostream>
//
////#include <Windows.h>
//
//#include <osg/Matrix>
//
//
//
//#include "BvhReader.h"
//
//
//
//#include "BvhData.h"
//
//#include <Windows.h>
//
//#include <osgViewer/Viewer>
//
//#include <osgViewer/Renderer>
//
//#include <osgDB/ReadFile>
//
//#include <osgDB/WriteFile>
//
//#include <osgGA/TrackballManipulator>
//
//#include <osg/Image>
//
//
//
//
//
//#include <osgGA/DriveManipulator>
//
//#include <osgGA/FlightManipulator>
//
//#include <osgGA/KeySwitchMatrixManipulator>
//
//#include <osgGA/TerrainManipulator>
//
//#include <osgGA/TrackballManipulator>
//
//
//
//#include <osgText/Text>
//
//#include <osg/MatrixTransform>
//
//#include <osg/Point>
//
//
//
//#include <osg/CameraView>
//
//#include <osg/LightModel>
//
//
//
//#include <opencv/cv.h>
//
//#include <opencv/highgui.h>
//
//
//
//#include <boost/filesystem/operations.hpp>
//
//#include <boost/filesystem/path.hpp>
//
//#include <boost/filesystem/convenience.hpp>
//
////#include <fstream>
//
//
//
////using namespace std;
//
//using namespace BVHViewer_DataAccess;
//
//using namespace BVHViewer_DataObjects;
//
//
//
//using namespace std;
//
//using namespace osg;
//
//using namespace cv;
//
//
//
//namespace fs = boost::filesystem;
//
//
//
//#define WIN_WIDTH 640
//
//#define WIN_HEIGHT 480
//
//
//
//float __deltaD = 0.02;//相邻区域有效深度差阈值
//
//
//
//int __dirIndex = 131;//1
//
//int __dirEndIndex = 144;
//
//int __fileIndex = 0;//++ before replace //0
//
//
//
//int __frameIndex = -1;//-1;//++ before replace //-1
//
//int __frameCount = 0;
//
//
//
//bool __bReplacedNode = false;
//
//bool __bStartRepalce = false;
//
//bool __bStartCaputure = false;
//
//
//
//char __objDirName[] = "E:/DATASET/cmu-mocap-master/obj";
//
//char __outputDirName[] = "E:/DATASET/cmu-mocap-master/depth";
//
//
//
//////float *__pDepth = new float[WIN_HEIGHT * WIN_WIDTH];
//
//
//
//BvhData __bvhData;
//
//BvhReader __bvhReader;
//
//
//
//
//
////-------------------------------------------------------------
//
//
//
//int colorMap[PART_NUM * 3] =
//
//{
//
//	170, 0, 170,//170,0,0,//Lfoot = 0,
//
//	85, 0, 170,//0,170,0,//Lleg = 1,
//
//	0, 0, 170,//255,0,85,//Lknee = 2,
//
//	0, 85, 170,//85, 170, 255,//Lthigh = 3,
//
//	170, 0, 85,//170,0,170,//Rfoot = 4,//
//
//	85, 0, 85,//0,170,255,//Rleg = 5,
//
//	0, 0, 85,//170,85,85,//Rknee = 6,
//
//	0, 85, 85,//255,170,255,//Rthigh = 7,
//
//	85, 85, 85,//85,255,170,//Rhips = 8,
//
//	85, 85, 170,//85,85,170,//Lhips = 9,
//
//	85, 85, 255,//0, 0, 255,//Neck = 10,
//
//	170, 85, 0,//170,85,0,//Rarm = 11,//
//
//	255, 85, 0,//85,85,0,//Relbow = 12,
//
//	255, 0, 0,//0,85,255,//Rforearm = 13,//
//
//	170, 0, 0,//0,85,85,//Rhand = 14,//
//
//	170, 85, 255,//0,85,170,//Larm = 15,//
//
//	255, 85, 255,//85,0,0,//Lelbow = 16,
//
//	255, 0, 255,//255,85,0,//Lforearm = 17,//
//
//	170, 0, 255,//85,0,85,//Lhand = 18,//
//
//	255, 170, 170,//85, 170, 85,//FaceLB = 19,
//
//	255, 170, 85,//255, 0, 255,//FaceRB = 20,
//
//	255, 255, 170,//0, 255, 0,//FaceLT = 21,
//
//	255, 255, 85,//255, 0, 0,//FaceRT = 22,
//
//	170, 85, 85,//255,85,85,//Rchest = 23,
//
//	170, 85, 170,//170,0,255,//Lchest = 24,
//
//	255, 85, 170,//0, 170, 85,//Lshoulder = 25,
//
//	255, 85, 85//255,170,0//Rshoulder = 26,
//
//	//255,255,255//Groundplane = 27,
//
//	//Ceiling = 28,
//
//	//Background = 29,
//
//	//Plane = 30,
//
//	//NOLABEL = 31
//
//};
//
//
//
//int getMapValue(float f)
//
//{
//
//	if ((int)f % 85 < 85 / 2)
//
//	{
//
//		return (int)f / 85 * 85;
//
//	}
//
//	else
//
//	{
//
//		return ((int)f / 85 + 1) * 85;
//
//	}
//
//}
//
//
//
//bool isMapColor(cv::Vec3b color)
//
//{
//
//	int k = 0;
//
//	for (k = 0; k < PART_NUM; k++)
//
//	{
//
//		if (abs((int)color[2] - colorMap[3 * k]) < 0.0001
//
//			&& abs((int)color[1] - colorMap[3 * k + 1]) < 0.0001
//
//			&& abs((int)color[0] - colorMap[3 * k + 2]) < 0.0001
//
//			)
//
//		{
//
//			break;
//
//		}
//
//	}
//
//
//
//	if (k == PART_NUM)
//
//	{
//
//		return false;
//
//	}
//
//	else
//
//	{
//
//		return true;
//
//	}
//
//
//
//}
//
//
//
//int mapColorIndex(cv::Vec3b color)
//
//{
//
//	int k = 0;
//
//	for (k = 0; k < PART_NUM; k++)
//
//	{
//
//		if (abs((int)color[2] - colorMap[3 * k]) < 0.0001
//
//			&& abs((int)color[1] - colorMap[3 * k + 1]) < 0.0001
//
//			&& abs((int)color[0] - colorMap[3 * k + 2]) < 0.0001
//
//			)
//
//		{
//
//			break;
//
//		}
//
//	}
//
//
//
//	return k;
//
//}
//
//
//
//cv::Vec3b medFilter1(Mat m, Mat depthMat, int r, int c, int slide, float ratio)
//
//{
//
//	osg::ref_ptr<osg::Vec4bArray> colorArr = new osg::Vec4bArray;
//
//	int count = 0;
//
//	for (int i = 0; i < slide; i++)
//
//	{
//
//		for (int j = 0; j < slide; j++)
//
//		{
//
//			int rr = r + i - slide / 2;
//
//			int cc = c + j - slide / 2;
//
//			bool hit = false;
//
//
//
//			if ((rr != r || cc != c)//非当前点
//
//				&& (rr >= 0 && rr < m.rows)//边界检查
//
//				&& (cc >= 0 && cc < m.cols))
//
//			{
//
//				if (depthMat.at<float>(rr, cc) != depthMat.at<float>(0, 0)
//
//					&& abs(depthMat.at<float>(rr, cc) - depthMat.at<float>(r, c))<__deltaD
//
//					//m.at<cv::Vec3b>(rr, cc)[0] != 0
//
//					//|| m.at<cv::Vec3b>(rr, cc)[1] != 0
//
//					//|| m.at<cv::Vec3b>(rr, cc)[2] != 0
//
//					)//not backgroud
//
//				{
//
//					if (isMapColor(m.at<cv::Vec3b>(rr, cc)))
//
//					{
//
//						for (int k = 0; k < colorArr->size(); k++)
//
//						{
//
//							if ((unsigned char)colorArr->at(k)[0] == m.at<cv::Vec3b>(rr, cc)[0]
//
//							&& (unsigned char)colorArr->at(k)[1] == m.at<cv::Vec3b>(rr, cc)[1]
//
//							&& (unsigned char)colorArr->at(k)[2] == m.at<cv::Vec3b>(rr, cc)[2])
//
//							{
//
//								colorArr->at(k)[3]++;
//
//								hit = true;
//
//								break;
//
//							}
//
//						}
//
//
//
//						if (!hit)
//
//						{
//
//							colorArr->push_back(osg::Vec4b(
//
//								m.at<cv::Vec3b>(rr, cc)[0],
//
//								m.at<cv::Vec3b>(rr, cc)[1],
//
//								m.at<cv::Vec3b>(rr, cc)[2],
//
//								0));
//
//
//
//						}
//
//
//
//						count++;
//
//					}
//
//				}
//
//			}
//
//		}
//
//	}
//
//
//
//	if (count > slide*slide*ratio)
//
//	{
//
//
//
//		int maxIndex = 0;
//
//		for (int k = 1; k < colorArr->size(); k++)
//
//		{
//
//
//
//			if (colorArr->at(k)[3] > colorArr->at(maxIndex)[3])//>=
//
//			{
//
//				maxIndex = k;
//
//			}
//
//
//
//		}
//
//
//
//		return cv::Vec3b(
//
//			colorArr->at(maxIndex)[0],
//
//			colorArr->at(maxIndex)[1],
//
//			colorArr->at(maxIndex)[2]
//
//		);
//
//	}
//
//	else
//
//	{
//
//		return m.at<cv::Vec3b>(r, c);
//
//	}
//
//}
//
//
//
//
//
//
//
//cv::Vec3b medFilter2(Mat m, Mat depthMat, int r, int c, int slide)
//
//{
//
//	osg::ref_ptr<osg::Vec4bArray> colorArr = new osg::Vec4bArray;
//
//	int count = 0;
//
//	for (int i = 0; i < slide; i++)
//
//	{
//
//		for (int j = 0; j < slide; j++)
//
//		{
//
//			int rr = r + i - slide / 2;
//
//			int cc = c + j - slide / 2;
//
//			bool hit = false;
//
//
//
//			if ((rr != r || cc != c)//非当前点
//
//				&& (rr >= 0 && rr < m.rows)//边界检查
//
//				&& (cc >= 0 && cc < m.cols))
//
//			{
//
//				if (depthMat.at<float>(rr, cc) != depthMat.at<float>(0, 0)
//
//					&& abs(depthMat.at<float>(rr, cc) - depthMat.at<float>(r, c))<__deltaD
//
//					//m.at<cv::Vec3b>(rr, cc)[0] != 0
//
//					//|| m.at<cv::Vec3b>(rr, cc)[1] != 0
//
//					//|| m.at<cv::Vec3b>(rr, cc)[2] != 0
//
//					)//not backgroud
//
//				{
//
//					if (isMapColor(m.at<cv::Vec3b>(rr, cc)))
//
//					{
//
//						for (int k = 0; k < colorArr->size(); k++)
//
//						{
//
//							if ((unsigned char)colorArr->at(k)[0] == m.at<cv::Vec3b>(rr, cc)[0]
//
//							&& (unsigned char)colorArr->at(k)[1] == m.at<cv::Vec3b>(rr, cc)[1]
//
//							&& (unsigned char)colorArr->at(k)[2] == m.at<cv::Vec3b>(rr, cc)[2])
//
//							{
//
//								colorArr->at(k)[3]++;
//
//								hit = true;
//
//								break;
//
//							}
//
//						}
//
//
//
//						if (!hit)
//
//						{
//
//							colorArr->push_back(osg::Vec4b(
//
//								m.at<cv::Vec3b>(rr, cc)[0],
//
//								m.at<cv::Vec3b>(rr, cc)[1],
//
//								m.at<cv::Vec3b>(rr, cc)[2],
//
//								0));
//
//
//
//						}
//
//
//
//						count++;
//
//					}
//
//				}
//
//			}
//
//		}
//
//	}
//
//
//
//	if (count > 0)
//
//	{
//
//
//
//		int maxIndex = 0;
//
//		for (int k = 1; k < colorArr->size(); k++)
//
//		{
//
//
//
//			if (colorArr->at(k)[3] > colorArr->at(maxIndex)[3])//>=
//
//			{
//
//				maxIndex = k;
//
//			}
//
//
//
//		}
//
//
//
//		return cv::Vec3b(
//
//			colorArr->at(maxIndex)[0],
//
//			colorArr->at(maxIndex)[1],
//
//			colorArr->at(maxIndex)[2]
//
//		);
//
//	}
//
//	else
//
//	{
//
//		return m.at<cv::Vec3b>(r, c);
//
//	}
//
//}
//
//
//
////-------------------------------------------------------------
//
//osg::ref_ptr<osg::Node> geodeCoordinate()
//
//{
//
//	osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array;
//
//	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
//
//	coords->push_back(Vec3(0, 0, 0));
//
//	coords->push_back(Vec3(1, 0, 0));
//
//	coords->push_back(Vec3(0, 0, 0));
//
//	coords->push_back(Vec3(0, 1, 0));
//
//	coords->push_back(Vec3(0, 0, 0));
//
//	coords->push_back(Vec3(0, 0, 1));
//
//	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
//
//	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
//
//	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
//
//	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
//
//	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
//
//	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
//
//
//
//	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
//
//	geom->setVertexArray(coords);
//
//	geom->setColorArray(colors);
//
//	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
//
//	geom->addPrimitiveSet(new  osg::DrawArrays(osg::PrimitiveSet::LINES, 0, coords->size()));
//
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//
//	geode->addDrawable(geom.get());
//
//
//
//	return geode;
//
//}
//
//
//
//void transPosition(osg::Matrix &matrix1)
//
//{
//
//	osg::Matrix matrix2 = matrix1;
//
//	for (int i = 0; i < 4; i++)
//
//	{
//
//		for (int j = 0; j < 4; j++)
//
//		{
//
//			matrix1(i, j) = matrix2(j, i);
//
//		}
//
//	}
//
//}
//
//
//
//osg::Matrix getTransformMatrix(BvhData &bvhData, BvhHierarchyNode &node, int iFrame)
//
//{
//
//	osg::Matrix mat;
//
//	mat.makeIdentity();
//
//
//
//	float *channelData = bvhData.getMotionData()
//
//		+ iFrame * bvhData.getMotionDataStride()
//
//		+ node.FirstChannelIndex;
//
//
//
//	float xposition, yposition, zposition;
//
//	float xrotation, yrotation, zrotation;
//
//	float xscale, yscale, zscale;
//
//
//
//	xposition = yposition = zposition = 0;
//
//	xrotation = yrotation = zrotation = 0;
//
//	xscale = yscale = zscale = 1;
//
//
//
//	//static int index=0;
//
//	//std::cout<<"--"<<index++<<"--:\t"<<node.NodeName<<":--\t";
//
//	osg::Matrix rotateMatrix;
//
//
//
//
//
//	for (int i = 0; i < node.ChannelCount; i++)
//
//	{
//
//		switch (node.channels[i])
//
//		{
//
//		case E_BVH_TOKEN_TKW_XPOSITION:
//
//			xposition = channelData[i];
//
//			//std::cout<<"xposition:"<<xposition<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_YPOSITION:
//
//			yposition = channelData[i];
//
//			//std::cout<<"yposition:"<<yposition<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_ZPOSITION:
//
//			zposition = channelData[i];
//
//			//std::cout<<"zposition:"<<zposition<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_XROTATION:
//
//			xrotation = channelData[i];
//
//			rotateMatrix.makeRotate(osg::inDegrees(xrotation), osg::Vec3(1, 0, 0));
//
//			//osg::Matrix::inverse(rotateMatrix);
//
//			transPosition(rotateMatrix);
//
//			mat.postMult(rotateMatrix);//osg::Quat(osg::inDegrees(xrotation),1,0,0));
//
//			//std::cout<<"xrotation:"<<xrotation<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_YROTATION:
//
//			yrotation = channelData[i];
//
//			rotateMatrix.makeRotate(osg::inDegrees(yrotation), osg::Vec3(0, 1, 0));
//
//			transPosition(rotateMatrix);
//
//			mat.postMult(rotateMatrix);//osg::Quat(osg::inDegrees(yrotation),0,1,0));
//
//			//std::cout<<"yrotation:"<<yrotation<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_ZROTATION:
//
//			zrotation = channelData[i];
//
//			rotateMatrix.makeRotate(osg::inDegrees(zrotation), osg::Vec3(0, 0, 1));
//
//			transPosition(rotateMatrix);
//
//			mat.postMult(rotateMatrix);//osg::Quat(osg::inDegrees(zrotation),0,0,1));
//
//			//std::cout<<"zrotation:"<<zrotation<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_XSCALE:
//
//			xscale = channelData[i];
//
//			//std::cout<<"xscale:"<<xscale<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_YSCALE:
//
//			yscale = channelData[i];
//
//			//std::cout<<"yscale:"<<yscale<<" \t";
//
//			break;
//
//		case E_BVH_TOKEN_TKW_ZSCALE:
//
//			zscale = channelData[i];
//
//			//std::cout<<"zscale:"<<zscale<<" \t";
//
//			break;
//
//		}
//
//	}
//
//	//std::cout<<std::endl;
//
//
//
//	mat(0, 3) = xposition;
//
//	mat(1, 3) = yposition;
//
//	mat(2, 3) = zposition;
//
//
//
//	mat.postMultScale(osg::Vec3(xscale, yscale, zscale));
//
//
//
//	return mat;
//
//}
//
//
//
////屏幕坐标与世界坐标转换（屏幕坐标为左下0,0点）
//
//osg::Vec3 getWorldPoint(osg::Camera *camera,float winx,float winy)
//
//{
//
//	osg::Vec3 vec3;
//
//
//
//	osgViewer::Renderer *render = dynamic_cast<osgViewer::Renderer *>(camera->getRenderer());
//
//	osgUtil::SceneView *sceneView = render->getSceneView(0);
//
//
//
//	osg::Vec3 vScreen(winx, winy, 0);
//
//	osg::Vec3 window;
//
//	sceneView->projectWindowIntoObject(vScreen, window);
//
//
//
//	osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();
//
//	osg::Matrix invertVPW;
//
//	invertVPW.invert(mVPW);
//
//
//
//	vec3 = window * invertVPW;
//
//
//
//	return vec3;
//
//}
//
//
//
////屏幕坐标与世界坐标转换（屏幕坐标为左下0,0点）
//
//osg::Vec3 getWindowPoint(osg::Camera *camera, osg::Vec3 point)
//
//{
//
//	osg::Vec3 vec3;
//
//
//
//	osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();
//
//
//
//	vec3 = point * mVPW;
//
//
//
//	return vec3;
//
//}
//
////////////////////////////////////////////////////////////////////////////
//
//class DepthHld : public osgGA::GUIEventHandler
//
//{
//
//
//
//public:
//
//	DepthHld()
//
//	{
//
//
//
//	}
//
//
//
//	~DepthHld()
//
//	{
//
//
//
//	}
//
//
//
//	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//
//	{
//
//		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
//
//		if (!viewer) return false;
//
//
//
//		switch (ea.getEventType())
//
//		{
//
//			//////////////////////////////////////////////
//
//		case(osgGA::GUIEventAdapter::KEYDOWN) :
//
//			{
//
//				if (ea.getKey() == 'o')
//
//				{
//
//					return false;
//
//				}
//
//				if (ea.getKey() == 's')
//
//				{
//
//					__bStartRepalce = true;
//
//
//
//					return false;
//
//				}
//
//			}
//
//			break;
//
//		case(osgGA::GUIEventAdapter::DOUBLECLICK) :
//
//			{
//
//				osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(
//
//					osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
//
//				osgUtil::IntersectionVisitor iv(picker.get());
//
//				viewer->getCamera()->accept(iv);//(从相机往下遍历)
//
//
//
//				if (picker->containsIntersections())
//
//
//
//				{  //获取最前的交点。
//
//
//
//					osg::Vec3 ptWorldIntersectPointFirst = picker->getFirstIntersection().getWorldIntersectPoint();
//
//
//
//					std::cout << ea.getX() << " " << ea.getY() << std::endl;
//
//					std::cout << "world coords vertex(" << ptWorldIntersectPointFirst.x() << ","
//
//						<< ptWorldIntersectPointFirst.y() << "," << ptWorldIntersectPointFirst.z() << ")" << std::endl;
//
//
//
//				}
//
//
//
//			}
//
//
//
//			break;
//
//		default:
//
//			return false;
//
//		}
//
//
//
//		return false;
//
//	}
//
//};
//
//
//
//class ObjCallBack :public osg::NodeCallback
//
//{
//
//public:
//
//	ObjCallBack()
//
//		:_group(NULL)
//
//	{
//
//
//
//	}
//
//
//
//	ObjCallBack(osg::Group *r)
//
//		:_group(r)
//
//	{
//
//
//
//	}
//
//
//
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//
//	{
//
//		if (__bStartRepalce && !__bReplacedNode)
//
//		{
//
//			//cout << "update"<<endl;
//
//			_group->removeChild((unsigned int)0);
//
//
//
//			char fileName[1024];
//
//			char filePrefix[1024];
//
//
//
//			while (__dirIndex <= __dirEndIndex && !__bReplacedNode)
//
//			{
//
//				while (__fileIndex < 50 && !__bReplacedNode)
//
//				{
//
//					if (__frameIndex < __frameCount-1)
//
//					{
//
//						//replace node
//
//						__frameIndex++;
//
//
//
//						//------------------生成错误图片处理-------------------------
//
//						sprintf(fileName, "%s/%03d/color_%02d_%02d_%d.png", __outputDirName, __dirIndex, __dirIndex, __fileIndex, __frameIndex);
//
//						if (fs::exists(fileName))
//
//						{
//
//							bool isNeedReGen = true;
//
//
//
//							Mat colorMat = imread(fileName);
//
//							cout << "check:" << fileName << endl;
//
//							for (int i = 0; i < colorMat.rows && isNeedReGen; i++)
//
//							{
//
//								for (int j = 0; j < colorMat.cols && isNeedReGen; j++)
//
//								{
//
//									if (colorMat.at<cv::Vec3b>(i,j)!=cv::Vec3b(0,0,0))
//
//									{
//
//										isNeedReGen = false;
//
//									}								
//
//								}
//
//							}
//
//
//
//							colorMat.release();
//
//
//
//							if (!isNeedReGen)
//
//							{
//
//								continue;
//
//							}
//
//
//
//							cout << "ReGen-------------" << endl;
//
//						}
//
//
//
//
//
//						sprintf(filePrefix, "%s/%03d/%02d_%02d", __objDirName, __dirIndex, __dirIndex, __fileIndex);
//
//						if (__frameCount == 1)
//
//						{
//
//							sprintf(fileName, "%s.obj", filePrefix);
//
//						}
//
//						else
//
//						{
//
//							sprintf(fileName, "%s_%d.obj", filePrefix, __frameIndex);
//
//						}
//
//
//
//						osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(fileName);
//
//						_group->addChild(node);
//
//						cout << fileName << endl;
//
//						__bReplacedNode = true;
//
//					}
//
//					else
//
//					{
//
//						//read new bvh
//
//						__fileIndex++;
//
//
//
//						sprintf(filePrefix, "%s/%03d", __objDirName, __dirIndex);
//
//						sprintf(fileName, "%s/%02d_%02d.bvh", filePrefix, __dirIndex, __fileIndex);
//
//
//
//						cout << fileName << endl;
//
//						//BvhReader bvhReader;
//
//
//
//						if (__bvhReader.read(&__bvhData, fileName))
//
//						{
//
//							__frameCount = __bvhData.getFrameCount();
//
//							__frameIndex = -1;
//
//
//
//							sprintf(fileName, "%s/PPro_MR_Male.png", filePrefix);
//
//							//判断纹理是否存在
//
//
//
//							if (!fs::exists(fileName))
//
//							{
//
//								fs::copy_file("E:/DATASET/cmu-mocap-master/PPro_MR_Male.png", fileName);
//
//								cout << "copy file to:" << fileName << endl;
//
//							}
//
//
//
//							sprintf(fileName, "%s/%03d", __outputDirName, __dirIndex);
//
//
//
//							if (!fs::exists(fileName))
//
//							{
//
//								fs::create_directories(fileName);
//
//								cout << "create dir:" << fileName << endl;
//
//							}
//
//
//
//						}
//
//					}
//
//
//
//
//
//				}
//
//				if (!__bReplacedNode)
//
//				{
//
//					__dirIndex++;
//
//					__fileIndex = 0;
//
//				}
//
//			}
//
//
//
//		}
//
//
//
//		//继续遍历
//
//		traverse(node, nv);
//
//
//
//	}
//
//
//
//public:
//
//	osg::Group *_group;
//
//};
//
//
//
//
//
//class CaptureCallback : public osg::Camera::DrawCallback
//
//{
//
//public:
//
//	osg::ref_ptr<osg::Camera>	_camera;
//
//	osg::ref_ptr<osg::Image>	_depthbufferimg;
//
//	osg::ref_ptr<osg::Image>	_colorbufferimg;
//
//
//
//	Mat _depthMat;
//
//	//osg::Vec3Array *_bvhPoints;
//
//
//
//public:
//
//	CaptureCallback(osg::Camera *camera,
//
//		osg::Image* depthbufferimg,
//
//		osg::Image* colorbufferimg
//
//		)
//
//		:_camera(camera),
//
//		_depthbufferimg(depthbufferimg),
//
//		_colorbufferimg(colorbufferimg)
//
//	{
//
//		//_bvhPoints = new osg::Vec3Array();
//
//
//
//		_depthMat.create(WIN_HEIGHT, WIN_WIDTH, CV_32FC1);
//
//		if (!_depthMat.isContinuous())
//
//		{
//
//			cout << "_depthimage is not continuous!" << endl;
//
//		}
//
//	}
//
//
//
//	~CaptureCallback()
//
//	{
//
//		_depthMat.release();
//
//	}
//
//
//
//	//Mat BGR
//
//	void processLabel(Mat labelMat) const
//
//	{
//
//		Mat orgMat = labelMat.clone();
//
//		Mat m(orgMat.rows, orgMat.cols, CV_8UC3);
//
//
//
//		//滤掉单点
//
//		for (int i = 0; i < m.rows; i++)
//
//		{
//
//			for (int j = 0; j < m.cols; j++)
//
//			{
//
//				if (_depthMat.at<float>(i, j) != _depthMat.at<float>(0, 0))
//
//				{
//
//					orgMat.at<cv::Vec3b>(i, j) = medFilter1(labelMat, _depthMat, i, j, 3, 0);
//
//				}
//
//			}
//
//		}
//
//
//
//		char buf[1024];
//
//		sprintf(buf, "%s/%03d/depth_%02d_%02d_%d.txt", __outputDirName, __dirIndex, __dirIndex, __fileIndex, __frameIndex);
//
//		std::ofstream outfile(buf);
//
//		std::cout << "Save to:" << buf << std::endl;
//
//		for (int i = 0; i < m.rows; i++)
//
//		{
//
//			for (int j = 0; j < m.cols; j++)
//
//			{
//
//				m.at<cv::Vec3b>(i, j) = orgMat.at<cv::Vec3b>(i, j);
//
//				if (_depthMat.at<float>(i, j) != _depthMat.at<float>(0, 0))
//
//				{
//
//					if (!isMapColor(orgMat.at<cv::Vec3b>(i, j)))
//
//					{
//
//						m.at<cv::Vec3b>(i, j) = medFilter2(orgMat, _depthMat, i, j, 5);
//
//					}
//
//
//
//					osg::Vec3 xyz = getWorldPoint(_camera, i, WIN_HEIGHT - j);
//
//
//
//					outfile << j << " " << i << " "
//
//						<< _depthMat.at<float>(i, j) << " "
//
//						<< xyz.x() << " " << xyz.y() << " " << xyz.z() << " "
//
//						<< mapColorIndex(m.at<cv::Vec3b>(i, j)) << " "
//
//						<< endl;
//
//
//
//
//
//				}
//
//
//
//			}
//
//		}
//
//		outfile.close();
//
//
//
//		//for (int i = 0; i < _bvhPoints->size();i++)
//
//		//{
//
//		//	cv::circle(m, cv::Point(_bvhPoints->at(i).x(), _bvhPoints->at(i).y()),5,Scalar(255,255,255));
//
//		//}
//
//
//
//		////imshow("orgM", orgMat);
//
//		imshow("Label", m);
//
//		sprintf(buf, "%s/%03d/color_%02d_%02d_%d.png", __outputDirName, __dirIndex, __dirIndex, __fileIndex, __frameIndex);
//
//		imwrite(buf, m);
//
//
//
//		waitKey(10);
//
//
//
//		orgMat.release();
//
//		m.release();
//
//	}
//
//
//
//
//
//	void ComputeDepthImage() const
//
//	{
//
//		// get the viewport size
//
//		osg::Viewport* viewport = _camera->getViewport();
//
//		int width = viewport->width();
//
//		int height = viewport->height();
//
//
//
//		// get the intrinsic parameters of the camera
//
//		double fovy, aspectRatio, Zn, Zf;
//
//		_camera->getProjectionMatrixAsPerspective(fovy, aspectRatio, Zn, Zf);
//
//
//
//		// the z buffer values source
//
//		float* z = (float*)_depthbufferimg->data();
//
//
//
//		// the depth image destination
//
//		float* Z = (float *)_depthMat.data;
//
//		////float* Z = _depthMat.ptr<float>();
//
//		////if (_depthMat.isContinuous())
//
//		////{
//
//		////	Z = _depthMat.ptr<float>();
//
//		////}
//
//
//
//		////CMN_ASSERT( Z != NULL );
//
//		size_t i = 0;
//
//
//
//
//
//		Mat labelMat(WIN_HEIGHT, WIN_WIDTH, CV_8UC3);
//
//
//
//		// convert zbuffer values [0,1] to depth values + flip the image vertically
//
//		for (int r = height - 1; 0 <= r; r--){
//
//			for (int c = 0; c < width; c++){
//
//				// forgot where I took this equation
//
//				Z[i] = Zn*Zf / (Zf - z[r*width + c] * (Zf - Zn));
//
//
//
//				if (i && Z[i] != Z[0])
//
//				{
//
//
//
//					float cr = _colorbufferimg->getColor(r*width + c).r();
//
//					float cg = _colorbufferimg->getColor(r*width + c).g();
//
//					float cb = _colorbufferimg->getColor(r*width + c).b();
//
//
//
//
//
//					labelMat.at<cv::Vec3b>(height - 1 - r, c)[2] = getMapValue(cr * 255);
//
//					labelMat.at<cv::Vec3b>(height - 1 - r, c)[1] = getMapValue(cg * 255);
//
//					labelMat.at<cv::Vec3b>(height - 1 - r, c)[0] = getMapValue(cb * 255);
//
//				}
//
//				i++;
//
//			}
//
//		}
//
//
//
//		//char buf[1024];
//
//		//sprintf(buf, "%s/%03d/color_%02d_%02d_%d_0.png", __outputDirName, __dirIndex, __dirIndex, __fileIndex, __frameIndex);
//
//		//osgDB::writeImageFile(*_colorbufferimg, buf);
//
//
//
//		////RGB
//
//		//cv::Mat colorMat(WIN_HEIGHT, WIN_WIDTH, CV_8UC3);
//
//
//
//		//glReadPixels(0, 0, WIN_WIDTH, WIN_HEIGHT,
//
//		//	GL_BGR, GL_UNSIGNED_BYTE, colorMat.data);
//
//
//
//		//for (int i = 0; i < WIN_HEIGHT; i++)
//
//		//{
//
//		//	for (int j = 0; j < WIN_WIDTH; j++)
//
//		//	{
//
//		//		cv::Vec3b rgbv3 = colorMat.at<cv::Vec3b>(i, j);
//
//		//		colorMat.at<cv::Vec3b>(i, j) = colorMat.at<cv::Vec3b>(WIN_HEIGHT - 1 - i, WIN_WIDTH - 1 - j);
//
//		//		colorMat.at<cv::Vec3b>(WIN_HEIGHT - 1 - i, WIN_WIDTH - 1 - j) = rgbv3;
//
//		//	}
//
//		//}
//
//
//
//		//imshow("RGB",colorMat);
//
//		//sprintf(buf, "%s/%03d/color_%02d_%02d_%d.png", __outputDirName, __dirIndex, __dirIndex, __fileIndex, __frameIndex);
//
//		//imwrite(buf, colorMat);
//
//
//
//		//imshow("RGB1", labelMat1);
//
//
//
//		processLabel(labelMat);
//
//		labelMat.release();
//
//		////cv::waitKey(0);
//
//
//
//	}
//
//
//
//	void ComputeBVHPoint() const
//
//	{
//
//		char buf[1024];
//
//		sprintf(buf, "%s/%03d/groundTruth_%02d_%02d_%d.txt", __outputDirName, __dirIndex, __dirIndex, __fileIndex, __frameIndex);
//
//		std::ofstream output(buf);//to be changed
//
//
//
//		osg::ref_ptr<osg::Vec3Array> nodesPosition
//
//			= new osg::Vec3Array(__bvhData.getNodesCount() + 1);
//
//		osg::ref_ptr<osg::Vec3Array> framePosition
//
//			= new osg::Vec3Array(__bvhData.getNodesCount() + 1);
//
//
//
//		osg::ref_ptr<osg::MatrixfArray> matrixArr
//
//			= new osg::MatrixfArray(__bvhData.getNodesCount() + 1);
//
//
//
//		//_bvhPoints->clear();
//
//		for (int i = 1; i <= __bvhData.getNodesCount(); i++)
//
//		{
//
//			BvhHierarchyNode currNode = __bvhData.getNode(i);
//
//			if (currNode.ParentIndex == -1)
//
//			{
//
//				nodesPosition->at(i) = osg::Vec3(0, 0, 0);
//
//				framePosition->at(i) = osg::Vec3(0, 0, 0);
//
//			}
//
//
//
//			osg::Matrix transMatrix = getTransformMatrix(__bvhData, currNode, __frameIndex);
//
//
//
//			transMatrix(0, 3) += currNode.Offset[0];
//
//			transMatrix(1, 3) += currNode.Offset[1];
//
//			transMatrix(2, 3) += currNode.Offset[2];
//
//
//
//			osg::Vec3 parentPosition(0, 0, 0);
//
//			if (currNode.ParentIndex != -1)
//
//			{
//
//				parentPosition = framePosition->at(currNode.ParentIndex);
//
//				transMatrix.preMult(matrixArr->at(currNode.ParentIndex));
//
//			}
//
//
//
//			matrixArr->at(i) = transMatrix;
//
//
//
//			framePosition->at(i) = osg::Vec3(transMatrix(0, 3), transMatrix(1, 3), transMatrix(2, 3));
//
//
//
//			//output
//
//			//std::cout<<"**"<<framePosition->at(i).x()<<" "<<framePosition->at(i).y()<<" "<<framePosition->at(i).z()<<std::endl;
//
//			//output<<framePosition->at(i).x()<<" "<<framePosition->at(i).y()<<std::endl;
//
//			osg::Vec3 oxyz(
//
//				framePosition->at(i).x(),
//
//				-framePosition->at(i).z(),
//
//				framePosition->at(i).y()
//
//				);
//
//			osg::Vec3 xyz = getWindowPoint(_camera, oxyz);//变换为屏幕坐标
//
//
//
//			//_bvhPoints->push_back(osg::Vec3(xyz.x(), (WIN_HEIGHT - xyz.y()), outz));
//
//
//
//			output << xyz.x() << " " << (WIN_HEIGHT - xyz.y()) << " " << xyz.z() << " "
//
//				<< oxyz.x() << " " << oxyz.y() << " " << oxyz.z() << " "
//
//				////<< currNode.NodeName 
//
//				<< std::endl;
//
//
//
//		}//end for
//
//		output.close();
//
//
//
//	}
//
//
//
//
//
//	virtual void operator () (osg::RenderInfo& renderInfo) const
//
//	{
//
//		static bool bDrawSwith = false;
//
//
//
//		if (__bReplacedNode)
//
//		{
//
//			if (bDrawSwith)
//
//			{
//
//				ComputeBVHPoint();
//
//				ComputeDepthImage();
//
//
//
//				__bReplacedNode = false;
//
//			}
//
//
//
//			bDrawSwith = !bDrawSwith;
//
//
//
//		}
//
//		//std::cout<<"draw call back"<<std::endl;
//
//	}
//
//};
//
//
//
//
//
//int main(int argc, char *argv[])
//
//{
//
//
//
//	//-------------------node----------------------
//
//	osg::ref_ptr<osg::Node> node;
//
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	node = osgDB::readNodeFile("E:/DATASET/cmu-mocap-master/obj/001/01_01_0.obj");
//
//
//
//	root->addChild(node);
//
//	root->setDataVariance(osg::Object::DYNAMIC);
//
//
//
//	//-------------------coordinate----------------------
//
//	//root->addChild(geodeCoordinate());
//
//	//-------------------viewer----------------------
//
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//
//
//
//	viewer->setSceneData(root);
//
//	viewer->setUpViewInWindow(100, 100, WIN_WIDTH, WIN_HEIGHT);
//
//
//
//	//-------------------camera----------------------
//
//	osg::ref_ptr<osg::Camera> rttCamera = viewer->getCamera();
//
//
//
//	osg::ref_ptr<osg::Image> depthbufferimg = new osg::Image;
//
//	depthbufferimg->allocateImage(WIN_WIDTH, WIN_HEIGHT, 1, GL_DEPTH_COMPONENT, GL_FLOAT);
//
//	osg::ref_ptr<osg::Image> colorbufferimg = new osg::Image;
//
//	colorbufferimg->allocateImage(WIN_WIDTH, WIN_HEIGHT, 1, GL_RGBA, GL_UNSIGNED_BYTE);
//
//
//
//	rttCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//	rttCamera->attach(osg::Camera::DEPTH_BUFFER, depthbufferimg.get(), 0, 0);
//
//	rttCamera->attach(osg::Camera::COLOR_BUFFER, colorbufferimg.get(), 0, 0);
//
//
//
//	//-------------------camera set----------------------
//
//	rttCamera->setClearColor(Vec4(1.0, 1.0, 1.0, 1.0));
//
//
//
//	//osg::Vec3 vPosEye(0, -1.5, 0.4), vCenter(0, -0.5, 0.4), vUp(0, 0, 1);
//
//	osg::Vec3 vPosEye(0, -1.0, 0.4), vCenter(0, -0.0, 0.4), vUp(0, 0, 1);
//
//	rttCamera->setViewMatrixAsLookAt(vPosEye, vCenter, vUp);
//
//	rttCamera->setProjectionMatrixAsPerspective(58, WIN_WIDTH*1.0 / WIN_HEIGHT, 0.0001, 1000);//kinect fov
//
//
//
//	//-------------------event----------------------aaaaaaaaaaaa
//
//	viewer->addEventHandler(new DepthHld());
//
//
//
//	//-------------------light off----------------------
//
//	viewer->setLightingMode(osg::View::NO_LIGHT);
//
//	osg::ref_ptr<osg::StateSet> globalStateset = viewer->getCamera()->getStateSet();
//
//	osg::ref_ptr<osg::LightModel> lightModel = new osg::LightModel();
//
//	lightModel->setAmbientIntensity(osg::Vec4(0, 0, 0, 0));
//
//	globalStateset->setAttributeAndModes(lightModel, osg::StateAttribute::ON);
//
//
//
//	//-------------------call back----------------------
//
//	root->setUpdateCallback(new ObjCallBack(root));
//
//	rttCamera->setFinalDrawCallback(new CaptureCallback(rttCamera, depthbufferimg, colorbufferimg));//必须放viewer设置最后
//
//
//
//	while (!viewer->done())
//
//	{
//
//		viewer->frame();
//
//	}
//
//
//
//	return 0;
//
//}
//
