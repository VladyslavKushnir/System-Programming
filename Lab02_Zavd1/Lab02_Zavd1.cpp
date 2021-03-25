#include "Header.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ukr"); //ініціалізація української
	CHAR choice; //змінна, в якій зберігаєть номер дії
	DWORD сount = 0, //загальна кількість записів 
		dwFileSize = 0;
	HANDLE H_myFile = NULL;
	Initialize(&H_myFile, &dwFileSize, &сount);
	//нескінченний цикл
	while (1)
	{

		cout << "\tМеню" << endl;
		cout << " 1 - Створити запис" << endl;
		cout << " 2 - Змiнити запис" << endl;
		cout << " 3 - Видалити запис" << endl;
		cout << " 4 - Вивести данi" << endl;
		cout << " 0 - Вихiд" << endl;
		cout << "\nВиконати дiю >> ";
		cin >> choice;
		//Вибір дії
		switch (choice)
		{
		case '1': // Створити запис
		{
			CHAR create[TEXT_FIELD_IN_USER_RECORD] = { 0 };
			cout << "Введiть текст нового запису або '0' для створення порожнього запису:\n";
			cin >> create;
			BOOL isNull = false;
			USER_RECORD userRec;
			if (strcmp(create, "0") == 0)
			{
				memset(userRec.Text, '\0', sizeof(userRec.Text));
				isNull = true;
			}
			else
			{
				strcpy_s(userRec.Text, TEXT_FIELD_IN_USER_RECORD, create);
				userRec.Text[TEXT_FIELD_IN_USER_RECORD] = '\0';
				isNull = false;
			}
			userRec.Change = 0;
			userRec.IdRec = *&сount;
			SYSTEMTIME sysTime = { 0 };
			FILETIME fileTime = { 0 };
			GetLocalTime(&sysTime);
			SystemTimeToFileTime(&sysTime, &fileTime);
			userRec.Time = fileTime;
			// онивити хедер (запис запису)
			WriteRecord(H_myFile, userRec, sizeof(FILE_FOR_RECORDS_HEADER) + (сount) * sizeof(USER_RECORD));
			FILE_FOR_RECORDS_HEADER header = { 0 };
			ReadHeader(H_myFile, &header, FALSE);
			header.FileSize += sizeof(USER_RECORD);
			header.NotNull = isNull ? header.NotNull : ++header.NotNull;
			CreateHeader(H_myFile, &header, FALSE);
			(сount)++;
			(dwFileSize) += sizeof(USER_RECORD);
			continue;
		}
		break;
		case '2': // Змінити запис
		{
			DWORD id;
			cout << "Введiть номер запису для внесення змiн: ";
			cin >> id;
			if (id >= сount)
			{
				cout << "Запис не знайдено!\n";
				continue;
			}
			else
			{
				Modify(H_myFile, id);
				continue;
			}

		} break;

		case '3': // Видалити запис
		{
			DWORD id;
			cout << "Введiть номер запису для видалення: ";
			cin >> id;
			if (id >= сount)
			{
				cout << "Запис не знайдено!\n";
				continue;
			}
			else
			{
				Delete(H_myFile, id, &сount, &dwFileSize);
				continue;
			}
		} break;

		case '4': // Вивести дані
		{
			FILE_FOR_RECORDS_HEADER header = { 0 };
			USER_RECORD recordsBuff = { 0 };
			DWORD cReadedBytes = 0;
			ReadHeader(H_myFile, &header, FALSE);
			cout << "\nРезультат:\n";
			cout << "Кiлькiсть не пустих запиiв: " << header.NotNull << endl;
			cout << "Розмiр файлу в байтах: " << header.FileSize << "\n" << endl;
			while ((ReadFile(H_myFile, &recordsBuff, sizeof(USER_RECORD), &cReadedBytes, NULL) != false) &&
				(cReadedBytes == sizeof(USER_RECORD)))
			{
				SYSTEMTIME sysTime;
				FileTimeToSystemTime(&recordsBuff.Time, &sysTime);
				cout << "Запис № " << recordsBuff.IdRec << endl;
				printf("Час створення: %d %d %d, %d %d\n", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute);
				cout << "Змiст запису: " << recordsBuff.Text << endl;
				cout << "Кiлькiсть змiн: " << recordsBuff.Change << "\n" << endl;

			}
		} break;

		case '0': // Вихід
		{
			CloseHandle(H_myFile);
			return 0;
		} break;

		default: {
			cout << "Дiя вiдсутня!" << endl;
			continue; }
		}
	}
}
// Створити хедер
bool CreateHeader(HANDLE hRecordsFile, PFILE_FOR_RECORDS_HEADER pHeader, BOOL clearFile) {
	DWORD bytes = 0;
	if (SetFilePointer(hRecordsFile, 0, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return false;
	}
	if ((WriteFile(hRecordsFile, pHeader, sizeof(FILE_FOR_RECORDS_HEADER), &bytes, NULL) == false) ||
		(bytes != sizeof(FILE_FOR_RECORDS_HEADER)))
	{
		return false;
	}
	if (clearFile)
	{
		if (SetEndOfFile(hRecordsFile) == false)
		{
			return false;
		}
	}

}
//Читати хедер
bool ReadHeader(HANDLE hRecordsFile, PFILE_FOR_RECORDS_HEADER pHeader, BOOL clearFile)
{
	DWORD bytes = 0;
	if (SetFilePointer(hRecordsFile, 0, 0, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return false;
	}
	if ((ReadFile(hRecordsFile, pHeader, sizeof(FILE_FOR_RECORDS_HEADER), &bytes, NULL) == false) ||
		(bytes != sizeof(FILE_FOR_RECORDS_HEADER)))
	{
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
// Змінити запис
bool Modify(HANDLE H_File, DWORD idToModify)
{
	FILE_FOR_RECORDS_HEADER header;
	CHAR buff[TEXT_FIELD_IN_USER_RECORD + 1] = { 0 };
	ReadHeader(H_File, &header, FALSE);
	cout << "Введiть новий текст для запису або '0', щоб скинути запис: \n";
	cin >> buff;
	buff[TEXT_FIELD_IN_USER_RECORD] = '\0';
	USER_RECORD recBuff = { 0 };
	DWORD cBytes = 0;
	BOOL isNullRecord = false;
	if (SetFilePointer(H_File, sizeof(FILE_FOR_RECORDS_HEADER) + sizeof(USER_RECORD) * idToModify,
		NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return false;
	}
	if ((ReadFile(H_File, &recBuff, sizeof(USER_RECORD), &cBytes, NULL) == false) ||
		(cBytes != sizeof(USER_RECORD)))
	{
		return false;
	}
	isNullRecord = recBuff.Text[0] == '\0' ? true : false;
	if (strcmp(buff, "0") == 0)
	{
		memset(recBuff.Text, '\0', sizeof(recBuff.Text));
	}
	else
	{
		strcpy_s(recBuff.Text, TEXT_FIELD_IN_USER_RECORD, buff);
		recBuff.Text[TEXT_FIELD_IN_USER_RECORD] = '\0';
	}
	recBuff.Change++;
	WriteRecord(H_File, recBuff, (sizeof(FILE_FOR_RECORDS_HEADER) + sizeof(USER_RECORD) * idToModify));
	if (isNullRecord && (strcmp(buff, "0") != 0)) header.NotNull++;
	if (!isNullRecord && (strcmp(buff, "0") == 0)) header.NotNull--;
	CreateHeader(H_File, &header, FALSE);
}
// Видалити запис
bool Delete(HANDLE H_File, DWORD idToDelete, DWORD* countRecords, DWORD* fileSize)
{
	FILE_FOR_RECORDS_HEADER header;
	ReadHeader(H_File, &header, FALSE);
	if (SetFilePointer(H_File, (sizeof(FILE_FOR_RECORDS_HEADER) + ((idToDelete) * sizeof(USER_RECORD))),
		NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return false;
	}
	USER_RECORD delRec;
	if (ReadFile(H_File, &delRec, sizeof(delRec), 0, NULL) == false)
	{
		return false;
	}
	WORD countElemToEndFromDeletedEl = (*countRecords) - idToDelete - 1;
	USER_RECORD* recordsBuff = (USER_RECORD*)calloc(countElemToEndFromDeletedEl, sizeof(USER_RECORD));
	DWORD cBytes;
	if ((ReadFile(H_File, recordsBuff, sizeof(USER_RECORD) * countElemToEndFromDeletedEl, &cBytes, NULL) == false) ||
		cBytes != sizeof(USER_RECORD) * countElemToEndFromDeletedEl)
	{
		return false;
	}
	for (WORD i = 0; i < countElemToEndFromDeletedEl; i++)
	{
		recordsBuff[i].IdRec = i + idToDelete;
	}
	if (SetFilePointer(H_File, 0 - (sizeof(USER_RECORD) * countElemToEndFromDeletedEl + sizeof(USER_RECORD)),
		NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
	{
		return false;
	}
	if ((WriteFile(H_File, recordsBuff, sizeof(USER_RECORD) * countElemToEndFromDeletedEl, &cBytes, NULL) == false) ||
		(cBytes != sizeof(USER_RECORD) * countElemToEndFromDeletedEl))
	{
		return false;
	}
	free(recordsBuff);
	SetEndOfFile(H_File);
	header.NotNull = delRec.Text[0] == '\0' ?
		header.NotNull : header.NotNull - 1;
	header.FileSize = header.FileSize - sizeof(USER_RECORD);
	(*countRecords)--;
	(*fileSize) -= sizeof(USER_RECORD);
	CreateHeader(H_File, &header, FALSE);
	return true;
}

bool WriteRecord(HANDLE hRecordsFile, USER_RECORD usRec, DWORD offset)
{
	if (SetFilePointer(hRecordsFile, offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return false;
	}
	DWORD countWritenBytes = 0;
	if ((WriteFile(hRecordsFile, &usRec, sizeof(usRec), &countWritenBytes, NULL) == false) ||
		(countWritenBytes != sizeof(usRec)))
	{
		return false;
	}
	return true;
}


int GetCount(DWORD fileSize)
{
	WORD recordsCount = (fileSize - sizeof(FILE_FOR_RECORDS_HEADER)) / sizeof(USER_RECORD);
	return recordsCount;
}

bool Initialize(HANDLE* H_File, DWORD* fileSize, DWORD* countRecords)
{
	WCHAR PATH[] = L"myFile.txt";
	*H_File = CreateFile(PATH,
		GENERIC_ALL,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (*H_File == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	*fileSize = GetFileSize(*H_File, NULL);
	if (*fileSize == INVALID_FILE_SIZE)
	{
		return false;
	}
	if (*fileSize <= sizeof(FILE_FOR_RECORDS_HEADER))
	{
		FILE_FOR_RECORDS_HEADER header;
		header.NotNull = 0;
		header.FileSize = sizeof(FILE_FOR_RECORDS_HEADER);
		CreateHeader(H_File, &header, TRUE);
		*fileSize = sizeof(FILE_FOR_RECORDS_HEADER);
	}
	*countRecords = GetCount(*fileSize);
}
