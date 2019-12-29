#ifndef __PREREADFILE__H_
#define __PREREADFILE__H_
#include <windows.h>
#define _PREREADSIZE		64
#ifdef __cplusplus
extern "C" {
#endif
	PBYTE preCVReadFile(const char* szFileName);
#ifdef __cplusplus
}
#endif

#endif