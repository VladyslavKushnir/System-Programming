#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>

#define _CRT_SECURE_NO_WARNINGS 
#pragma warning(disable : 4996)

using namespace std;

typedef struct KEY_DATA {
    TCHAR    achKey[255]; //ім'я підключа
    DWORD    cbName;//розмір імені 
    TCHAR    achClass[MAX_PATH] = TEXT("");//назва класу
    DWORD    cchClassName = MAX_PATH;//розмір назви
    DWORD    cSubKeys = 0;//кількість підключів 
    DWORD    cbMaxSubKey;// адреса буфера для найбільшого розміру імені підключа
    DWORD    cchMaxClass;// адреса буфера для найбільшого розміру імені класу 
    DWORD    cValues;//адреса буфера для кількості вхідних значень 
    DWORD    cchMaxValue;// адреса буфера для найбільшого розміру імен значень 
    DWORD    cbMaxValueData;//адреса буфера для найбільшого розміру даних значення 
    DWORD    cbSecurityDescriptor;// адреса буфера для довгих дескрипторів безпеки
    FILETIME ftLastWriteTime;// адреса буфера для отримання часу останніх записів 
} KEY_DATA, * PKEY_DATA;

bool Key(HKEY** hKey, LPSTR keyPath, REGSAM samDesired);
void Subkeys(HKEY hKey);
bool GetKeyData(HKEY key, KEY_DATA* keyData);
void Parameters(HKEY hKey);
bool FindString(HKEY hKey, string search_s, LPSTR keyPath);
bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
void SaveToFile(HKEY hKey);

int main()
{
    setlocale(LC_CTYPE, "ukr");
    HKEY hKey;
    PHKEY phKey = &hKey;
    CHAR keyPath[MAX_PATH];

    int mode;
    bool isExit = false;

    do
    {
        cout << "\n\tМеню" << endl
            << "1 - Вивести перелiк пiдключей." << endl
            << "2 - Вивести перелiк парметрiв з їх значеннями та типами." << endl
            << "3 - Шукати у реєстрi." << endl
            << "4 - Вивантажити ключ у файл." << endl
            << "0 - Вихiд. \n" << endl;

        cout << "Ваша дiя >> ";
        cin >> mode;

        switch (mode)
        {
        case 1:
        {
            if (Key(&phKey, keyPath, KEY_READ)) {
                Subkeys(hKey);
            }
            else
            {
                cout << "Ключ вiдсутнiй!" << endl;
            }

            break;
        }
        case 2:
        {
            if (Key(&phKey, keyPath, KEY_READ)) {
                Parameters(hKey);
            }
            else
            {
                cout << "Ключ вiдсутнiй!" << endl;
            }

            break;
        }
        case 3:
        {
            if (Key(&phKey, keyPath, KEY_READ))
            {
                string req_str = "";
                cout << "Введiть строку: ";
                getline(cin, req_str);

                FindString(hKey, req_str, keyPath);
            }
            else
            {
                cout << "Ключ вiдсутнiй!" << endl;
            }

            break;
        }
        case 4:
        {
            HANDLE processToken;

            //get access token 
            if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &processToken))
            {
                cout << "Cant get access rights " << endl;
            }
            else {
                // set privilege
                if (SetPrivilege(processToken, SE_BACKUP_NAME, TRUE))
                {
                    if (Key(&phKey, keyPath, KEY_READ)) {
                        SaveToFile(hKey);
                    }
                    else
                    {
                        cout << "Key does not exist." << endl;
                    }
                }
            }

            break;
        }
        case 0:
        {
            isExit = true;
            break;
        }
        default:
        {
            cout << "Дiя вiдсутня." << endl;
            break;
        }
        }

    }     while (!isExit);
}

