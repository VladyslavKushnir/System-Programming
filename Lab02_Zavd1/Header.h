#include <iostream>
#include "windows.h"
#include <vector>
#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS
#define TEXT_FIELD_IN_USER_RECORD 79

//Структура хедера
typedef struct
{
	WORD NotNull; //кількість не нульових
	WORD FileSize; //розмрі файлу
} FILE_FOR_RECORDS_HEADER, * PFILE_FOR_RECORDS_HEADER;

//Структура запису
typedef struct
{
	WORD IdRec; // номер
	FILETIME Time; // час створення
	CHAR Text[TEXT_FIELD_IN_USER_RECORD + 1]; // зміст
	WORD Change; // число змін
} USER_RECORD, * PUSER_RECORD;



//Ініціалізація
bool Initialize(HANDLE* hRecordsFile, DWORD* fileSize, DWORD* countRecords);
//Створити та зчитати хедер
bool CreateHeader(HANDLE hRecordsFile, PFILE_FOR_RECORDS_HEADER pHeader, BOOL clearFile);
bool ReadHeader(HANDLE hRecordsFile, PFILE_FOR_RECORDS_HEADER pHeader, BOOL clearFile);
//Запис 
bool WriteRecord(HANDLE hRecordsFile, USER_RECORD usRec, DWORD offset);
//Видалили та модифікувати
bool Delete(HANDLE hRecordsFile, DWORD idToDelete, DWORD* countRecords, DWORD* fileSize);
bool Modify(HANDLE hRecordsFile, DWORD idToModify);
//Отримати кількість записів
int GetCount(DWORD fileSize);
