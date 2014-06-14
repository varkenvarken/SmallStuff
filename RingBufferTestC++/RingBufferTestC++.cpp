// RingBufferTestC++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "RingBuffer.h"

using namespace ExtraContainers;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string line;
	RingBuffer<int, 5> buffer;

	for (int i = 0; i < 12; i++){ buffer.Add(i); }
	std::cout << "buffer contains:";
	for (auto item : buffer){
		cout << ' ' << item;
	}
	cout << endl;
	cin >> line;
	return 0;
}

