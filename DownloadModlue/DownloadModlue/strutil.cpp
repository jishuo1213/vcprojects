#include "stdafx.h"
#include <stdarg.h>
#include <ctime>
#include<algorithm>

const char* MimeTypes[][2];


//static FILE* logfile;
LPCSTR trim(const char *str);
bool isHexNum(char c);

std::string BuildProgressResponseJson(DownloadInfo *downloadInfo, FILE_LENGTH speed, FILE_LENGTH downloaded_size, FILE_LENGTH total_size, double time) {
	Json::Value info;
	info["speed"] = speed;
	info["downloaded"] = downloaded_size;
	info["total"] = total_size;
	info["duration"] = time;
	Json::Value responese;
	responese["status_code"] = 1;
	responese["uuid"] = downloadInfo->GetUUid();
	responese["info"] = info;
	//Log()
	std::string res = responese.toStyledString();
	//std::remove(res.begin(), res.end(),'\n');
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}

std::string BuildSuccessResponseJson(DownloadInfo *downloadInfo) {
	Json::Value info;
	char* message = WcharToUTF8_New(_T("download success"));
	info["message"] = message;
	Json::Value responese;
	responese["status_code"] = 0;
	responese["uuid"] = downloadInfo->GetUUid();
	responese["info"] = info;
	delete[] message;
	std::string res = responese.toStyledString();
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}



std::string BuildRenameFailedJson(DownloadInfo *downloadInfo) {
	Json::Value info;
	char* message = WcharToUTF8_New(_T("rename failed"));
	info["message"] = message;
	Json::Value responese;
	responese["status_code"] = 3;
	responese["info"] = info;
	responese["uuid"] = downloadInfo->GetUUid();
	delete[] message;
	std::string res = responese.toStyledString();
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}

std::string BuildFailedResponseJson(DownloadInfo *downloadInfo) {
	Json::Value info;
	char* message = WcharToUTF8_New(_T("download failed net work failed"));
	info["message"] = message;
	Json::Value responese;
	responese["status_code"] = 2;
	responese["info"] = info;
	responese["uuid"] = downloadInfo->GetUUid();
	delete[] message;
	std::string res = responese.toStyledString();
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}

std::string BuildFailedResponseJson(DownloadInfo * downloadInfo, int code, LPCWSTR msg) {
	Json::Value info;
	char* message = WcharToUTF8_New(msg);
	info["message"] = message;
	Json::Value responese;
	responese["status_code"] = code;
	responese["info"] = info;
	responese["uuid"] = downloadInfo->GetUUid();
	delete[] message;
	std::string res = responese.toStyledString();
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase) {
	if (szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
		return FALSE;

	size_t len_ascii;
	len_ascii = strlen(szSrc);
	if (len_ascii == 0) {
		pBuf[0] = 0;
		return TRUE;
	}

	//先转换到UTF-8
	char baseChar = bUpperCase ? 'A' : 'a';
	int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, NULL, 0);
	LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
	if (pUnicode == NULL)
		return FALSE;
	MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, pUnicode, cchWideChar + 1);

	int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
	LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
	if (pUTF8 == NULL) {
		free(pUnicode);
		return FALSE;
	}
	WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
	pUTF8[cbUTF8] = '\0';

	unsigned char c;
	int cbDest = 0; //累加
	unsigned char *pSrc = (unsigned char*)pUTF8;
	unsigned char *pDest = (unsigned char*)pBuf;
	while (*pSrc && cbDest < cbBufLen - 1) {
		c = *pSrc;
		if (isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~') {
			*pDest = c;
			++pDest;
			++cbDest;
		} else if (c == ' ') {
			*pDest = '+';
			++pDest;
			++cbDest;
		} else {
			//检查缓冲区大小是否够用？
			if (cbDest + 3 > cbBufLen - 1)
				break;
			pDest[0] = '%';
			pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
			pDest[2] = ((c & 0xF) >= 0xA) ? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
			pDest += 3;
			cbDest += 3;
		}
		++pSrc;
	}
	//null-terminator
	*pDest = '\0';
	free(pUnicode);

	free(pUTF8);
	return TRUE;
}

