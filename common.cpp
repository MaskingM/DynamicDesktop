#include "common.h"
#define WM_TASKBAR_CREATED		0xC085
HWND _workerw = NULL;

int readSettings(HINSTANCE hInstance, TCHAR* szFileName, int* format){
	HANDLE hFile = NULL;
	DWORD dwFileSize = 0, dwHighSize = 0, dwBytesRead = 0;
	BOOL bSuccess = FALSE;
	TCHAR* pBits = NULL;
	*format = UNKNOWNFORMAT;
	int type = 0;
	
	do{
		hFile = CreateFile(
			TEXT("setting.ini"),
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
		pBits = (TCHAR*)malloc(MAX_PATH);
		if(!pBits){
			CloseHandle(hFile);
			break;
		}
		bSuccess = ReadFile(hFile, pBits, dwFileSize, &dwBytesRead, NULL);
		CloseHandle(hFile);
		if(!bSuccess || (dwFileSize != dwBytesRead)){
			free(pBits);
			pBits = NULL;
			break;
		}
		if(pBits[0] && pBits[1]){
			PBYTE buf = preReadFile(hInstance, pBits);
			*format = getFileFormat(buf);
			free(buf);
		}
		
		if(
			*format == BMP || *format == JPEG || *format == PNG || *format == GIF
		){
			type = 1;
			lstrcpy(szFileName, pBits);
		}else if(
			*format == MPG || *format == MP4 || *format == AVI || *format == MOV || *format == RM || *format == MKV
		){
			type = 2;
			lstrcpy(szFileName, pBits);
		}
		free(pBits);
	}while(FALSE);
	return type;
}

void writeSetting(TCHAR* szFileName){
	DWORD dwBytesWritten = 0;
	HANDLE hFile = NULL;
	TCHAR buf[MAX_PATH] = {0};
	GetModuleFileName(NULL, buf, MAX_PATH);
	int idx = lstrlen(buf);
	while(1){
		idx--;
		if(buf[idx] == '\\' || buf[idx] == '/'){
			lstrcat(buf, TEXT("setting.ini"));
			break;
		}
		buf[idx] = '\0';
		if(idx == 0){
			break;
		}
	}
	do{
		if(INVALID_HANDLE_VALUE == (hFile = CreateFile(buf, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL))){
			break;
		}
		WriteFile(hFile, szFileName, strlen(szFileName) + sizeof(TCHAR), &dwBytesWritten, NULL);
		
		CloseHandle (hFile);
	}while(FALSE);
}


DWORD WINAPI ThreadProc(void* pParam){
	pCVGL pCvgl = (pCVGL)pParam;
	pCvgl->frame->setContext(1);
	GLFWwindow* window = pCvgl->frame->getFrameWindow();
	HWND hwnd = (HWND)(pCvgl->frame->getFrameWndHandle());
	SetParent(hwnd, pCvgl->hwnd);
	int h = -(GetSystemMetrics(SM_CYSMICON)+GetSystemMetrics(SM_CYFIXEDFRAME)/2);
	MoveWindow(hwnd, 0, 0, pCvgl->width, pCvgl->height, FALSE);
	SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE);
	while(!pCvgl->bDestroy){
		if(pCvgl->bResize){
			pCvgl->bResize = FALSE;
			pCvgl->frame->setFrameSize(pCvgl->width, pCvgl->height);
		}
		if(pCvgl->bDisplay){
			pCvgl->frame->displayFrame();
		}
	}
	pCvgl->frame->setContext(0);
	return 0;
}

BOOL initWndClass(HINSTANCE hInstance, WNDCLASS* wndClass){
	BOOL result = TRUE;
	TCHAR szProgName[64] = {0};
	LoadString(hInstance, IDS_SELF_STR_PROG, szProgName, 64);
	wndClass->style = 0;//CS_HREDRAW | CS_VREDRAW;
	wndClass->lpfnWndProc = WndProc;
	wndClass->cbClsExtra = 0;
	wndClass->cbWndExtra = 0;
	wndClass->cbWndExtra = 0;
	wndClass->hInstance = hInstance;
	wndClass->hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SELF_ICON_MAIN));
	wndClass->hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass->lpszMenuName = NULL;
	wndClass->lpszClassName = szProgName;
	if(!RegisterClass(wndClass)){
		TCHAR errBuf[64] = {0};
		LoadString(hInstance, IDS_SELF_ERR_CLASS, errBuf, 64);
		BoxPrintf(hInstance, errBuf);
		result = FALSE;
	}
	
	return result;
}


