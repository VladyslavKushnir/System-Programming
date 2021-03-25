#include <iostream>
#include "windows.h"
#include <vector>
#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS
#define TEXT_FIELD_IN_USER_RECORD 79

//��������� ������
typedef struct
{
	WORD NotNull; //������� �� ��������
	WORD FileSize; //����� �����
} FILE_FOR_RECORDS_HEADER, * PFILE_FOR_RECORDS_HEADER;

//��������� ������
typedef struct
{
	WORD IdRec; // �����
	FILETIME Time; // ��� ���������
	CHAR Text[TEXT_FIELD_IN_USER_RECORD + 1]; // ����
	WORD Change; // ����� ���
} USER_RECORD, * PUSER_RECORD;



//�����������
bool Initialize(HANDLE* hRecordsFile, DWORD* fileSize, DWORD* countRecords);
//�������� �� ������� �����
bool CreateHeader(HANDLE hRecordsFile, PFILE_FOR_RECORDS_HEADER pHeader, BOOL clearFile);
bool ReadHeader(HANDLE hRecordsFile, PFILE_FOR_RECORDS_HEADER pHeader, BOOL clearFile);
//����� 
bool WriteRecord(HANDLE hRecordsFile, USER_RECORD usRec, DWORD offset);
//�������� �� ������������
bool Delete(HANDLE hRecordsFile, DWORD idToDelete, DWORD* countRecords, DWORD* fileSize);
bool Modify(HANDLE hRecordsFile, DWORD idToModify);
//�������� ������� ������
int GetCount(DWORD fileSize);
