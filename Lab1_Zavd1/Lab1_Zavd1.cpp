#include <stdio.h>
#include "windows.h"
#include "iostream"
#include <locale.h>

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ukr");
	if (argc != 2)
	{
		printf("�������! �������� �� �������!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		if (strcmp(argv[1], "-e") == 0)
		{
			MEMORYSTATUS MemoryStatus;
			GlobalMemoryStatus(&MemoryStatus);
			HLOCAL pAllocMem = LocalAlloc(LMEM_FIXED, MemoryStatus.dwTotalVirtual);
			if (pAllocMem == NULL)
			{
				LPVOID lpMsgBuf;
				DWORD dwLastError = GetLastError();
				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					NULL, dwLastError,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);
				printf("\n%s\n", lpMsgBuf);
				LocalFree(lpMsgBuf);
			}
			else
			{
				printf("�������� �i���� ���'��!");
			}


		}
		else if (strcmp(argv[1], "-s") == 0)
		{
			SYSTEM_INFO SystemInfo;
			GetSystemInfo(&SystemInfo);
			printf("�������� i�������i�:\n");
			printf(" - ���i������� ��������� - %u\n", SystemInfo.wProcessorArchitecture);
			printf(" - ��� ��������� - %u\n", SystemInfo.dwProcessorType);
			printf(" - ������� ������ ���'��i - %p\n", SystemInfo.lpMinimumApplicationAddress);
			printf(" - ������ ������ ���'��i - %p\n", SystemInfo.lpMaximumApplicationAddress);
			printf(" - ����i� ����i��� - %u\n", SystemInfo.dwPageSize);
			MEMORYSTATUS MemoryStat;
			GlobalMemoryStatus(&MemoryStat);
			printf("I�������i� ��� ���'���:\n");
			printf(" - ������������ �i����� ���'��i(percent) - %u\n", MemoryStat.dwMemoryLoad);
			printf(" - ��'�� �i����� ���'��i(bytes) - %u\n", MemoryStat.dwTotalPhys);
			printf(" - ��'�� �i�������� ���'��i(bytes) - %u\n", MemoryStat.dwTotalVirtual);
			printf(" - �������� ����i� ���i���� ���'��i(bytes) - %u\n", MemoryStat.dwTotalPageFile);
			printf(" - ������������ ����i� ���i���� ���'��i(bytes) - %u\n", MemoryStat.dwAvailPageFile);
		}
		else
		{
			printf("�������! �������� �� �������!\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}






