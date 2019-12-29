#include "fileDlg.h"


void initOpenFileName(HWND hwnd, OPENFILENAME* ofn, TCHAR* szFileName, TCHAR* szTitleName, TCHAR* szFilter){
	ofn->lStructSize = sizeof(OPENFILENAME);
	ofn->hwndOwner = hwnd;
	ofn->hInstance = NULL;
	ofn->lpstrFilter = szFilter;
	ofn->lpstrCustomFilter = NULL;
	ofn->nMaxCustFilter = 0;
	ofn->nFilterIndex = 0;
	ofn->lpstrFile = szFileName;
	ofn->nMaxFile = MAX_PATH;
	ofn->lpstrFileTitle = szTitleName;
	ofn->nMaxFileTitle = MAX_PATH;
	ofn->lpstrInitialDir = NULL;
	ofn->lpstrTitle = NULL;
	ofn->Flags = 0;
	ofn->nFileOffset = 0;
	ofn->nFileExtension = 0;
	ofn->lpstrDefExt = TEXT("bmp");
	ofn->lCustData = 0;
	ofn->lpfnHook = NULL;
	ofn->lpTemplateName = NULL;
}

BOOL openFileDlg(OPENFILENAME* ofn){
	ofn->Flags = 0;
	return GetOpenFileName(ofn);
}

BOOL saveFileDlg(OPENFILENAME* ofn){
	ofn->Flags = OFN_OVERWRITEPROMPT;
	return GetSaveFileName(ofn);
}

PBYTE preReadFile(HINSTANCE hInstance, TCHAR* szFileName){
	HANDLE hFile = NULL;
	DWORD dwFileSize = 0, dwHighSize = 0, dwBytesRead = 0;
	BOOL bSuccess = FALSE;
	PBYTE pBits = NULL;
	TCHAR title[64] = {0}, buf[64] = {0};
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
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILEINVALID, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
		dwFileSize = GetFileSize(hFile, &dwHighSize);
		if(dwHighSize){
			CloseHandle(hFile);
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILETOOLARGE, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
		pBits = (PBYTE)malloc(PREREADSIZE);
		if(!pBits){
			CloseHandle(hFile);
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_MEMERY, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
		bSuccess = ReadFile(hFile, pBits, PREREADSIZE, &dwBytesRead, NULL);
		CloseHandle(hFile);
		if(!bSuccess || (PREREADSIZE != dwBytesRead)){
			free(pBits);
			pBits = NULL;
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILEREAD, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
	}while(FALSE);
	return pBits;
}

PBYTE readFile(HINSTANCE hInstance, TCHAR* szFileName){
	HANDLE hFile = NULL;
	DWORD dwFileSize = 0, dwHighSize = 0, dwBytesRead = 0;
	BOOL bSuccess = FALSE;
	PBYTE pBits = NULL;
	TCHAR title[64] = {0}, buf[64] = {0};
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
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILEINVALID, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
		dwFileSize = GetFileSize(hFile, &dwHighSize);
		if(dwHighSize){
			CloseHandle(hFile);
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILETOOLARGE, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
		pBits = (PBYTE)malloc(dwFileSize);
		if(!pBits){
			CloseHandle(hFile);
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_MEMERY, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
		bSuccess = ReadFile(hFile, pBits, dwFileSize, &dwBytesRead, NULL);
		CloseHandle(hFile);
		if(!bSuccess || (dwFileSize != dwBytesRead)){
			free(pBits);
			pBits = NULL;
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILEREAD, buf, 64);
			sendSysNotifyMsg(title, buf);
			break;
		}
	}while(FALSE);
	return pBits;
}

BOOL saveFile(HINSTANCE hInstance, TCHAR* szFileName, PBYTE pBits, DWORD size){
	HANDLE hFile = NULL;
	DWORD dwBytesWrited = 0;
	BOOL bSuccess = FALSE;
	BOOL bResult = TRUE;
	TCHAR title[64] = {0}, buf[64] = {0};
	
	do{
		hFile = CreateFile(
			szFileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if(INVALID_HANDLE_VALUE == hFile){
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILEINVALID, buf, 64);
			sendSysNotifyMsg(title, buf);
			bResult = FALSE;
			break;
		}
		
		bSuccess = WriteFile(hFile, pBits, size, &dwBytesWrited, NULL);
		CloseHandle(hFile);
		if(!bSuccess || (size != dwBytesWrited)){
			LoadString(hInstance, IDS_SELF_NID_TITLE, title, 64);
			LoadString(hInstance, IDS_SELF_ERR_FILEWRITE, buf, 64);
			sendSysNotifyMsg(title, buf);
			bResult = FALSE;
			DeleteFile(szFileName);
			break;
		}
		
		bResult = TRUE;
	}while(FALSE);
	
	return bResult;
}














