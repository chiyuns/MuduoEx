#include <iostream>
#include "Thread.h"
#include <unistd.h>

using namespace std;

class TestThread :public Thread
{
public:
	TestThread()
	{
		cout << "TestThread()" << endl;
	}
	~TestThread()
	{
		cout << "~TestThread()" << endl;
	}
private:
	void run()
	{
		int i = 5;
		while (i--)
		{
			cout << i << endl;
			sleep(1);
		}
	}
};


int main()
{
	TestThread   *tt = new TestThread();
	tt->setAutoDelete(true);
	tt->start();
	tt->join();
	return 0;
}