BOOL initWindow(HINSTANCE hInstance, HWND* hwnd){
	BOOL result = TRUE;
	TCHAR szProgName[64] = {0};
	TCHAR szTitlName[64] = {0};
	LoadString(hInstance, IDS_SELF_STR_PROG, szProgName, 64);
	LoadString(hInstance, IDS_SELF_STR_NAME, szTitlName, 64);
	
	int width = 640, height = 480;
	int maxWidth = GetSystemMetrics(SM_CXSCREEN);
	int maxHeight = GetSystemMetrics(SM_CYSCREEN);
	
	*hwnd = CreateWindow(
		szProgName,
		szTitlName,
		WS_POPUP,
		0,
		0,
		maxWidth,
		maxHeight,
		NULL,
		NULL,
		hInstance,
		0
	);
	if(NULL == *hwnd){
		TCHAR errBuf[64] = {0};
		LoadString(hInstance, IDS_SELF_ERR_WND, errBuf, 64);
		BoxPrintf(hInstance, errBuf);
		result = FALSE;
	}
	
	return result;
}

BOOL CALLBACK EnumWindowsProc(HWND topHandle, LPARAM lParam){
	// 查找 SHELLDLL_DefView 窗体句柄
	// 存在多个WorkerW窗体，只有底下存在SHELLDLL_DefView的才是 SHELLDLL_DefView 表示桌面图标，图标下方则是壁纸层
	HWND hWndShl = FindWindowEx(topHandle, NULL, TEXT("SHELLDLL_DefView"), NULL);
	if (hWndShl == NULL) {
		return true;
	}
	
	// win7或更高版本
	// 查找 WorkerW 窗口句柄(以桌面窗口为父窗口)，找到壁纸层
	_workerw = FindWindowEx(NULL, topHandle, TEXT("WorkerW"), NULL);
	
	if(_workerw == NULL){
		return true;
	}
	return false;
}

