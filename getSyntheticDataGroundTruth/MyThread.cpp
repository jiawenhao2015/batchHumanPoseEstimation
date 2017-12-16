#include <Windows.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/Image>

#include "MyThread.h"
#include <direct.h>


extern char *_dirName_;
extern char *_frontName_;
extern char *_extenName_;

extern int _index_;
extern int _step_;
extern OpenThreads::Mutex *mutex;
extern osg::Group *_root;

void MyThread::run()
{
	std::ostringstream dirName;
	osg::ref_ptr<osg::Node> node = NULL;

	//do 
	{
		SYSTEMTIME t;
		GetLocalTime(&t);
		dirName<<_dirName_;
		dirName<<"Depth/"<<t.wYear<<"-"<<t.wMonth<<"-"<<t.wDay<<" "<<t.wHour;
		_mkdir(dirName.str().c_str());

		std::ostringstream ostr;
		ostr<<_frontName_;
		ostr<<_index_;// + _step_;
		ostr<<_extenName_;

		mutex->lock();
		_root->removeChild(0,_root->getNumChildren());

		node = osgDB::readNodeFile(ostr.str());
		if(node)
		{
			_root->addChild(node);
		
		}
		mutex->unlock();

		_index_ += _step_;
		std::cout<<"--Thread--"<<std::endl;
		//Sleep(1000);
	}// while(node);


}