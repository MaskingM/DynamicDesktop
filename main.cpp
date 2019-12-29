#include "common.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
	HWND hwnd = NULL;
	WNDCLASS wndClass = {0};
	MSG msg = {0};
	//������
	
	TCHAR buf[64] = {0};
	LoadString(hInstance, IDS_SELF_STR_PROG, buf, 64);
	
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, buf);
	
	
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		HWND oldHWnd = FindWindow(NULL, buf);
		if(oldHWnd){
			//������ʾ�ҵ��������еĳ���
			ShowWindow(oldHWnd, SW_RESTORE);
			//�������еĳ�������Ϊ��ǰ����
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
	//����������
	ReleaseMutex(m_hMutex);
    CloseHandle(m_hMutex);
    m_hMutex = NULL;
	return msg.wParam;
}