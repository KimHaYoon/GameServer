#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

int g_data;
bool flag = false;
mutex f_m;

void recv()
{
	f_m.lock();
	while (false == flag)
	{
		f_m.unlock();
		f_m.lock();
	}
	f_m.unlock();

	cout << "l got " << g_data << endl;
}

void send()
{
	g_data = 999;
	f_m.lock();
	flag = true;
	f_m.unlock();
}

int main()
{
	thread receiver{ recv };
	thread sender{ send };
	receiver.join();
	sender.join();

	return 0;
}