/*
这个函数返回的值会分配内存，注意回收
*/
WCHAR* UrlDecode(LPCSTR szSrc) {
	if (szSrc == NULL)
		return FALSE;
	LPCSTR trimSrc = trim(szSrc);
	size_t len_ascii = strlen(trimSrc);
	if (len_ascii == 0) {
		return NULL;
	}
	CHAR *pUTF8 = new CHAR[len_ascii + 1];
	ZeroMemory(pUTF8, len_ascii + 1);
	if (pUTF8 == NULL)
		return NULL;

	LPSTR pDest = pUTF8;
	LPCSTR pSrc = trimSrc;
	size_t i = 0;
	while (*pSrc) {
		if (*pSrc == '%') {
			*pDest = 0;
			//高位
			if (pSrc[1] >= 'A' && pSrc[1] <= 'F')
				*pDest += (pSrc[1] - 'A' + 10) * 0x10;
			else if (pSrc[1] >= 'a' && pSrc[1] <= 'f')
				*pDest += (pSrc[1] - 'a' + 10) * 0x10;
			else
				*pDest += (pSrc[1] - '0') * 0x10;

			//低位
			if (pSrc[2] >= 'A' && pSrc[2] <= 'F')
				*pDest += (pSrc[2] - 'A' + 10);
			else if (pSrc[2] >= 'a' && pSrc[2] <= 'f')
				*pDest += (pSrc[2] - 'a' + 10);
			else
				*pDest += (pSrc[2] - '0');

			pSrc += 3;
		} else if (*pSrc == '+') {
			*pDest = ' ';
			++pSrc;
		} else {
			*pDest = *pSrc;
			++pSrc;
		}
		++pDest;
	}

	int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, NULL, 0);
	LPWSTR pUnicode = new WCHAR[cchWideChar + 1];
	ZeroMemory(pUnicode, (cchWideChar + 1) * sizeof(WCHAR));

	MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, pUnicode, cchWideChar);
	delete[] pUTF8;
	delete[] trimSrc;
	return pUnicode;
}

TCHAR* parse_url(TCHAR *start, TCHAR *end, int prefix_length) {
	int length = end - start - prefix_length;
	TCHAR *files_id = new TCHAR[length + 1];
	ZeroMemory(files_id, length + 1);
	_tcsnccpy_s(files_id, length + 1, start + prefix_length, length);
	return files_id;
}

/*
这个函数返回的值会分配内存，注意回收
*/
TCHAR* GetDocId(char *url) {
	std::string url_md5 = md5((std::string(url)));
	return CharToWchar_New(url_md5.c_str());
}

int GetDownloadType(char *url) {
	if (strstr(url, "doc_id=") != NULL) {//普通下载
		return 0;
	} else if (strstr(url, ("fileids=")) != NULL &&  strstr(url, ("folderids=")) != NULL) {//批量下载
		return 1;
	} else if (strstr(url, ("folderids=")) != NULL) {
		return 2;
	} else {
		Log(_T("%s"), _T("url错误,没有找到docid folderid fileid"));
		return -1;
	}
}

/*
这个函数返回的值会分配内存，注意回收
*/
LPCSTR trim(const char *str) {
	std::string* s = new std::string(str);
	auto ch = s->cend() - 1;
	int pos = s->length() - 1;
	while (isspace(*ch)) {
		--pos;
		--ch;
	}
	s->erase(pos + 1);
	// s->erase(std::remove_if(s->begin(),s->end(),isspace),s->end());
	LPSTR res = new CHAR[s->length() + 1];
	strcpy_s(res, s->length() + 1, s->c_str());
	delete s;
	return res;
}

bool isHexNum(char c) {
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}


char* FileLengthToString(FILE_LENGTH file_size) {
	std::stringstream a;
	a << file_size;
	int length = a.str().length() + 1;
	char* bytes = new char[length];
	std::string s = a.str();
	const char* size = s.c_str();
	strcpy_s(bytes, length, size);
	return bytes;
}

FILE_LENGTH StringToFileLength(char *bytes) {
	std::stringstream strValue;
	strValue << bytes;
	FILE_LENGTH value = 0;
	strValue >> value;

	return value;
}

char* WcharToUTF8_New(LPCWSTR str) {
	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	char* dest = new char[len + 1];
	ZeroMemory(dest, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, dest, len, NULL, NULL);
	return dest;
}

char* WcharToChar_New(LPCWSTR str) {
	int  iLength = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *url = new char[iLength + 1];
	ZeroMemory(url, iLength + 1);
	WideCharToMultiByte(CP_ACP, 0, str, -1, url, iLength, NULL, NULL);
	return url;
}

WCHAR* CharToWchar_New(const char *str) {
	int nLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	if (nLen == 0) {
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen + 1];
	ZeroMemory(pResult, nLen + 1);
	MultiByteToWideChar(CP_ACP, 0, str, -1, pResult, nLen);
	return pResult;
}



