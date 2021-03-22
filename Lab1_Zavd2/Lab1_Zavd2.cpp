#include <stdio.h>
#include "windows.h"
#include <locale.h>
#include <iostream>

using namespace std;


void Error()
{
    LPVOID lpMsgBuf;
    DWORD dwLastError = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMsgBuf, 0, NULL);
    printf("\n%s\n", lpMsgBuf);
    LocalFree(lpMsgBuf);
    exit(EXIT_FAILURE);
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ukr");
    if (argc != 3)
    {
        printf("Помилка! Аргумент не вказано, або вказано не коректно!\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (strcmp(argv[1], "-a") == 0)
        {
            HANDLE file1, file2;
            CHAR Buff[1024];
            WCHAR* uniBuff;
            DWORD read, write;
            BOOL resultRdFile = true;
            char namefile2[FILENAME_MAX] = "To_Unicode.txt";
            printf("Файл записано: To_Unicode.txt\n %s\n", namefile2);
            file1 = CreateFile(argv[2], GENERIC_READ, FILE_SHARE_READ,
                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (file1 == INVALID_HANDLE_VALUE)
            {
                Error();
            }
            file2 = CreateFile(namefile2, GENERIC_WRITE, 0,
                NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (file2 == INVALID_HANDLE_VALUE)
            {
                Error();
            }
            while (((resultRdFile = ReadFile(file1, &Buff, sizeof(Buff), &read, NULL)) != false) && (read != 0))
            {
                int sizeToAllocate = MultiByteToWideChar(CP_UTF8, 0, Buff, read, NULL, 0);
                uniBuff = (WCHAR*)calloc(sizeToAllocate, sizeof(WCHAR));
                MultiByteToWideChar(CP_UTF8, 0, Buff, read, uniBuff, sizeToAllocate);
                if ((!WriteFile(file2, uniBuff, sizeToAllocate * sizeof(WCHAR), &write, NULL)) ||
                    (write != sizeToAllocate * sizeof(WCHAR)))
                {
                    Error();
                }
                free(uniBuff);
            }
            if (resultRdFile == false && read != 0)
            {
                Error();
            }
            CloseHandle(file1);
            CloseHandle(file2);
        }
        else if (strcmp(argv[1], "-u") == 0)
        {
            HANDLE file1, file2;
            CHAR BytesBuff[1024];
            WCHAR* uniBuff;
            CHAR* asciBuff;
            DWORD read, write;
            BOOL resultRdFile = true;
            CHAR namefile2[FILENAME_MAX] = "To_ANSI.txt";
            printf("Файл записано: To_ANSI.txt\n %s\n", namefile2);
            file1 = CreateFile(argv[2], GENERIC_READ, FILE_SHARE_READ,
                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (file1 == INVALID_HANDLE_VALUE)
            {
               Error();
            }
            file2 = CreateFile(namefile2, GENERIC_WRITE, 0,
                NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (file2 == INVALID_HANDLE_VALUE)
            {
               Error();
            }
            if (!ReadFile(file1, &BytesBuff, 2, &read, NULL))
            {
                Error();
            }
            while (((resultRdFile = ReadFile(file1, &BytesBuff, sizeof(BytesBuff), &read, NULL)) != false) && (read != 0))
            {
                int unicodeLenght = MultiByteToWideChar(CP_UTF8, 0, BytesBuff, read, NULL, 0);
                uniBuff = (WCHAR*)calloc(unicodeLenght, sizeof(WCHAR));
                MultiByteToWideChar(CP_UTF8, 0, BytesBuff, read,
                    uniBuff, unicodeLenght);
                int asciLenght = WideCharToMultiByte(CP_ACP, 0, uniBuff, unicodeLenght, NULL, 0, NULL, NULL);
                asciBuff = (CHAR*)calloc(asciLenght, sizeof(CHAR));
                WideCharToMultiByte(CP_ACP, 0, uniBuff, unicodeLenght, asciBuff,
                    asciLenght, NULL, NULL);
                if ((!WriteFile(file2, asciBuff, asciLenght, &write, NULL)) || (write != asciLenght))
                {
                    Error();
                }
                free(uniBuff);
                free(asciBuff);
            }
            if (resultRdFile == false && read != 0)
            {
                Error();
            }
            CloseHandle(file1);
            CloseHandle(file2);
        }
        else
        {
            printf("Помилка! Аргумент не вказано, або вказано не коректно!\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}


