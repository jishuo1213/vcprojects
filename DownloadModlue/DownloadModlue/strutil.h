#pragma once

#include <curl\curl.h>
#include "DownloadInfo.h"

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);

WCHAR* UrlDecode(LPCSTR szSrc);

TCHAR* GetDocId(TCHAR * url);

std::string BuildProgressResponseJson(FILE_LENGTH speed,FILE_LENGTH downloaded_size,FILE_LENGTH total_size,double time);

char * FileLengthToString(FILE_LENGTH file_size);

FILE_LENGTH StringToFileLength(char *bytes);

std::string BuildSuccessResponseJson();

std::string BuildFailedResponseJson();

std::string BuildRenameFailedJson();