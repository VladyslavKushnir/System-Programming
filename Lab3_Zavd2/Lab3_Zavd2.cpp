#include <iostream>
#include <ctime>
#include <vector>
#include <windows.h>
#define _CRT_SECURE_NO_WARNINGS 
#pragma warning(disable : 4996)

using namespace std;

HANDLE semaphore;

DWORD WINAPI thread_routine(LPVOID lpParam)
{
    DWORD bytes_written;
    HANDLE file;
    clock_t t_start, t_end;
    LPSTR buf = new char[20];
    bool running = true;
    t_start = clock();
    DWORD dw_wait_result = WaitForSingleObject(semaphore, INFINITE);
    while (running)
    {
        if (dw_wait_result == WAIT_OBJECT_0)
        {
            printf("Очiкування %d \n", GetCurrentThreadId());
            Sleep(1000 * (rand() % 3 + 1));
            printf("Запис %d \n", GetCurrentThreadId());
            file = CreateFileA(
                "lab05_file.txt",
                FILE_GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            t_end = clock();
            SetFilePointer(file, 0, NULL, FILE_END);
            sprintf(buf, " %f с ", (float)(t_end - t_start) / CLOCKS_PER_SEC);
            WriteFile(file, buf, sizeof(char) * strlen(buf), &bytes_written, NULL);
            if (file == INVALID_HANDLE_VALUE)
            {
                printf("Помилка! %d\n", GetLastError());
            }
            running = false;
            Sleep(1000 * (rand() % 3 + 1));
            CloseHandle(file);
            printf("Закiнчив работу: %d\n", GetCurrentThreadId());

            if (!ReleaseSemaphore(semaphore, 1, NULL)) {
                printf("Семфор не створено! %d\n", GetLastError());
            }
        }
    }
    return 0;
}

int main()
{
    setlocale(LC_CTYPE, "ukr");
    int count;
    cout << "Кiлькiсть потокiв: ";
    cin >> count;
    HANDLE* h_threads = new HANDLE[count];
    semaphore = CreateSemaphore(NULL,2,2,TEXT("Lab05_Zavd2"));
    if (semaphore == NULL)
    {
        printf("Помилка Mutex%d\n", GetLastError());
        return 1;
    }
    for (size_t i = 0; i < count; i++)
    {
        HANDLE thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thread_routine,NULL,NULL,NULL);
        if (h_threads[i] == NULL)
        {
            fprintf(stderr, "%s\n", "Помилка Thread\n");
            ExitProcess(0);
        }
        h_threads[i] = thread;
    }
    WaitForMultipleObjects(count, h_threads, TRUE, INFINITE);
    for (size_t i = 0; i < count; i++)
    {
        CloseHandle(h_threads[i]);
    }
    CloseHandle(semaphore);
    return 0;
}