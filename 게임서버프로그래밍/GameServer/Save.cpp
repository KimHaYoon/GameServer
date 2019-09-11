#include "Save.h"

#include <fstream>
#include <algorithm>
#include <iterator>
#include <chrono>

void save(string FileName)
{
	// FileName�� �б���� ����.

	ifstream in(FileName);
	if (!in)
		return;

	// ����� ������ ���ٿ����� ���� ����.
	ofstream out("../���Ӽ������α׷���.txt", ios::app);		// std::ios::app : ���ٿ����� ���

	// ����� �ð��� ����Ѵ�.
	// �ð����(1970.01.01 ��������)���� �� tic�� �귶����?
	auto tic = chrono::system_clock::now();

	// �� ���� time_t�� ��ȯ
	auto time = chrono::system_clock::to_time_t(tic);

	// �� ���� ���ڷ� ��ȯ
	string date = ctime(&time);

	out << endl << endl;
	out << "========================" << endl;
	out << date;
	out << "========================" << endl << endl;

	// FileName�� ��� ������ ���ٿ� ����.
	copy(istreambuf_iterator<char>(in), istreambuf_iterator<char>(), ostreambuf_iterator<char>(out));	// copy(������, ������, ����);
}
