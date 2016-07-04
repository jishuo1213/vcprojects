#pragma once

#include <curl\curl.h>
#include "DownloadInfo.h"

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase);

WCHAR* UrlDecode(LPCSTR szSrc);

TCHAR* GetDocId(char * url);

std::string BuildProgressResponseJson(DownloadInfo *downloadInfo,FILE_LENGTH speed,FILE_LENGTH downloaded_size,FILE_LENGTH total_size,double time);

char * FileLengthToString(FILE_LENGTH file_size);

FILE_LENGTH StringToFileLength(char *bytes);

std::string BuildSuccessResponseJson(DownloadInfo *downloadInfo);

std::string BuildFailedResponseJson(DownloadInfo *downloadInfo);

std::string BuildFailedResponseJson(DownloadInfo *downloadInfo,int code,LPCWSTR msg);

std::string BuildRenameFailedJson(DownloadInfo *downloadInfo);

char* WcharToUTF8_New(LPCWSTR str);

char* WcharToChar_New(LPCWSTR str);

WCHAR* CharToWchar_New(const char *str);

void Log(const TCHAR *info, ...);

void CloseLog();

int GetDownloadType(char *url);