#include "Header.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ukr"); //����������� ���������
	CHAR choice; //�����, � ��� �������� ����� 䳿
	DWORD �ount = 0, //�������� ������� ������ 
		dwFileSize = 0;
	HANDLE H_myFile = NULL;
	Initialize(&H_myFile, &dwFileSize, &�ount);
	//����������� ����
	while (1)
	{

		cout << "\t����" << endl;
		cout << " 1 - �������� �����" << endl;
		cout << " 2 - ��i���� �����" << endl;
		cout << " 3 - �������� �����" << endl;
		cout << " 4 - ������� ���i" << endl;
		cout << " 0 - ���i�" << endl;
		cout << "\n�������� �i� >> ";
		cin >> choice;
		//���� 䳿
		switch (choice)
		{
		case '1': // �������� �����
		{
			CHAR create[TEXT_FIELD_IN_USER_RECORD] = { 0 };
			cout << "����i�� ����� ������ ������ ��� '0' ��� ��������� ���������� ������:\n";
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
			userRec.IdRec = *&�ount;
			SYSTEMTIME sysTime = { 0 };
			FILETIME fileTime = { 0 };
			GetLocalTime(&sysTime);
			SystemTimeToFileTime(&sysTime, &fileTime);
			userRec.Time = fileTime;
			// ������� ����� (����� ������)
			WriteRecord(H_myFile, userRec, sizeof(FILE_FOR_RECORDS_HEADER) + (�ount) * sizeof(USER_RECORD));
			FILE_FOR_RECORDS_HEADER header = { 0 };
			ReadHeader(H_myFile, &header, FALSE);
			header.FileSize += sizeof(USER_RECORD);
			header.NotNull = isNull ? header.NotNull : ++header.NotNull;
			CreateHeader(H_myFile, &header, FALSE);
			(�ount)++;
			(dwFileSize) += sizeof(USER_RECORD);
			continue;
		}
		break;
		case '2': // ������ �����
		{
			DWORD id;
			cout << "����i�� ����� ������ ��� �������� ��i�: ";
			cin >> id;
			if (id >= �ount)
			{
				cout << "����� �� ��������!\n";
				continue;
			}
			else
			{
				Modify(H_myFile, id);
				continue;
			}

		} break;

		case '3': // �������� �����
		{
			DWORD id;
			cout << "����i�� ����� ������ ��� ���������: ";
			cin >> id;
			if (id >= �ount)
			{
				cout << "����� �� ��������!\n";
				continue;
			}
			else
			{
				Delete(H_myFile, id, &�ount, &dwFileSize);
				continue;
			}
		} break;

		case '4': // ������� ���
		{
			FILE_FOR_RECORDS_HEADER header = { 0 };
			USER_RECORD recordsBuff = { 0 };
			DWORD cReadedBytes = 0;
			ReadHeader(H_myFile, &header, FALSE);
			cout << "\n���������:\n";
			cout << "�i���i��� �� ������ ����i�: " << header.NotNull << endl;
			cout << "����i� ����� � ������: " << header.FileSize << "\n" << endl;
			while ((ReadFile(H_myFile, &recordsBuff, sizeof(USER_RECORD), &cReadedBytes, NULL) != false) &&
				(cReadedBytes == sizeof(USER_RECORD)))
			{
				SYSTEMTIME sysTime;
				FileTimeToSystemTime(&recordsBuff.Time, &sysTime);
				cout << "����� � " << recordsBuff.IdRec << endl;
				printf("��� ���������: %d %d %d, %d %d\n", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute);
				cout << "��i�� ������: " << recordsBuff.Text << endl;
				cout << "�i���i��� ��i�: " << recordsBuff.Change << "\n" << endl;

			}
		} break;

		case '0': // �����
		{
			CloseHandle(H_myFile);
			return 0;
		} break;

		default: {
			cout << "�i� �i������!" << endl;
			continue; }
		}
	}
}
// �������� �����
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
//������ �����
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
// ������ �����
bool Modify(HANDLE H_File, DWORD idToModify)
{
	FILE_FOR_RECORDS_HEADER header;
	CHAR buff[TEXT_FIELD_IN_USER_RECORD + 1] = { 0 };
	ReadHeader(H_File, &header, FALSE);
	cout << "����i�� ����� ����� ��� ������ ��� '0', ��� ������� �����: \n";
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
// �������� �����
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
