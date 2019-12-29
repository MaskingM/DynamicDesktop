#include "systemNotify.h"
static NOTIFYICONDATA nid = {0};

void initSysNotify(HINSTANCE hInstance, HWND hwnd){
	TCHAR buf[64] = {0};
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = NOTIFY_ID;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_NOTIFYMSG;
	nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SELF_ICON_MAIN));
	LoadString(hInstance, IDS_SELF_NID_TIPS, buf, 64);
	lstrcpy(nid.szTip, buf);
	
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void sendSysNotifyMsg(TCHAR* szTitle, TCHAR* szContent){
	lstrcpy(nid.szInfoTitle, szTitle);
	lstrcpy(nid.szInfo, szContent);
	nid.uTimeout = 1000;
	Shell_NotifyIcon(NIM_MODIFY, &nid);
}