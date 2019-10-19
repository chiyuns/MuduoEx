#include "Thread.h"

Thread::Thread(const threadfunc &func):func_(func),autodelete_(false)
{
	cout << "Thread()" << endl;
}


Thread::~Thread()
{
	cout << "~Thread()" << endl;
}

void Thread::start()
{
	pthread_create(&thread_id, NULL, xxx, this);
}

void Thread::join()
{
	pthread_join(thread_id, NULL);
}

void* Thread::xxx(void *arg)
{
	Thread *thread = static_cast<Thread*>(arg);
	thread->run(); // 线程函数内执行run()
	if (thread->autodelete_)
	{
		delete  thread;
	}
	return NULL;
}


void Thread::setAutoDelete(bool autodelete)
{
	autodelete_ = autodelete;
}

void Thread::run()
{
	func_(); //执行绑定函数
}
