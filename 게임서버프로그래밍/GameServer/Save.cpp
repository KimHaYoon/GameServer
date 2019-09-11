#include "Save.h"

#include <fstream>
#include <algorithm>
#include <iterator>
#include <chrono>

void save(string FileName)
{
	// FileName을 읽기모드로 연다.

	ifstream in(FileName);
	if (!in)
		return;

	// 기록할 파일을 덧붙여쓰기 모드로 연다.
	ofstream out("../게임서버프로그래밍.txt", ios::app);		// std::ios::app : 덧붙여쓰기 모드

	// 실행된 시간을 기록한다.
	// 시간기원(1970.01.01 영국기준)에서 몇 tic이 흘렀는지?
	auto tic = chrono::system_clock::now();

	// 이 값을 time_t로 변환
	auto time = chrono::system_clock::to_time_t(tic);

	// 이 값을 글자로 변환
	string date = ctime(&time);

	out << endl << endl;
	out << "========================" << endl;
	out << date;
	out << "========================" << endl << endl;

	// FileName의 모든 내용을 덧붙여 쓴다.
	copy(istreambuf_iterator<char>(in), istreambuf_iterator<char>(), ostreambuf_iterator<char>(out));	// copy(어디부터, 어디까지, 어디로);
}
