#include "windows.h"
#include <iostream>
using namespace std;

void main()
{
	HANDLE mut;
	mut = CreateMutex(NULL, FALSE, L"Lab03Zavd1");
	DWORD result;
	result = WaitForSingleObject(mut, 0);
	if (result == WAIT_OBJECT_0)
	{
		cout << "Is Running" << endl;
		int i;
		cin >> i;
		ReleaseMutex(mut);
	}
	else
		cout << "Fail programm running" << endl;
	system("pause");
	CloseHandle(mut);
}