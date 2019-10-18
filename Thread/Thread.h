#pragma once

#include <pthread.h>
class Thread
{
public:
	Thread();
	virtual  ~Thread();
	void start();
	void join();
	void setAutoDelete(bool autodelete);
private:
	virtual  void run() = 0;
	pthread_t thread_id;
	static void* ThreadFunc(void *arg); //静态的线程函数,如果不是静态成员函数，那么编译函数会变成void* ThreadFunc(void *arg, Thread *this),哈哈，不符合规定了
	bool  autodelete_;
};
