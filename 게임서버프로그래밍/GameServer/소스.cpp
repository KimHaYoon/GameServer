#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include "Save.h"

using namespace std;
using namespace chrono;

const int g_iMaxCount = 150000;
vector<int> g_vecPrimes;
int g_count = 0;
mutex g_pl;
mutex g_cl;

bool IsPrimeNumber(int iNumber)
{
	if (iNumber == 1)
		return false;

	if ((iNumber % 2) == 0)
		return false;

	if (iNumber == 2)
		return true;

	for (int i = 3; i < iNumber - 1; i += 2)		 // 짝수만 검사
	{
		if ((iNumber % i) == 0)
			return false;
	}

	return true;
}

void thread_func()
{
	while (true)
	{
		int num;
		{
			lock_guard<mutex> ll(g_cl);
			num = g_count;
			++g_count;

			if (g_count > g_iMaxCount)
				break;
		}

		if (IsPrimeNumber(num))
		{
			g_pl.lock();
			g_vecPrimes.push_back(num);
			g_pl.unlock();
		}
	}
}

int main()
{
	vector<thread> threads;
	auto t0 = system_clock::now();
	for (int i = 0; i < 4; ++i)
		threads.emplace_back(thread_func);

	for (auto& th : threads)
		th.join();			// 이 스레드가 끝날 때까지 기다려라

	cout << "Number of Prime less than " << g_iMaxCount << " is " << g_vecPrimes.size() << endl;

	auto t1 = system_clock::now();

	auto duration = duration_cast<milliseconds>(t1 - t0).count();
	cout << "Took " << duration << " milliseconds." << endl;

	system("pause");

	save("소스.cpp");
	return 0;
}