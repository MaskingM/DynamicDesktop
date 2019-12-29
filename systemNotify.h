#ifndef __SYSTEMNOTIFY__H_
#define __SYSTEMNOTIFY__H_
#include <windows.h>
#include "res.h"

#define WM_NOTIFYMSG	(WM_USER + 100)
#define NOTIFY_ID		1001

#ifdef __cplusplus
extern "C" {
#endif

	void initSysNotify(HINSTANCE hInstance, HWND hwnd);
	void sendSysNotifyMsg(TCHAR* szTitle, TCHAR* szContent);

#ifdef __cplusplus
}
#endif


#endif