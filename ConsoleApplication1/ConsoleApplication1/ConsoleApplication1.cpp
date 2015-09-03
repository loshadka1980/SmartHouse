// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 0;
	cout << "A = ";
	cin >> a;

	int b = 0;
	cout << "B = ";
	cin >> b;

	int c = 0;
	c = a + b;
	cout << "A+B = " << c << endl;

	_getch();

	return 0;
}

