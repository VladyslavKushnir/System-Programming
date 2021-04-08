#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <locale.h>

void main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "ukr");
	while (1) {
		printf("\nМеню:\n");
		printf("Змiнити директорiю: cd 'path'\n");
		printf("Друк директорiї: print\n");
		printf("Копiювати файл: copy\n");
		printf("Створити директорiю: newdir\n");
		printf("Видалити файл: remfile\n");
		printf("Видалити порожню директорiю: remdir\n");
		printf("Iнформацiя про файл: info\n");
		char command[20];
		printf("\nВаша дiя >> ");
		scanf_s("%s", command, 20);

		if (strcmp(command, "cd") == 0) {

				DWORD check;
				char DirPath[MAX_PATH];
				char DirName[MAX_PATH];
				scanf_s("%s", DirPath, MAX_PATH);
				if (SetCurrentDirectoryA((LPCSTR)DirPath))
				{
					check = GetCurrentDirectoryA(MAX_PATH, DirName);
					if (check != 0) {
						printf("Поточний каталог => %s\n", DirName);
						printf("\n");
					}
					else {
						printf("Не вiрно вказано шлях!\n");
					}
				}
				else {
					printf("Щось не так!\n");
				}

		}
		else if (strcmp(command, "print") == 0) {

			WIN32_FIND_DATAA FileData;
			HANDLE find;
			char DirName[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, DirName);
			find = FindFirstFileA(strcat(DirName, "\\*"), &FileData);
			if (find == INVALID_HANDLE_VALUE) {
				printf("Щось не так!\n");
			}
			else {
				do {
					printf("\n");
					SYSTEMTIME systemTime;
					FileTimeToSystemTime(&FileData.ftCreationTime, &systemTime);
					printf("Iм'я: %s\n", (char*)FileData.cFileName);
					printf("Час створення: %02d-%02d-%d %02d:%02d:%02d\n",
						systemTime.wMonth, systemTime.wDay, systemTime.wYear,
						systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
					printf("Розмiр в байтах: %d\n", FileData.nFileSizeLow);
				} while (FindNextFileA(find, &FileData) != 0);
				FindClose(find);
			}
		}
		else if (strcmp(command, "copy") == 0) {
			
			char file_path_1[MAX_PATH];
			char file_path_2[MAX_PATH];
			printf("Шлях до файлу, який необхiдно скопiювати: ");
			scanf_s("%s", file_path_1, MAX_PATH);
			printf("Шлях у мiсце копiювання: ");
			scanf_s("%s", file_path_2, MAX_PATH);
			if (CopyFileA((LPCSTR)file_path_1, (LPCSTR)file_path_2, FALSE)) {
				printf("Файл скопiйовано!\n");
			}
			else {
				printf("Шлях не вiрний або не iснує!\n");
			}
		}
		else if (strcmp(command, "newdir") == 0) {

			char dir_name[MAX_PATH];
			printf("Введiть iм'я директорiї: ");
			scanf_s("%s", dir_name, MAX_PATH);
			if (CreateDirectoryA((LPCSTR)dir_name, NULL)) {
				printf("Директорiя створена!\n");
			}
			else {
				printf("Щось не так!\n");
			}
		}
		else if (strcmp(command, "remdir") == 0) {
			
			char dir_path[MAX_PATH];
			printf("Введiть шлях до директорiї: ");
			scanf_s("%s", dir_path, MAX_PATH);
			if (RemoveDirectoryA(dir_path)) {
				printf("Директорiя видалена!\n");
			}
			else {
				printf("Щось не так!\n");
			}
		}
		else if (strcmp(command, "remfile") == 0) {
			
			char file_path[MAX_PATH];
			printf("Введiть шлях до файлу: ");
			scanf_s("%s", file_path, MAX_PATH);
			if (DeleteFileA(file_path)) {
				printf("Файл видалено!\n");
			}
			else {
				printf("Щось не так!\n");
			}
		}
		else if (strcmp(command, "info") == 0) {

			char file_path[MAX_PATH];
			BY_HANDLE_FILE_INFORMATION lpFileInformation;
			printf("Введiть шлях до файлу: ");
			scanf_s("%s", file_path, MAX_PATH);
			HANDLE file = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (GetFileInformationByHandle(file, &lpFileInformation)) {
				wprintf(L"Атрибут файлу: %d\n", lpFileInformation.dwFileAttributes);
				SYSTEMTIME systemTime;
				FileTimeToSystemTime(&(lpFileInformation.ftCreationTime), &systemTime);
				wprintf(L"Час створення: %02d-%02d-%d %02d:%02d:%02d\n",
					systemTime.wMonth, systemTime.wDay, systemTime.wYear,
					systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				FileTimeToSystemTime(&(lpFileInformation.ftLastAccessTime), &systemTime);
				wprintf(L"Час останнього доступу: %02d-%02d-%d %02d:%02d:%02d\n",
					systemTime.wMonth, systemTime.wDay, systemTime.wYear,
					systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				FileTimeToSystemTime(&(lpFileInformation.ftLastWriteTime), &systemTime);
				wprintf(L"Час останнього запису: %02d-%02d-%d %02d:%02d:%02d\n",
					systemTime.wMonth, systemTime.wDay, systemTime.wYear,
					systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				wprintf(L"Число посилань: %d\n", lpFileInformation.nNumberOfLinks);
				wprintf(L"Серiйний номер тома: %d\n", lpFileInformation.dwVolumeSerialNumber);
			}
			else {
				printf("Щось не так!\n");
			}
			CloseHandle(file);

		}
		else {
			printf("\nДiя вiдсутня!");
		}
	}
	exit(0);
}