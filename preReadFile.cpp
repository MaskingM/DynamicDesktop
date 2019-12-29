#include "preReadFile.h"

PBYTE preCVReadFile(const char* szFileName){
	HANDLE hFile = NULL;
	DWORD dwFileSize = 0, dwHighSize = 0, dwBytesRead = 0;
	BOOL bSuccess = FALSE;
	PBYTE pBits = NULL;
	do{
		hFile = CreateFile(
			szFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL
		);
		if(INVALID_HANDLE_VALUE == hFile){
			break;
		}
		dwFileSize = GetFileSize(hFile, &dwHighSize);
		if(dwHighSize){
			CloseHandle(hFile);
			break;
		}
		pBits = (PBYTE)malloc(_PREREADSIZE);
		if(!pBits){
			CloseHandle(hFile);
			break;
		}
		bSuccess = ReadFile(hFile, pBits, _PREREADSIZE, &dwBytesRead, NULL);
		CloseHandle(hFile);
		if(!bSuccess || (_PREREADSIZE != dwBytesRead)){
			free(pBits);
			pBits = NULL;
			break;
		}
	}while(FALSE);
	return pBits;
}