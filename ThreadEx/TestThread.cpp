#include "Thread.h"
#include <iostream>
#include <unistd.h>

using namespace std;

void threadfunc(int y)
{
	int x = 5;
	while (x--)
	{
		cout << x << endl;
		cout << "y:" << y << endl;
		sleep(1);
	}
}


int main(void)
{
	Thread tt(bind(threadfunc, 10)); //
	tt.start();
	tt.join();
}
