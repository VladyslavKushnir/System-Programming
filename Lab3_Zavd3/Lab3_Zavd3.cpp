#include <Windows.h>
#include <ctime>
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

CRITICAL_SECTION critical_section;
vector<int> arr;

DWORD WINAPI Sum(LPVOID lpv_use_critical_section)
{
    bool use_critical_section = (bool)lpv_use_critical_section;

    if (use_critical_section)
    {
        if (!InitializeCriticalSectionAndSpinCount(&critical_section, 0))
        {
            return 0;
        }

        EnterCriticalSection(&critical_section);
        int sum = 0;
        for (int i = 0; i < 1000000; i++)
        {
            sum += arr[i];
        }
        printf("Сума елементiв: %d\n", sum);
        LeaveCriticalSection(&critical_section);
    }
    else
    {
        int sum = 0;
        for (int i = 0; i < 1000000; i++)
        {
            sum += arr[i];
        }
        printf("Сума елементiв: %d\n", sum);
    }

    return 0;
}

DWORD WINAPI Avg(LPVOID lpv_use_critical_section)
{
    bool use_critical_section = (bool)lpv_use_critical_section;

    if (use_critical_section)
    {
        if (!InitializeCriticalSectionAndSpinCount(&critical_section, 0))
        {
            return 0;
        }

        EnterCriticalSection(&critical_section);
        float avg = 0;
        for (int i = 0; i < 1000000; i++)
        {
            avg += arr[i];
        }

        printf("Середнє арифметичне: %f\n", (float)(avg / 1000000));
        LeaveCriticalSection(&critical_section);
    }
    else
    {
        float avg = 0;
        for (int i = 0; i < 1000000; i++)
        {
            avg += arr[i];
        }

        printf("Середнє арифметичне: %f\n", (float)(avg / 1000000));
    }

    return 0;
}

DWORD WINAPI Max(LPVOID lpv_use_critical_section)
{
    bool use_critical_section = (bool)lpv_use_critical_section;

    if (use_critical_section)
    {
        if (!InitializeCriticalSectionAndSpinCount(&critical_section, 0))
        {
            return 0;
        }
        EnterCriticalSection(&critical_section);
        int max = 0;
        for (size_t i = 0; i < 1000000; i++)
        {
            if (arr[i] > max)
            {
                max = arr[i];
            }
        }
        printf("Максимальний елемент: %d\n", max);
        LeaveCriticalSection(&critical_section);
    }
    else
    {
        int max = 0;
        for (size_t i = 0; i < 1000000; i++)
        {
            if (arr[i] > max)
            {
                max = arr[i];
            }
        }
        printf("Максимальний елемент: %d\n", max);
    }
    return 0;
}

void CreateThreads(bool use_critical_section)
{
    vector<HANDLE> v_h_thread; //Вектор дескрипторів потоку
    vector<DWORD> v_id_thread; //Вектор ідентифікаторів потоку
    clock_t t_start, t_end;
    // генерація масиву
    for (size_t i = 0; i < 1000000; i++)
    {
        arr.push_back(rand() % 1000000 + 1000);
    }
    // початок відліку
    t_start = clock();

    for (size_t i = 0; i < 3; i++)
    {
        v_h_thread.push_back(NULL);
        v_id_thread.push_back(NULL);
    }
    //виконання дій
    v_h_thread[0] = CreateThread(NULL, 0, Sum, (LPVOID)use_critical_section, 0, &v_id_thread[0]);
    v_h_thread[1] = CreateThread(NULL, 0, Avg, (LPVOID)use_critical_section, 0, &v_id_thread[1]);
    v_h_thread[2] = CreateThread(NULL, 0, Max, (LPVOID)use_critical_section, 0, &v_id_thread[2]);
    WaitForMultipleObjects(3, v_h_thread.data(), TRUE, INFINITE);
    // закінчення відліку
    t_end = clock();
    printf("Час виконання: %f секунд.\n", (float)(t_end - t_start) / CLOCKS_PER_SEC);

    DeleteCriticalSection(&critical_section);
    
    for (int i = 0; i < 3; i++) {
        CloseHandle(v_h_thread[i]);
    }
}

int main()
{
    setlocale(LC_CTYPE, "ukr");
    CHAR choice;
    while (1)
    {
        cout << "\n\tМеню" << endl;
        cout << " 1 - Без взаємовиключення" << endl;
        cout << " 2 - З взаємовиключенням" << endl;
        cout << " 0 - Вихiд" << endl;
        cout << "\nВиконати дiю >> ";
        cin >> choice;
        cout << "\n";
        //Вибір дії
        switch (choice)
        {
        case '1': // Без взаємовиключення
        {
            CreateThreads(false);
            continue;
          
        }
        break;
        case '2': // З взаємовиключенням
        {
            CreateThreads(true); 
            continue;
        } break;

        case '0': // Вихід
        {
            return 0;
        } break;

        default: {
            cout << "Дiя вiдсутня!" << endl;
            continue; }
        }
    }

    return 0;
}