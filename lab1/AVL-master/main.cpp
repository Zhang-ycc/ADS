#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <windows.h>
#include <string>

#include "AVL.h"
using namespace std;

#define m 20000

int main(int argc, char* argv[])
{
	AVL* root = NULL;
	string file = "./input/input";
	ifstream in(file);
	int key[m];
	for (int i = 0; i < m; i++) {
		in >> key[i];
	}

	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	for (int i = 0; i < m; i++)
		Insert(root, key[i]);
	QueryPerformanceCounter(&t2);
	cout <<"Insert Time: " << 1000 * ((t2.QuadPart - t1.QuadPart) * 1.0 / tc.QuadPart) << endl;

	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	for (int i = 0; i < m; i++)
		Search(root, key[i]);
	QueryPerformanceCounter(&t2);
	cout << "Search Time: " << 1000 * ((t2.QuadPart - t1.QuadPart) * 1.0 / tc.QuadPart) << endl;

	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	for (int i = 0; i < m; i++)
		Delete(root, i);
	QueryPerformanceCounter(&t2);
	cout << "Delete Time: " << 1000 * ((t2.QuadPart - t1.QuadPart) * 1.0 / tc.QuadPart) << endl;


	return 0;
}
