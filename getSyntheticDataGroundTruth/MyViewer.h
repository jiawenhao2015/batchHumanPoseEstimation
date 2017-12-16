#pragma once
#include <Windows.h>
#include <osgViewer/Viewer>
#include <OpenThreads/Mutex>

class MyViewer: public osgViewer::Viewer
{
	OpenThreads::Mutex *mutex;
public:
	MyViewer(OpenThreads::Mutex *m);

	//int run();
};