//обрати корінь
bool Key(HKEY** hKey, LPSTR keyPath, REGSAM samDesired)
{
    HKEY rootKey;
    enum root_keys
    {
        CLASSES_ROOT,
        CURRENT_USER,
        LOCAL_MACHINE,
        USERS,
        CURRENT_CONFIG,
        PERFORMANCE_DATA
    };
    int mode;
    memset(keyPath, '\0', sizeof(keyPath));
    cout << "\nВиберiть корiнь пошуку:" << endl
        << "0 - HKEY_CLASSES_ROOT" << endl
        << "1 - HKEY_CURRENT_USER" << endl
        << "2 - HKEY_LOCAL_MACHINE" << endl
        << "3 - HKEY_USERS" << endl
        << "4 - HKEY_CURRENT_CONFIG" << endl
        << "5 - HKEY_PERFORMANCE_DATA" << endl
        << "Ваш вибiр: ";
       cin >> mode;

    switch (mode)
    {
    case CLASSES_ROOT:
    {
        rootKey = HKEY_CLASSES_ROOT;
        strcpy(keyPath, "HKEY_CLASSES_ROOT\\");
        break;
    }
    case CURRENT_USER:
    {
        rootKey = HKEY_CURRENT_USER;
        strcpy(keyPath, "HKEY_CURRENT_USER\\");
        break;
    }
    case LOCAL_MACHINE:
    {
        rootKey = HKEY_LOCAL_MACHINE;
        strcpy(keyPath, "HKEY_LOCAL_MACHINE\\");
        break;
    }
    case USERS:
    {
        rootKey = HKEY_USERS;
        strcpy(keyPath, "HKEY_USERS\\");
        break;
    }
    case CURRENT_CONFIG:
    {
        rootKey = HKEY_CURRENT_CONFIG;
        strcpy(keyPath, "HKEY_CURRENT_CONFIG\\");
        break;
    }
    case PERFORMANCE_DATA:
    {
        rootKey = HKEY_PERFORMANCE_DATA;
        strcpy(keyPath, "HKEY_PERFORMANCE_DATA\\");
        break;
    }
    default:
    {
        return false;
    }
    }
    string destKey = "";
    cout << "Для показу списку пiдключей кореня натиснiть Ентер. \nАбо вкажiть необхiдний шлях: ";
    getline(cin, destKey);
    getline(cin, destKey);
    LPSTR destination_key = const_cast<char*>(destKey.c_str());
    if (RegOpenKeyExA(rootKey, destination_key, 0, samDesired, *hKey) == ERROR_SUCCESS)
    {
        strcat(keyPath, destination_key);
        return true;
    }
    return false;
}

void Subkeys(HKEY hKey)
{
    KEY_DATA keyData = {};
    GetKeyData(hKey, &keyData);

    if (keyData.cSubKeys)
    {
        for (size_t i = 0; i < keyData.cSubKeys; i++)
        {
            keyData.cbName = 255;
            DWORD retCode = RegEnumKeyEx(
                hKey,
                i,
                keyData.achKey,
                &keyData.cbName,
                NULL,
                NULL,
                NULL,
                NULL
            );
            if (retCode == ERROR_SUCCESS)
            {
                _tprintf(TEXT("(%d) %s\n"), i + 1, keyData.achKey);
            }
        }
    }
    else
    {
        cout << "Пiдключi вiдсутнi." << endl;
    }

    RegCloseKey(hKey);
}

bool GetKeyData(HKEY key, KEY_DATA* keyData)
{
    DWORD retCode = RegQueryInfoKey(
        key,
        (*keyData).achClass,
        &(*keyData).cchClassName,
        NULL,
        &(*keyData).cSubKeys,
        &(*keyData).cbMaxSubKey,
        &(*keyData).cchMaxClass,
        &(*keyData).cValues,
        &(*keyData).cchMaxValue,
        &(*keyData).cbMaxValueData,
        &(*keyData).cbSecurityDescriptor,
        &(*keyData).ftLastWriteTime
    );

    return retCode == ERROR_SUCCESS;
}



void Parameters(HKEY hKey)
{
    DWORD retCode = ERROR_SUCCESS;
    DWORD i = 0;
    CHAR achValue[16383];
    DWORD cchValue = 16383;
    DWORD dwType = 0;
    LPBYTE lpData = NULL;
    DWORD dwData = 0;

    KEY_DATA keyData = {};
    GetKeyData(hKey, &keyData);

    if (keyData.cValues)
    {
          for (i = 0, retCode = ERROR_SUCCESS; i < keyData.cValues; i++)
        {
            cchValue = 16383;
            achValue[0] = '\0';
            retCode = RegEnumValueA(hKey, i, achValue, &cchValue, NULL, NULL, NULL, &dwData);
            lpData = (LPBYTE)malloc((dwData + 1) * sizeof(BYTE));
            cchValue = keyData.cchMaxValue + 1;

            retCode = RegEnumValueA(
                hKey,
                i,
                achValue,
                &cchValue,
                NULL,
                &dwType,
                lpData,
                &dwData
            );

            if (retCode == ERROR_SUCCESS)
            {
                cout << i + 1 << ". " << achValue << endl;

                switch (dwType)
                {
                case REG_BINARY:
                {
                    printf("Тип значення: REG_BINARY\nДанi: binary\n");
                    break;
                }
                case REG_DWORD:
                {
                    printf("Тип значення: REG_DWORD\nДанi: %#x|%u\n", *(DWORD*)(lpData), *(DWORD*)(lpData));
                    break;
                }
                case REG_EXPAND_SZ:
                {
                    printf("Тип значення: REG_EXPAND_SZ\nДанi: %s\n", lpData);
                    break;
                }
                case REG_LINK:
                {
                    LPCWCHAR data = reinterpret_cast<wchar_t*>(lpData);
                    wprintf(L"Тип значення: REG_LINK\nДанi: %ws\n", data);
                    break;
                }
                case REG_SZ:
                {
                    printf("Тип значення: REG_SZ\nДанi: %s\n", lpData);
                    break;
                }
                case REG_NONE:
                {
                    printf("Тип значення: REG_NONE\nДанi: %x\n", *(DWORD*)(lpData));
                    break;
                }
                default:
                {
                    printf("Тип значення: undefined\nДанi: %x\n", *(DWORD*)(lpData));
                    break;
                }
                }
            }
            free(lpData);
        }
    }
    else
    {
        cout << "Параметри не знайдено." << endl;
    }

    RegCloseKey(hKey);
}

