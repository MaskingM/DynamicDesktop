#include "utilsTool.h"

void BoxPrintf(HINSTANCE hInstance, TCHAR* szFormat, ...){
	TCHAR buf[128] = {0};
	TCHAR title[64] = {0};
	
	LoadString(hInstance, IDS_SELF_STR_TITLE, title, 64);
	
	va_list arg_list = NULL;
	va_start(arg_list, szFormat);
	wvsprintf(buf, szFormat, arg_list);
	va_end(arg_list);
	MessageBox(NULL, buf, title, MB_OK);
}