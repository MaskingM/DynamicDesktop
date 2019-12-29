#include "common.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
	HWND hwnd = NULL;
	WNDCLASS wndClass = {0};
	MSG msg = {0};
	//互斥锁
	
	TCHAR buf[64] = {0};
	LoadString(hInstance, IDS_SELF_STR_PROG, buf, 64);
	
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, buf);
	
	
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		HWND oldHWnd = FindWindow(NULL, buf);
		if(oldHWnd){
			//激活显示找到的已运行的程序
			ShowWindow(oldHWnd, SW_RESTORE);
			//将已运行的程序设置为当前窗口
			SetForegroundWindow(oldHWnd);
		}
		return 0;
	}
	
	if(!initWndClass(hInstance, &wndClass)){
		return -1;
	}
	if(!initWindow(hInstance, &hwnd)){
		return -1;
	}
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	
	
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//解锁互斥锁
	ReleaseMutex(m_hMutex);
    CloseHandle(m_hMutex);
    m_hMutex = NULL;
	return msg.wParam;
}