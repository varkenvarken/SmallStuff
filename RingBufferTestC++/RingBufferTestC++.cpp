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
	std::cout << "buffer contains (iterator, oldest to newest):";
	for (auto item : buffer){
		cout << ' ' << item;
	}
	cout << endl;
	std::cout << "buffer contains (reverse iterator, newest to oldest):";
	for (auto rit = buffer.rbegin(); rit != buffer.rend(); ++rit){
		cout << ' ' << *rit;
	}
	cout << endl;
	std::cout << "buffer contains (reverse iterator, newest to oldest, post increment):";
	for (auto rit = buffer.rbegin(); rit != buffer.rend(); cout << ' ' << *rit++) continue;
	cout << endl;
	std::cout << "buffer contains (indexed access, newest to oldest):";
	for (int i = 0; i < (int)buffer.Count(); i++){
		cout << ' ' << buffer[i];
	}
	cout << endl;
	std::cout << "buffer contains (indexed access, oldest to newest):";
	for (int i = buffer.Oldest(); i<=0; i++){
		cout << ' ' << buffer[i];
	}
	cout << endl;

	cin >> line;
	return 0;
}