HWND GetWorkerW(){
	DWORD_PTR result = 0;
	// 先找到 Program Manager 句柄
    HWND windowHandle = FindWindow(TEXT("Progman"), NULL);
	if(windowHandle == NULL){
		sendSysNotifyMsg(TEXT("动态桌面"), TEXT("未找到 Program Manage 句柄"));
		return NULL;
	}
	// 发送消息到Program Manager窗口
	// 要在桌面图标和壁纸之间触发创建WorkerW窗口，必须向Program Manager发送一个消息
	// 这个消息没有一个公开的WindowsAPI来执行，只能使用SendMessageTimeout来发送0x052C
    SendMessageTimeout(windowHandle, 0x052C, 0 ,0, SMTO_NORMAL, 1000,(PDWORD_PTR)&result);
    EnumWindows(EnumWindowsProc, NULL);
	if(_workerw == NULL){
		sendSysNotifyMsg(TEXT("动态桌面"), TEXT("无法获取桌面壁纸层窗体"));
	}else {
		
	}
    return windowHandle;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	int result = 0;
	HDC hdc = NULL;
	PAINTSTRUCT ps = {0};
	POINT pt = {0};
	
	static HINSTANCE hInstance = NULL;
	static HMENU hMenu = NULL;
	static int cxClient = 0, cyClient = 0;
	static OPENFILENAME ofn = {0};
	static TCHAR szFileName[MAX_PATH] = {0};
	static TCHAR szPreFileName[MAX_PATH] = {0};
	static TCHAR szTitleName[MAX_PATH] = {0};
	static TCHAR szFilter[] = TEXT("图片文件 (jpg,jpeg,png)\0*.jpg;*.jpeg;*.png;*.bmp\0") TEXT("全部文件 (*.*)\0*.*\0\0");
		
	static TCHAR szFilter2[] = TEXT("视频文件 (mp4,mpg,avi,mov,rm,mkv)\0*.mp4;*.mpg;*.avi;*.mov;*.rm;*.mkv\0") TEXT("全部文件 (*.*)\0*.*\0\0");

	static BYTE* pBits = NULL;
	static int maxWidth = 0, maxHeight = 0;
	static int format = UNKNOWNFORMAT;
	static BOOL defaultBk = FALSE;
	static BOOL imgBk = FALSE;
	static BOOL videoBk = FALSE;
	
	
	static HANDLE thread = NULL;
	static CVGL cvgl = {0};
	static DWORD threadID = 0;
	static BaseFrame* frame = NULL;
	
	
	static HWND parent = NULL;
	HWND child = 0;
	
	int type = 0;
	
	static HGLOBAL hVGlobale = NULL;
	static HGLOBAL hFGlobale = NULL;
	static char vShaderCode[4096] = {0};
	static char fShaderCode[4096] = {0};
	char* pText = NULL;
	
	switch(msg){
		case WM_CREATE:
			hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
			initSysNotify(hInstance, hwnd);
			hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING, IDM_SELF_IMGBK, TEXT("选择壁纸图片"));
			AppendMenu(hMenu, MF_STRING, IDM_SELF_VEDIOBK, TEXT("选择壁纸视频"));
			AppendMenu(hMenu, MF_STRING, IDM_SELF_DEFAULT, TEXT("使用默认背景"));
			AppendMenu(hMenu, MF_STRING, IDM_SELF_EXIT, TEXT("退出软件"));
			
			initOpenFileName(hwnd, &ofn, szFileName, szTitleName, szFilter);
			
			//读取配置文件，查看当前是否有
			maxWidth = GetSystemMetrics(SM_CXSCREEN);
			maxHeight = GetSystemMetrics(SM_CYSCREEN);
			
			
			hVGlobale = LoadResource(hInstance, FindResource(hInstance, TEXT("BASEVSHADER"), TEXT("TEXT")));
			pText = (char *)LockResource(hVGlobale);
			lstrcpy(vShaderCode, pText);
			FreeResource(hVGlobale);
			
			hFGlobale = LoadResource(hInstance, FindResource(hInstance, TEXT("BASEFSHADER"), TEXT("TEXT")));
			pText = (char *)(char *)LockResource(hFGlobale);
			lstrcpy(fShaderCode, pText);
			FreeResource(hFGlobale);
			
			parent = GetWorkerW();
			if(_workerw){
				SetParent(hwnd, parent);
				ShowWindow(_workerw, SW_HIDE);
			}else {
				MoveWindow(hwnd, 0, 0, maxWidth / 2, maxHeight / 2, TRUE);
			}
			
			SetWindowPos(hwnd,HWND_TOP,0,0,0,0,WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_NOACTIVATE);
			ShowWindow(hwnd, SW_SHOW);
			
			type = readSettings(hInstance, szFileName, &format);
			defaultBk = TRUE;
			if(type == 0){
				defaultBk = TRUE;
			}else if(type == 1){
				cvgl.bDestroy = TRUE;
				if(thread){
					WaitForSingleObject(thread, INFINITE);
					CloseHandle(thread);
					thread = NULL;
				}
				frame = new ImageFrame(TEXT("GLFrame"), vShaderCode, fShaderCode);
				
				child = (HWND)(frame->getFrameWndHandle());
				SetParent(child, hwnd);
				MoveWindow(child, 0, 0, 1, 1, FALSE);
				SetWindowLong(child, GWL_STYLE, WS_DISABLED);
				
				cvgl.frame = frame;
				cvgl.bDestroy = FALSE;
				cvgl.bDisplay = TRUE;
				cvgl.hwnd = hwnd;
				frame->loadResource(szFileName);
				frame->setContext(0);
				//Sleep(frame->getFrameDelay());
				thread = CreateThread(NULL, 0, ThreadProc, ((PVOID)&cvgl), 0, &threadID);
				imgBk = TRUE;
				lstrcpy(szPreFileName, szFileName);
			}else if(type == 2){
				cvgl.bDestroy = TRUE;
				if(thread){
					WaitForSingleObject(thread, INFINITE);
					CloseHandle(thread);
					thread = NULL;
				}
				frame = new VedioFrame(TEXT("GLFrame"), vShaderCode, fShaderCode);
				
				child = (HWND)(frame->getFrameWndHandle());
				SetParent(child, hwnd);
				MoveWindow(child, 0, 0, 1, 1, FALSE);
				SetWindowLong(child, GWL_STYLE, WS_DISABLED);
				
				cvgl.frame = frame;
				cvgl.bDestroy = FALSE;
				cvgl.bDisplay = TRUE;
				cvgl.hwnd = hwnd;
				frame->loadResource(szFileName);
				frame->setContext(0);
				//Sleep(frame->getFrameDelay());
				thread = CreateThread(NULL, 0, ThreadProc, ((PVOID)&cvgl), 0, &threadID);
				videoBk = TRUE;
				lstrcpy(szPreFileName, szFileName);
			}
			
			
			break;
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			cvgl.bResize = TRUE;
			cvgl.width = cxClient;
			cvgl.height = cyClient;
			break;
		case 0x052C:
			Sleep(100);
			GetWorkerW();
			ShowWindow(_workerw, SW_HIDE);
			break;
		case WM_ERASEBKGND:
			break;
		case WM_NOTIFYMSG:
			switch(lParam){
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
					GetCursorPos(&pt);
					SetForegroundWindow(hwnd);
					TrackPopupMenu(hMenu, TPM_LEFTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
					break;
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDM_SELF_IMGBK:
				case IDM_SELF_VEDIOBK:
					if(LOWORD(wParam) == IDM_SELF_IMGBK){
						ofn.lpstrFilter = szFilter;
						ofn.lpstrDefExt = TEXT("jpg");
						ofn.nFilterIndex = 0;
					}else {
						ofn.lpstrFilter = szFilter2;
						ofn.lpstrDefExt = TEXT("mp4");
						ofn.nFilterIndex = 0;
					}
					EnableMenuItem(hMenu, IDM_SELF_IMGBK, MF_GRAYED);
					EnableMenuItem(hMenu, IDM_SELF_VEDIOBK, MF_GRAYED);
					EnableMenuItem(hMenu, IDM_SELF_DEFAULT, MF_GRAYED);
					EnableMenuItem(hMenu, IDM_SELF_EXIT, MF_GRAYED);
					if(openFileDlg(&ofn)){
						//上次打开的文件
						if(!lstrcmp(szFileName, szPreFileName) && cvgl.bDestroy){
							//显示之前的文件
							if(
								format == BMP || format == JPEG || format == PNG || format == GIF
								|| format == MPG || format == MP4 || format == AVI || format == MOV || format == RM || format == MKV
							){
								if(frame){
									cvgl.bDestroy = TRUE;
									
									if(frame){
										Sleep(frame->getFrameDelay() * 3);
									}
									if(thread){
										WaitForSingleObject(thread, INFINITE);
										CloseHandle(thread);
										thread = NULL;
									}
									cvgl.bDestroy = FALSE;
									cvgl.bDisplay = TRUE;
									frame->setContext(0);
									thread = CreateThread(NULL, 0, ThreadProc, ((PVOID)&cvgl), 0, &threadID);
									imgBk = TRUE;
								}
							}
							EnableMenuItem(hMenu, IDM_SELF_IMGBK, MF_ENABLED);
							EnableMenuItem(hMenu, IDM_SELF_VEDIOBK, MF_ENABLED);
							EnableMenuItem(hMenu, IDM_SELF_DEFAULT, MF_ENABLED);
							EnableMenuItem(hMenu, IDM_SELF_EXIT, MF_ENABLED);
							break;
						}
						if(pBits){
							free(pBits);
							pBits = NULL;
						}
						SetCursor(LoadCursor(NULL, IDC_WAIT));
						ShowCursor(TRUE);
						pBits = preReadFile(hInstance, szFileName);
						if(pBits){
							format = getFileFormat(pBits);
							free(pBits);
							pBits = NULL;
							defaultBk = FALSE;
							imgBk = FALSE;
							videoBk = FALSE;
							if(LOWORD(wParam) == IDM_SELF_IMGBK){
								if(
									format == BMP || format == JPEG || format == PNG || format == GIF
								){
									if(frame){
										cvgl.bDestroy = TRUE;
										if(frame){
											Sleep(frame->getFrameDelay() * 3);
										}
										if(thread){
											WaitForSingleObject(thread, INFINITE);
											CloseHandle(thread);
											thread = NULL;
										}
										BaseFrame* cur = dynamic_cast<ImageFrame*>(frame);
										if(!cur){
											delete frame;
											frame = NULL;
											frame = new ImageFrame(TEXT("GLFrame"), vShaderCode, fShaderCode);
										}
										cvgl.frame = frame;
										cvgl.bDestroy = FALSE;
										cvgl.bDisplay = TRUE;
										cvgl.bResize = TRUE;
										cvgl.hwnd = hwnd;
										frame->setContext(1);
										if(frame->loadResource(szFileName)){
											frame->setContext(0);
											thread = CreateThread(NULL, 0, ThreadProc, ((PVOID)&cvgl), 0, &threadID);
											imgBk = TRUE;
											lstrcpy(szPreFileName, szFileName);
										}
									}else {
										//重新创建图像帧窗口
										frame = new ImageFrame(TEXT("GLFrame"), vShaderCode, fShaderCode);
										cvgl.frame = frame;
										cvgl.bDestroy = FALSE;
										cvgl.bDisplay = TRUE;
										cvgl.bResize = TRUE;
										cvgl.hwnd = hwnd;
										child = (HWND)(frame->getFrameWndHandle());
										SetParent(child, hwnd);
										MoveWindow(child, 0, 0, 1, 1, FALSE);
										SetWindowLong(child, GWL_STYLE, WS_DISABLED);
										if(frame->loadResource(szFileName)){
											frame->setContext(0);
											//Sleep(frame->getFrameDelay());
											thread = CreateThread(NULL, 0, ThreadProc, ((PVOID)&cvgl), 0, &threadID);
											imgBk = TRUE;
											lstrcpy(szPreFileName, szFileName);
										}
									}
								}
							}else {
								if(
									format == MPG || format == MP4 || format == AVI || format == MOV || format == RM || format == MKV
								){
									if(frame){
										cvgl.bDestroy = TRUE;
										if(frame){
											Sleep(frame->getFrameDelay() * 3);
										}
										if(thread){
											WaitForSingleObject(thread, INFINITE);
											CloseHandle(thread);
											thread = NULL;
										}
										BaseFrame* cur2 = dynamic_cast<VedioFrame*>(frame);
										if(!cur2){
											delete frame;
											frame = NULL;
											frame = new VedioFrame(TEXT("GLFrame"), vShaderCode, fShaderCode);
										}
										cvgl.frame = frame;
										cvgl.bDestroy = FALSE;
										cvgl.bDisplay = TRUE;
										cvgl.bResize = TRUE;
										cvgl.hwnd = hwnd;
										frame->setContext(1);
										if(frame->loadResource(szFileName)){
											frame->setContext(0);
											//Sleep(frame->getFrameDelay());
											thread = CreateThread(NULL, 0, ThreadProc, ((PVOID)&cvgl), 0, &threadID);
											videoBk = TRUE;
											lstrcpy(szPreFileName, szFileName);
										}
									}else {
										//重新创建视频帧窗口
										frame = new VedioFrame(TEXT("GLFrame"), vShaderCode, fShaderCode);
										cvgl.frame = frame;
										cvgl.bDestroy = FALSE;
										cvgl.bDisplay = TRUE;
										cvgl.bResize = TRUE;
										cvgl.hwnd = hwnd;
										child = (HWND)(frame->getFrameWndHandle());
										SetParent(child, hwnd);
										MoveWindow(child, 0, 0, 1, 1, FALSE);
										SetWindowLong(child, GWL_STYLE, WS_DISABLED);
										if(frame->loadResource(szFileName)){
											frame->setContext(0);
											//Sleep(frame->getFrameDelay());
											thread = CreateThread(NULL, 0, ThreadProc, ((PVOID)&cvgl), 0, &threadID);
											videoBk = TRUE;
											lstrcpy(szPreFileName, szFileName);
										}
									}
								}
								
							}
							writeSetting(szFileName);
						}
						SetCursor(LoadCursor(NULL, IDC_ARROW));
						ShowCursor(FALSE);
						
					};
					EnableMenuItem(hMenu, IDM_SELF_IMGBK, MF_ENABLED);
					EnableMenuItem(hMenu, IDM_SELF_VEDIOBK, MF_ENABLED);
					EnableMenuItem(hMenu, IDM_SELF_DEFAULT, MF_ENABLED);
					EnableMenuItem(hMenu, IDM_SELF_EXIT, MF_ENABLED);
					break;
				case IDM_SELF_DEFAULT:
					defaultBk = TRUE;
					
					if(imgBk || videoBk){
						cvgl.bDestroy = TRUE;
						if(frame){
							Sleep(frame->getFrameDelay());
						}
						if(thread){
							WaitForSingleObject(thread, INFINITE);
							CloseHandle(thread);
							thread = NULL;
						}
						frame->setContext(1);
						child = (HWND)(frame->getFrameWndHandle());
						SetParent(child, hwnd);
						MoveWindow(child, 0, 0, 0, 0, FALSE);
						SetWindowLong(child, GWL_STYLE, WS_DISABLED);
						imgBk = FALSE;
						videoBk = FALSE;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					break;
				case IDM_SELF_EXIT:
					SendMessage(hwnd, WM_DESTROY, 0, 0);
					break;
			}
			break;
		case WM_TASKBAR_CREATED:
			initSysNotify(hInstance, hwnd);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			if(defaultBk){
				drawDefaultBk(hwnd, cxClient, cyClient);
			}
			EndPaint(hwnd, &ps);
			break;;
		case WM_DESTROY:
			ShowWindow(_workerw,SW_SHOW);
			DestroyMenu(hMenu);
			if(pBits){
				free(pBits);
			}
			//DestroyIcon((HICON)SetClassLong(hwnd, GCL_HICON, (LONG)LoadIcon(NULL, IDI_APPLICATION)));
			cvgl.bDestroy = TRUE;
			if(frame){
				Sleep(frame->getFrameDelay());
			}
			if(thread){
				WaitForSingleObject(thread, INFINITE);
				CloseHandle(thread);
				thread = NULL;
			}
			if(frame){
				delete frame;
				frame = NULL;
			}
			if(cvgl.hBitmap){
				DeleteObject(cvgl.hBitmap);
			}
			
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProc(hwnd, msg, wParam, lParam);
	}
	
	
	return result;
}

int getFileFormat(PBYTE pBits){
	int format = UNKNOWNFORMAT;
	do{
		if(!pBits){
			break;
		}
		// BMP格式
		if(pBits[0] == 0x42 && pBits[1] == 0x4D){
			format = BMP;
			break;
		}
		// JPEG格式
		if(pBits[0] == 0xFF && pBits[1] == 0xD8){
			format = JPEG;
			break;
		}
		// PNG格式
		if(
			pBits[0] == 0x89
			&& pBits[1] == 0x50
			&& pBits[2] == 0x4E
			&& pBits[3] == 0x47
			&& pBits[4] == 0x0D
			&& pBits[5] == 0x0A
			&& pBits[6] == 0x1A
			&& pBits[7] == 0x0A
		){
			format = PNG;
			break;
		}
		// GIF格式
		if(
			pBits[0] == 0x47
			&& pBits[1] == 0x49
			&& pBits[2] == 0x46
			&& pBits[3] == 0x38
			&& (pBits[4] == 0x39 || pBits[4] == 0x37)
			&& pBits[5] == 0x61
		){
			format = PNG;
			break;
		}
		
		// MPG格式 0x000001BA  或 0x000001B3 
		if(
			pBits[0] == 0x00
			&& pBits[1] == 0x00
			&& pBits[2] == 0x01
			&& (pBits[3] == 0xBA || pBits[4] == 0xB3)
		){
			format = MPG;
			break;
		}
		
		if(
			pBits[4] == 0x66
			&& pBits[5] == 0x74
			&& pBits[6] == 0x79
			&& pBits[7] == 0x70
		){
			format = MP4;
			break;
		}
		
		// AVI格式 0x41564920
		if(
			pBits[0] == 0x41
			&& pBits[1] == 0x56
			&& pBits[2] == 0x49
			&& pBits[3] == 0x20
		){
			format = AVI;
			break;
		}
		
		// MOV格式 0x6D6F6F76
		if(
			pBits[0] == 0x6D
			&& pBits[1] == 0x6F
			&& pBits[2] == 0x6F
			&& pBits[3] == 0x76
		){
			format = MOV;
			break;
		}
		
		// RM格式 0x2E524D46
		if(
			pBits[0] == 0x2E
			&& pBits[1] == 0x52
			&& pBits[2] == 0x4D
			&& pBits[3] == 0x46
		){
			format = RM;
			break;
		}
		if( // mkv 42 82 88 6d 61 74 72 6f 73 6b 61
			pBits[5] == 0x42
			&& pBits[6] == 0x82
			&& pBits[7] == 0x88
			&& pBits[8] == 0x6D
			&& pBits[9] == 0x61
			&& pBits[10] == 0x74
		){
			format = MKV;
			break;
		}
		format = UNKNOWNFORMAT;
	}while(FALSE);
	return format;
}

void drawDefaultBk(HWND hwnd, int cxClient, int cyClient){
	static TCHAR* texts[4] = {
		TEXT("路见不平一声吼，吼完继续往前走"),
		TEXT("生活嘛，就是生下来，活下去"),
		TEXT("思想有多远，你就给我滚多远"),
		TEXT("比三观更重要的是五官")
	};
	static COLORREF colors[4] = {
		RGB(29,116,224),
		RGB(128,202,21),
		RGB(202,71,21),
		RGB(115,21,202)
	};
	HDC hdc = GetDC(hwnd);
	int height = -MulDiv(32, GetDeviceCaps(hdc, LOGPIXELSY), 96);
	HFONT hFont = CreateFont(
		height,0,0,0,0,0,0,0,0,0,0,0,0,TEXT("微软雅黑")
	);
	HBRUSH hBrush = NULL;
	HPEN hPen = NULL;
	SIZE size = {0};
	int w = cxClient / 2;
	int h = cyClient / 2;
	int i = 0;
	
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(255,255,255));
	SetBkMode(hdc, TRANSPARENT);
	
	for(i = 0; i < 4; i++){
		hBrush = CreateSolidBrush(colors[i]);
		hPen = CreatePen(PS_SOLID, 1, colors[i]);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		GetTextExtentPoint32(hdc, texts[i], lstrlen(texts[i]), &size);
		
		int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
		
		if(i == 0){
			x1 = cxClient / 2;
			y1 = cyClient / 2;
		}else if(i == 1){
			x0 = cxClient / 2;
			y0 = 0;
			x1 = cxClient;
			y1 = cyClient / 2;
		}else if(i == 2){
			x0 = 0;
			y0 = cyClient / 2;
			x1 = cxClient / 2;
			y1 = cyClient;
		}else if(i == 3){
			x0 = cxClient / 2;
			y0 = cyClient / 2;
			x1 = cxClient;
			y1 = cyClient;
		}
		Rectangle(hdc, x0, y0, x1, y1);
		TextOut(hdc, (x0 + (w - size.cx) / 2), (y0 + (h - size.cy) / 2), texts[i], lstrlen(texts[i]));
		DeleteObject(hBrush);
		DeleteObject(hPen);
	}
	
	
	DeleteObject(hFont);
	ReleaseDC(hwnd, hdc);
}






