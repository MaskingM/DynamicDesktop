#ifndef __COMMON__H_
#define __COMMON__H_

#include <windows.h>
#include "res.h"
#include "systemNotify.h"
#include "utilsTool.h"
#include "fileDlg.h"
#include "openCVImageFrame.h"
#include "openCVVedioFrame.h"

#define TIMER_ID		1

#ifdef __cplusplus
extern "C" {
#endif
	
	typedef struct _tagCVGL{
		BaseFrame* frame;
		int width;
		int height;
		BOOL bResize;
		BOOL bDestroy;
		BOOL bDisplay;
		HWND hwnd;
		HBITMAP hBitmap;
		int cxPreClient;
		int cyPreClient;
	} CVGL, *pCVGL;

	enum FILEFORMAT {
		BMP,JPEG,PNG,GIF,
		MPG,MP4,AVI,MOV,RM,MKV,
		UNKNOWNFORMAT=1000
	};

	BOOL initWndClass(HINSTANCE hInstance, WNDCLASS* wndClass);

	BOOL initWindow(HINSTANCE hInstance, HWND* hwnd);

	BOOL CALLBACK EnumWindowsProc(HWND topHandle, LPARAM lParam);

	HWND GetWorkerW();

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int getFileFormat(PBYTE pBits);

	void drawDefaultBk(HWND hwnd, int cxClient, int cyClient);

	DWORD WINAPI ThreadProc(void* pParam);

#ifdef __cplusplus
}
#endif


#endif