bool FindString(HKEY hKey, string searchStr, LPSTR keyPath)
{
    LPSTR request_string = const_cast<char*>(searchStr.c_str());
    KEY_DATA keyData = {};
    DWORD retCode = ERROR_SUCCESS;
    LPSTR subkeyPath;
    if (!GetKeyData(hKey, &keyData))
    {
        return false;
    }
    if (keyData.cSubKeys)
    {
        for (size_t i = 0; i < keyData.cSubKeys; i++)
        {
            keyData.cbName = 255;

            retCode = RegEnumKeyEx(
                hKey,
                i,
                keyData.achKey,
                &keyData.cbName,
                NULL,
                NULL,
                NULL,
                NULL
            );
            if (retCode == ERROR_SUCCESS)
            {
                wstring wideStr = wstring(searchStr.begin(), searchStr.end());
                if (_tcscmp(keyData.achKey, wideStr.c_str()) == 0)
                {
                    cout << "Знайдено в назвi: " << request_string << endl;
                }

                subkeyPath = (LPSTR)malloc(16383 * sizeof(TCHAR));

                wstring s = keyData.achKey;
                string b = string(s.begin(), s.end());

                strcpy(subkeyPath, const_cast<char*>(b.c_str()));
                strcat(subkeyPath, "\\");
                strcat(subkeyPath, request_string);

                HKEY subKey = {};

                if (RegOpenKeyEx(hKey, keyData.achKey, 0, KEY_READ, &subKey) == ERROR_SUCCESS)
                {
                    FindString(subKey, request_string, subkeyPath);
                }
            }
        }
    }
    if (keyData.cValues)
    {
        CHAR achValue[16383];
        DWORD cchValue = 16383;
        DWORD dwType = 0;
        LPBYTE lpData = NULL;
        DWORD dwData = 0;
        for (int i = 0; i < keyData.cValues; i++)
        {
            cchValue = 16383;
            achValue[0] = '\0';
            retCode = RegEnumValueA(hKey, i, achValue, &cchValue, NULL, NULL, NULL, &dwData);
            lpData = (LPBYTE)malloc((dwData + 1) * sizeof(BYTE));
            cchValue = keyData.cchMaxValue + 1;
            retCode = RegEnumValueA(
                hKey,
                i,
                achValue,
                &cchValue,
                NULL,
                &dwType,
                lpData,
                &dwData
            );
            if (retCode == ERROR_SUCCESS)
            {
                if (_strcmpi(achValue, request_string) == 0)
                {
                    cout << "Знайдено у назвi: " << keyPath << "; " << achValue << endl;
                }
                if (((dwType & REG_EXPAND_SZ) == REG_EXPAND_SZ) || ((dwType & REG_SZ) == REG_SZ))
                {
                    if (_strcmpi((LPSTR)lpData, request_string) == 0)
                    {
                        cout << "Значення: " << keyPath << "\n Iм'я: " << achValue << endl;
                            
                    }
                }
            }
        }
    }

    RegCloseKey(hKey);
}

// MSDN function
bool SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid
    ))                   // receives LUID of privilege
    {
        cout << "LookupPrivilegeValue error: " << GetLastError() << endl;
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;

    if (bEnablePrivilege)
    {
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }
    else
    {
        tp.Privileges[0].Attributes = 0;
    }

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL
    ))
    {
        cout << "AdjustTokenPrivileges error: " << GetLastError() << endl;
        return false;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        cout << "The token does not have the specified privilege." << endl;
        return false;
    }

    return true;
}

void SaveToFile(HKEY hKey) {

    string file_path = "";
    DWORD retCode = ERROR_SUCCESS;

    cout << "Шлях до файлу: ";
    getline(cin, file_path);

    retCode = RegSaveKeyA(hKey, file_path.c_str(), NULL);
    switch (retCode)
    {
    case ERROR_SUCCESS:
    {
        cout << "Збережено у файл: " << file_path << endl;
        break;
    }
    case ERROR_ALREADY_EXISTS:
    {
        cout << "Файл вже iснує!" << endl;
        break;
    }
    default:
    {
        cout << "Не вдається зберегти ключ у файл. " << endl;
        break;
    }
    }

    RegCloseKey(hKey);
}