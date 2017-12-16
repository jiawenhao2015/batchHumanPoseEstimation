#include <Windows.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Image>
#include <OpenThreads/Mutex>
#include <osgGA/TrackballManipulator>

#include <direct.h>

#include "MyViewer.h"
#include "MyThread.h"

OpenThreads::Mutex *mutex;
osg::Group *_root;

//char *_frontName_ = "3ds/obj1/pz__";
//char *_frontName_ = "F:/KINECT/PROJECT/DepthAcquisition/3ds/obj/Andy/pz__";
char *_frontName_ = "F:/KINECT/PROJECT/DepthAcquisition/3ds/obj2/pz_2_0_";
char *_dirName_ = "F:/KINECT/PROJECT/DepthAcquisition/";

//char *_frontName_ = "3ds/output/gz_0_";
char *_extenName_ = ".obj";
//char *_extenName_ = ".3ds";

int _step_ = 1;
int _start_  = 0;
int _index_ = _start_;
int _end_ = 1000;


int _winWidth_ = 800;
int _winHeight_ = 600;


//信号量
class SIGN{
public:
	int _event_start;
public:
	SIGN()
	{
		_event_start = false;
	}
};

//事件处理
class DepthHandler: public osgGA::GUIEventHandler
{
private:
	SIGN* _sign;
	osg::Image *_image;

public:
	DepthHandler(osg::Image *image,SIGN *s):_image(image),_sign(s)
	{
		
	}

	~DepthHandler()
	{

	}

	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) 
	{
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>( &aa ); 
		if (!viewer) return false; 

		switch(ea.getEventType()) 
		{ 
		case(osgGA::GUIEventAdapter::KEYDOWN): 
			{
				if(ea.getKey() == 'a')
				{ 
					//_sign->_event_start= 1;
					_sign->_event_start = !_sign->_event_start;
					return true;
				}
				if(ea.getKey() == 's')
				{ 
					std::ostringstream outputName;

					SYSTEMTIME t;
					GetLocalTime(&t);

					outputName<<_dirName_;
					outputName<<"Depth/"<<t.wYear<<"-"<<t.wMonth<<"-"<<t.wDay;
					_mkdir(outputName.str().c_str());

					outputName<<"/"<<t.wHour<<"_"<<t.wMinute<<"_"<<t.wMilliseconds<<".bmp";//<<"_"<<t.wSecond

					osgDB::writeImageFile(*_image,outputName.str());	//输出深度图


					return true;
				}
				if(ea.getKey() == 'd')
				{ 
					MyThread *myThread = new MyThread();
					myThread->run();
					_sign->_event_start = 2;

					return true;
				}
			}
			default: 
				return false; 
		}
	}
};

class DepthCallBack: public osg::Camera::DrawCallback
{
public:
	SIGN *_sign;
	osg::Image *_image;

	osg::Group *_root;

public:
	DepthCallBack(osg::Image *image,SIGN *s,osg::Group *root):_image(image),_sign(s),_root(root)
	{
		
	}
	~DepthCallBack()
	{

	}

	void operator() (const osg::Camera &camera) const 
	{
		if(_sign->_event_start == 1)
		{
			/*
			std::ostringstream outputName;

			SYSTEMTIME t;
			GetLocalTime(&t);

			outputName<<_dirName_;
			outputName<<"Depth/"<<t.wYear<<"-"<<t.wMonth<<"-"<<t.wDay;
			_mkdir(outputName.str().c_str());

			outputName<<"/"<<t.wHour<<"_"<<t.wMinute<<"_"<<t.wMilliseconds<<".bmp";//<<"_"<<t.wSecond

			osgDB::writeImageFile(*_image,outputName.str());	//输出深度图
			*/
			std::ostringstream dirName;

			SYSTEMTIME t;
			GetLocalTime(&t);
			dirName<<_dirName_;
			dirName<<"Depth/"<<t.wYear<<"-"<<t.wMonth<<"-"<<t.wDay<<" "<<t.wHour;
			_mkdir(dirName.str().c_str());

			std::ostringstream outputName;
			outputName<<dirName.str();
			outputName<<"/"<<_index_<<".bmp";//<<"_"<<t.wSecond

			osgDB::writeImageFile(*_image,outputName.str());	//输出深度图

			/*********************RGB图片********************************/
			int x, y, width, height;

			x = (int) camera.getViewport ()->x() ;// + 150;   
			y = (int) camera.getViewport()->y()  ;// + 100;
			width = (int) camera.getViewport()->width();// - 300; 
			height = (int) camera.getViewport()->height();// - 100;

			std::ostringstream outputName2;
			outputName2<<dirName.str();
			//outputName2<<"/rgb_"<<_index_<<".bmp";
			if (_index_<10)
			{
				outputName2<<"/img0000"<<_index_<<".png";
			}else
			{
				outputName2<<"/img000"<<_index_<<".png";
			}

			osg::Image * imageBuffer = new osg::Image();
			imageBuffer->readPixels(x, y, width, height,GL_RGBA,GL_UNSIGNED_BYTE); 
			osgDB::writeImageFile(*imageBuffer,outputName2.str());
			/*********************************************************/
			_sign->_event_start = 0;

		}else if (_sign->_event_start == 2)
		{
			std::ostringstream dirName;

			SYSTEMTIME t;
			GetLocalTime(&t);
			dirName<<_dirName_;
			dirName<<"Depth/"<<t.wYear<<"-"<<t.wMonth<<"-"<<t.wDay<<" "<<t.wHour;
			_mkdir(dirName.str().c_str());

			std::ostringstream ostr;
			ostr<<_frontName_;
			ostr<<_index_;// + _step_;
			ostr<<_extenName_;

			//mutex->lock();
			//_root->removeChild(0,_root->getNumChildren());

			//osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(ostr.str());
			//if(node)
			//{
			//	_root->addChild(node);
				//mutex->unlock();
				std::ostringstream outputName;
				outputName<<dirName.str();
				outputName<<"/"<<_index_<<".bmp";//<<"_"<<t.wSecond

				osgDB::writeImageFile(*_image,outputName.str());	//输出深度图

				
				/*********************RGB图片*********************************/
				int x, y, width, height;

				x = (int) camera.getViewport ()->x() ;// + 150;   
				y = (int) camera.getViewport()->y()  ;// + 100;
				width = (int) camera.getViewport()->width();// - 300; 
				height = (int) camera.getViewport()->height();// - 100;

				std::ostringstream outputName2;
				outputName2<<dirName.str();
				//outputName2<<"/rgb_"<<_index_<<".bmp";
				if (_index_<10)
				{
					outputName2<<"/img0000"<<_index_<<".png";
				}else
				{
					outputName2<<"/img000"<<_index_<<".png";
				}
				osg::Image * imageBuffer = new osg::Image();
				imageBuffer->readPixels(x, y, width, height,GL_RGBA,GL_UNSIGNED_BYTE); 
				osgDB::writeImageFile(*imageBuffer,outputName2.str());
				/****************************************************/
				
				std::cout<<"-------------"<<_index_<<"----------------------\n";
			//}else{
			//	//mutex->unlock();
			//	_sign->_event_start = 0;
			//}

			//_index_ += _step_;

			_sign->_event_start = 0;
		}
	}
};

