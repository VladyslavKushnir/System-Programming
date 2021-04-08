#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <locale.h>

void main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "ukr");
	while (1) {
		printf("\n����:\n");
		printf("��i���� ��������i�: cd 'path'\n");
		printf("���� ��������i�: print\n");
		printf("���i����� ����: copy\n");
		printf("�������� ��������i�: newdir\n");
		printf("�������� ����: remfile\n");
		printf("�������� ������� ��������i�: remdir\n");
		printf("I�������i� ��� ����: info\n");
		char command[20];
		printf("\n���� �i� >> ");
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
						printf("�������� ������� => %s\n", DirName);
						printf("\n");
					}
					else {
						printf("�� �i��� ������� ����!\n");
					}
				}
				else {
					printf("���� �� ���!\n");
				}

		}
		else if (strcmp(command, "print") == 0) {

			WIN32_FIND_DATAA FileData;
			HANDLE find;
			char DirName[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, DirName);
			find = FindFirstFileA(strcat(DirName, "\\*"), &FileData);
			if (find == INVALID_HANDLE_VALUE) {
				printf("���� �� ���!\n");
			}
			else {
				do {
					printf("\n");
					SYSTEMTIME systemTime;
					FileTimeToSystemTime(&FileData.ftCreationTime, &systemTime);
					printf("I�'�: %s\n", (char*)FileData.cFileName);
					printf("��� ���������: %02d-%02d-%d %02d:%02d:%02d\n",
						systemTime.wMonth, systemTime.wDay, systemTime.wYear,
						systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
					printf("����i� � ������: %d\n", FileData.nFileSizeLow);
				} while (FindNextFileA(find, &FileData) != 0);
				FindClose(find);
			}
		}
		else if (strcmp(command, "copy") == 0) {
			
			char file_path_1[MAX_PATH];
			char file_path_2[MAX_PATH];
			printf("���� �� �����, ���� �����i��� ����i�����: ");
			scanf_s("%s", file_path_1, MAX_PATH);
			printf("���� � �i��� ���i������: ");
			scanf_s("%s", file_path_2, MAX_PATH);
			if (CopyFileA((LPCSTR)file_path_1, (LPCSTR)file_path_2, FALSE)) {
				printf("���� ����i������!\n");
			}
			else {
				printf("���� �� �i���� ��� �� i���!\n");
			}
		}
		else if (strcmp(command, "newdir") == 0) {

			char dir_name[MAX_PATH];
			printf("����i�� i�'� ��������i�: ");
			scanf_s("%s", dir_name, MAX_PATH);
			if (CreateDirectoryA((LPCSTR)dir_name, NULL)) {
				printf("��������i� ��������!\n");
			}
			else {
				printf("���� �� ���!\n");
			}
		}
		else if (strcmp(command, "remdir") == 0) {
			
			char dir_path[MAX_PATH];
			printf("����i�� ���� �� ��������i�: ");
			scanf_s("%s", dir_path, MAX_PATH);
			if (RemoveDirectoryA(dir_path)) {
				printf("��������i� ��������!\n");
			}
			else {
				printf("���� �� ���!\n");
			}
		}
		else if (strcmp(command, "remfile") == 0) {
			
			char file_path[MAX_PATH];
			printf("����i�� ���� �� �����: ");
			scanf_s("%s", file_path, MAX_PATH);
			if (DeleteFileA(file_path)) {
				printf("���� ��������!\n");
			}
			else {
				printf("���� �� ���!\n");
			}
		}
		else if (strcmp(command, "info") == 0) {

			char file_path[MAX_PATH];
			BY_HANDLE_FILE_INFORMATION lpFileInformation;
			printf("����i�� ���� �� �����: ");
			scanf_s("%s", file_path, MAX_PATH);
			HANDLE file = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (GetFileInformationByHandle(file, &lpFileInformation)) {
				wprintf(L"������� �����: %d\n", lpFileInformation.dwFileAttributes);
				SYSTEMTIME systemTime;
				FileTimeToSystemTime(&(lpFileInformation.ftCreationTime), &systemTime);
				wprintf(L"��� ���������: %02d-%02d-%d %02d:%02d:%02d\n",
					systemTime.wMonth, systemTime.wDay, systemTime.wYear,
					systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				FileTimeToSystemTime(&(lpFileInformation.ftLastAccessTime), &systemTime);
				wprintf(L"��� ���������� �������: %02d-%02d-%d %02d:%02d:%02d\n",
					systemTime.wMonth, systemTime.wDay, systemTime.wYear,
					systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				FileTimeToSystemTime(&(lpFileInformation.ftLastWriteTime), &systemTime);
				wprintf(L"��� ���������� ������: %02d-%02d-%d %02d:%02d:%02d\n",
					systemTime.wMonth, systemTime.wDay, systemTime.wYear,
					systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
				wprintf(L"����� ��������: %d\n", lpFileInformation.nNumberOfLinks);
				wprintf(L"���i���� ����� ����: %d\n", lpFileInformation.dwVolumeSerialNumber);
			}
			else {
				printf("���� �� ���!\n");
			}
			CloseHandle(file);

		}
		else {
			printf("\n�i� �i������!");
		}
	}
	exit(0);
}