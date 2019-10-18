#include "Thread.h"
#include <iostream>

using namespace  std;

Thread::Thread()
{
	cout << "thread()" << endl;
}


Thread::~Thread()
{
	cout << "~Thread()" << endl;
}


void Thread::start()
{
	pthread_create(&thread_id, NULL, ThreadFunc, this);
}


void Thread::join()
{
	pthread_join(thread_id, NULL);
}

void *Thread::ThreadFunc(void *arg)
{
	Thread* thread = static_cast<Thread*>(arg);
	thread->run();
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
