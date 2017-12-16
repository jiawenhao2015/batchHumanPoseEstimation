#pragma once

#include <OpenThreads/Thread>

class MyThread : public OpenThreads::Thread
{
public:
	MyThread(){}

	virtual void run();
	
};