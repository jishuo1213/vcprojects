#pragma once

#include <curl\curl.h>

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);

WCHAR* UrlDecode(const char* szSrc);

TCHAR* GetDocId(TCHAR * url);