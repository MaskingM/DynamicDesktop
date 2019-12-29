#ifndef __FILEDIALOG__H_
#define __FILEDIALOG__H_
#include <windows.h>
#include <commdlg.h>
#include "res.h"
#include "systemNotify.h"
#define PREREADSIZE		64

#ifdef __cplusplus
extern "C" {
#endif

	void initOpenFileName(HWND hwnd, OPENFILENAME* ofn, TCHAR* szFileName, TCHAR* szTitleName, TCHAR* szFilter);
	BOOL openFileDlg(OPENFILENAME* ofn);
	BOOL saveFileDlg(OPENFILENAME* ofn);
	PBYTE readFile(HINSTANCE hInstance, TCHAR* szFileName);
	PBYTE preReadFile(HINSTANCE hInstance, TCHAR* szFileName);
	BOOL saveFile(HINSTANCE hInstance, TCHAR* szFileName, PBYTE pBits, DWORD size);
	
#ifdef __cplusplus
}
#endif

#endif