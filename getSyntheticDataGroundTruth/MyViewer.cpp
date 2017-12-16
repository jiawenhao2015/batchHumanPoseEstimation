#include "MyViewer.h"
#include <iostream>

MyViewer::MyViewer(OpenThreads::Mutex *m):Viewer()
{
	mutex = m;
}

//int MyViewer::run()
//{
//	while(!done())
//	{
//		//mutex->lock();
//		Viewer::frame();
//		//mutex->unlock();
//	}
//
//	return 1;
//}
//
