// dead lock

#include <iostream>
#include <thread>
#include <mutex>
#include "Save.h"

using namespace std;

int a, b;
mutex m_a, m_b;

void thread_ab()
{
	while (true)
	{
		lock_guard<mutex> al(m_a);
		a++;

		lock_guard<mutex> bl(m_b);
		b++;
		cout << "T1 Loop:" << endl;
	}
}

void thread_ba()
{
	while (true)
	{
		lock_guard<mutex> bl(m_b);
		b++;

		lock_guard<mutex> al(m_a);
		a++;
		cout << "T2 Loop:" << endl;
	}
}

int main()
{
	save();

	thread t1{ thread_ab };
	thread t2{ thread_ba };

	t1.join();
	t2.join();

	return 0;
}