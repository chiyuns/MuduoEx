#pragma once
#include <functional>
#include <pthread.h>
#include <iostream>
using namespace std;

class Thread
{
public:
	typedef function<void ()> threadfunc;
	Thread(const threadfunc& func);
	~Thread();
	void start();
	void join();
	void setAutoDelete(bool autodelete);
private:
	bool autodelete_;
	threadfunc  func_;
	pthread_t thread_id;
	void run();
	static void* xxx(void* arg);

};