int main0(int argc,char* argv[])
{
	mutex = new OpenThreads::Mutex();
	SIGN *sign = new SIGN;
	MyViewer *viewer = new MyViewer(mutex);
	//osgViewer::Viewer viewer; 
	std::ostringstream ostr;


	if(argc>1)
	{
		_start_ = atoi(argv[1]);
		_index_ = _start_;
	}

	if (argc>2)
	{
		_frontName_ = argv[2];
		//char *_dirName_ = "F:/KINECT/PROJECT/DepthAcquisition/";a
	}


	ostr<<_frontName_;
	ostr<<_start_;// + _step_;
	ostr<<_extenName_;

	_root = new osg::Group;
	//root->setDataVariance(osg::Object::DYNAMIC);

	_root->setDataVariance(osg::Object::DYNAMIC);
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(ostr.str());
	//root->setUpdateCallback(new RootCallBack(sign,root));
	_root->addChild(node);

	viewer->getCamera()->setClearMask(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ); 
	viewer->setSceneData(_root);

	osg::Image *image = new osg::Image();
	image->allocateImage(_winWidth_, _winHeight_, 1,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE);
	//viewer->getCamera()->attach(osg::Camera::DEPTH_BUFFER,image);
	viewer->getCamera()->attach(osg::Camera::PACKED_DEPTH_STENCIL_BUFFER,image);

	viewer->addEventHandler(new DepthHandler(image,sign));
	viewer->getCamera()->setPreDrawCallback(new DepthCallBack(image,sign,_root));

	viewer->setUpViewInWindow(100,100,_winWidth_,_winHeight_);
	viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	//viewer->run();

	osg::DisplaySettings::instance()->setNumMultiSamples(16);

	while(!viewer->done())
	{
		mutex->lock();
		viewer->frame();
		mutex->unlock();
		//Sleep(1000);
	}

	/***************************深度图*************************a
	std::ostringstream dirName;

	SYSTEMTIME t;
	GetLocalTime(&t);
	dirName<<_dirName_;
	dirName<<"Depth/"<<t.wYear<<"-"<<t.wMonth<<"-"<<t.wDay<<" "<<t.wHour;
	_mkdir(dirName.str().c_str());

	std::ostringstream outputName;
	outputName<<dirName.str();
	outputName<<"/"<<_index_<<".bmp";//<<"_"<<t.wSecond

	osgDB::writeImageFile(*image,outputName.str());	//输出深度图

	/*********************RGB图片********************************
	int x, y, width, height;

	x = (int) viewer.getCamera()->getViewport ()->x() ;// + 150;   
	y = (int) viewer.getCamera()->getViewport()->y()  ;// + 100;
	width = (int) viewer.getCamera()->getViewport()->width();// - 300; 
	height = (int) viewer.getCamera()->getViewport()->height();// - 100;

	std::ostringstream outputName2;
	outputName2<<dirName.str();
	outputName2<<"/rgb_"<<_index_<<".bmp";
	osg::Image * imageBuffer = new osg::Image();
	imageBuffer->readPixels(x, y, width, height,GL_RGBA,GL_UNSIGNED_BYTE); 
	osgDB::writeImageFile(*imageBuffer,outputName2.str());
	/****************************************************

	std::cout<<"------------------"<<_index_<<"----------------------\n";
	//exit(0);
	/*************************************************************/
	return 0;
}