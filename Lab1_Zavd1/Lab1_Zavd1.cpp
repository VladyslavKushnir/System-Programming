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
		printf("Помилка! Аргумент не вказано!\n");
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
				printf("Забагато вiльної пам'яті!");
			}


		}
		else if (strcmp(argv[1], "-s") == 0)
		{
			SYSTEM_INFO SystemInfo;
			GetSystemInfo(&SystemInfo);
			printf("Системна iнформацiя:\n");
			printf(" - архiтектура процесора - %u\n", SystemInfo.wProcessorArchitecture);
			printf(" - тип процесора - %u\n", SystemInfo.dwProcessorType);
			printf(" - молодша адреса пам'ятi - %p\n", SystemInfo.lpMinimumApplicationAddress);
			printf(" - старша адреса пам'ятi - %p\n", SystemInfo.lpMaximumApplicationAddress);
			printf(" - розмiр сторiнки - %u\n", SystemInfo.dwPageSize);
			MEMORYSTATUS MemoryStat;
			GlobalMemoryStatus(&MemoryStat);
			printf("Iнформацiя про пам'ять:\n");
			printf(" - використання фiзичної пам'ятi(percent) - %u\n", MemoryStat.dwMemoryLoad);
			printf(" - об'єм фiзичної пам'ятi(bytes) - %u\n", MemoryStat.dwTotalPhys);
			printf(" - об'єм вiртуальної пам'ятi(bytes) - %u\n", MemoryStat.dwTotalVirtual);
			printf(" - поточний розмiр видiленої пам'ятi(bytes) - %u\n", MemoryStat.dwTotalPageFile);
			printf(" - максимальний розмiр видiленої пам'ятi(bytes) - %u\n", MemoryStat.dwAvailPageFile);
		}
		else
		{
			printf("Помилка! Аргумент не вказано!\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}