void Log(const TCHAR *format, ...) {
	if (true) {
		FILE *logfile = NULL;
		TCHAR buffer[MAX_PATH];
		_tgetcwd(buffer, MAX_PATH);
		_tcscat_s(buffer, MAX_PATH, _T("\\download.log"));
		int res = _tfopen_s(&logfile, buffer, _T("a,ccs=UTF-8"));

		if (res != 0)
			return;
		time_t now_time;
		now_time = time(NULL);
		struct tm now;
		localtime_s(&now, &now_time);
		//strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
		char buf[30];
		ZeroMemory(buf, 30);
		strftime(buf, 30, "%Y-%m-%d %H:%M:%S", &now);
		WCHAR *w_buf = CharToWchar_New(buf);
		_ftprintf(logfile, _T("%s:"), w_buf);
		delete[] w_buf;
		va_list args;
		va_start(args, format);
		_vfwprintf_l(logfile, format, NULL, args);
		va_end(args);
		_ftprintf(logfile, _T("\n"));
		fclose(logfile);
	}
}


void Log(const CHAR *format, ...) {
	if (true) {
		FILE *logfile = NULL;
		TCHAR buffer[MAX_PATH];
		_tgetcwd(buffer, MAX_PATH);
		_tcscat_s(buffer, MAX_PATH, _T("\\download.log"));
		int res = _tfopen_s(&logfile, buffer, _T("a"));

		if (res != 0)
			return;
		time_t now_time;
		now_time = time(NULL);
		struct tm now;
		localtime_s(&now, &now_time);
		//strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
		char buf[30];
		ZeroMemory(buf, 30);
		strftime(buf, 30, "%Y-%m-%d %H:%M:%S", &now);
		//WCHAR *w_buf = CharToWchar_New(buf);
		fprintf(logfile, "%s:", buf);
		//delete[] w_buf;
		va_list args;
		va_start(args, format);
		_vfprintf_l(logfile, format, NULL, args);
		va_end(args);
		fprintf(logfile, "\n");
		fclose(logfile);
	}
}

void CloseLog() {
	/*if (logfile)
		fclose(logfile);*/
}



static char* ConvertToUpper(char* szSource) {
	if (szSource == NULL)
		return NULL;

	int len = strlen(szSource);
	char *szString = new char[len + 1];
	ZeroMemory(szString, len + 1);
	strcpy_s(szString, len + 1, szSource);

	char *start = szString;
	while (*szString) {
		*szString = (char)toupper(*szString);
		szString++;
	}
	return start;
}


const char* GetMimeType(char* szFileExt) {
	char *upperExt = ConvertToUpper(szFileExt);

	int length = 88;
	for (int i = 0; i < length; i++) {
		if (strcmp(MimeTypes[i][0], upperExt) == 0) {
			delete[] upperExt;
			return MimeTypes[i][1];
		}
	}
	delete[] upperExt;
	return MimeTypes[0][1];
}

const char* CopyCstring(LPCSTR src) {
	int len = strlen(src);
	char *res = new char[len + 1];
	ZeroMemory(res, len + 1);
	strcpy_s(res, len + 1, src);
	return res;
}

std::string BuildUploadProgressResJson(UploadInfo * uploadInfo, FILE_LENGTH upload_size, time_t total_time) {
	Json::Value info;
	info["uploaded"] = upload_size;
	info["total"] = uploadInfo->GetFileSize();
	info["duration"] = total_time;
	Json::Value responese;
	responese["status_code"] = 1;
	responese["uuid"] = uploadInfo->GetUUid();
	responese["info"] = info;
	std::string res = responese.toStyledString();
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}

std::string BuildUploadFailedResJson(UploadInfo * uploadInfo, int code, LPCSTR ms) {
	Json::Value info;
	info["code"] = code;
	info["msg"] = ms;
	Json::Value responese;
	responese["status_code"] = 2;
	responese["uuid"] = uploadInfo->GetUUid();
	responese["info"] = info;
	std::string res = responese.toStyledString();
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}

std::string BuildUploadSuccessResJson(UploadInfo * uploadInfo, char * server_res) {
	Json::Reader reader;
	Json::Value info;
	reader.parse(server_res, info);
	Json::Value responese;
	responese["status_code"] = 0;
	responese["uuid"] = uploadInfo->GetUUid();
	responese["info"] = info;
	std::string res = responese.toStyledString();
	res.erase(std::remove_if(res.begin(), res.end(), isspace), res.end());
	//Log("%s", res.c_str());
	return res;
}

