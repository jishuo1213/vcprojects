#pragma once

#include <curl\curl.h>

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);

WCHAR* UrlDecode(LPCSTR szSrc);

TCHAR* GetDocId(TCHAR * url);