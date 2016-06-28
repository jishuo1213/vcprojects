#include "stdafx.h"

LPCSTR trim(const char *str);
bool isHexNum(char c);

std::string BuildProgressResponseJson(FILE_LENGTH speed,FILE_LENGTH downloaded_size,FILE_LENGTH total_size,double time)
{
	Json::Value info;
	info["speed"] = speed;
	info["downloaded"] = downloaded_size;
	info["total"] = total_size;
	info["duration"] = time;
	Json::Value responese;
	responese["status_code"] = 1;
	responese["info"] = info;
	return responese.toStyledString();
}

std::string BuildSuccessResponseJson()
{
	Json::Value info;
	info["message"] = "下载成功";
	Json::Value responese;
	responese["status_code"] = 0;
	responese["info"] = info;
	return responese.toStyledString();
}

std::string BuildRenameFailedJson()
{
	Json::Value info;
	info["message"] = "下载成功,但是重命名文件失败";
	Json::Value responese;
	responese["status_code"] = 3;
	responese["info"] = info;
	return responese.toStyledString();
}

std::string BuildFailedResponseJson()
{
	Json::Value info;
	info["message"] = "下载失败，请检查网络";
	Json::Value responese;
	responese["status_code"] = 2;
	responese["info"] = info;
	return responese.toStyledString();
}

BOOL UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, BOOL bUpperCase)
{
    if(szSrc == NULL || pBuf == NULL || cbBufLen <= 0)
        return FALSE;

    size_t len_ascii;
	len_ascii = strlen(szSrc);
    if(len_ascii == 0)
    {
        pBuf[0] = 0;
        return TRUE;
    }
    
    //先转换到UTF-8
    char baseChar = bUpperCase ? 'A' : 'a';
    int cchWideChar = MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, NULL, 0);
    LPWSTR pUnicode = (LPWSTR)malloc((cchWideChar + 1) * sizeof(WCHAR));
    if(pUnicode == NULL)
        return FALSE;
    MultiByteToWideChar(CP_ACP, 0, szSrc, len_ascii, pUnicode, cchWideChar + 1);

    int cbUTF8 = WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, NULL, 0, NULL, NULL);
    LPSTR pUTF8 = (LPSTR)malloc((cbUTF8 + 1) * sizeof(CHAR));
    if(pUTF8 == NULL)
    {
        free(pUnicode);
        return FALSE;
    }
    WideCharToMultiByte(CP_UTF8, 0, pUnicode, cchWideChar, pUTF8, cbUTF8 + 1, NULL, NULL);
    pUTF8[cbUTF8] = '\0';

    unsigned char c;
    int cbDest = 0; //累加
    unsigned char *pSrc = (unsigned char*)pUTF8;
    unsigned char *pDest = (unsigned char*)pBuf;
    while(*pSrc && cbDest < cbBufLen - 1)
    {
        c = *pSrc;
        if(isalpha(c) || isdigit(c) || c == '-' || c == '.' || c == '~')
        {
            *pDest = c;
            ++pDest;
            ++cbDest;
        }
        else if(c == ' ')
        {
            *pDest = '+';
            ++pDest;
            ++cbDest;
        }
        else
        {
            //检查缓冲区大小是否够用？
            if(cbDest + 3 > cbBufLen - 1)
                break;
            pDest[0] = '%';
            pDest[1] = (c >= 0xA0) ? ((c >> 4) - 10 + baseChar) : ((c >> 4) + '0');
            pDest[2] = ((c & 0xF) >= 0xA)? ((c & 0xF) - 10 + baseChar) : ((c & 0xF) + '0');
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

//解码后是utf-8编码
WCHAR* UrlDecode(LPCSTR szSrc)
{
    if(szSrc == NULL)
        return FALSE;
	LPCSTR trimSrc = trim(szSrc);

    size_t len_ascii = strlen(trimSrc);
    if(len_ascii == 0)
    {
        return NULL;
    }
    CHAR *pUTF8 = new CHAR[len_ascii + 1];
	ZeroMemory(pUTF8,len_ascii + 1);
    if(pUTF8 == NULL)
        return NULL;

    int cbDest = 0; //累加
    LPSTR pDest = pUTF8;
	LPCSTR pSrc = trimSrc;
	size_t i = 0;
	while (i < len_ascii)
	{
		if(i <= (len_ascii - 3) && pSrc[i] == '%' && isHexNum(pSrc[i+1]) && isHexNum(pSrc[i+2])){
			sscanf_s(pSrc+i+1,"%x",pDest++);
			i+=3;
		}else{
			*(pDest++)=pSrc[i++];
		}
	}

    int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, NULL, 0);
    LPWSTR pUnicode = new WCHAR[cchWideChar + 1];
	ZeroMemory(pUnicode,(cchWideChar+ 1) * sizeof(WCHAR));

	MultiByteToWideChar(CP_UTF8,0,pUTF8,-1,pUnicode,cchWideChar);
	free(pUTF8);
	delete [] trimSrc;
    return pUnicode;
}

TCHAR* GetDocId(TCHAR *url)
{
	int length = _tcslen(url);
	TCHAR *src_url = new TCHAR[length + 1];
	ZeroMemory(src_url,length + 1);
	_tcscpy_s(src_url,length + 1,url);
	TCHAR *doc_start =_tcsstr(src_url,_T("doc_id="));
	TCHAR *doc_end = _tcsstr(src_url,_T("&"));
	int docid_length = doc_end - doc_start - 7;
	TCHAR *doc_id = new TCHAR[docid_length + 1];
	ZeroMemory(doc_id,docid_length + 1);
	_tcsnccpy_s(doc_id,docid_length + 1,doc_start + 7,docid_length);
	delete [] src_url;
	return doc_id;
}

LPCSTR trim(const char *str)
{
	std::string* s = new std::string(str);
	s->erase(std::remove_if(s->begin(),s->end(),isspace),s->end());
	LPSTR res = new CHAR[s->length() + 1];
	strcpy_s(res,s->length() + 1,s->c_str());
	delete s;
	return res;
}

bool isHexNum(char c)
{
	return (c>='0' && c<='9')||(c>='A' && c<='F')||(c>='a' && c<='f');
}

char* FileLengthToString(FILE_LENGTH file_size)
{
	std::stringstream a;
    a << file_size;
	int length = a.str().length() + 1;
	char* bytes = new char[length];
	std::string s = a.str();
	const char* size = s.c_str();
    strcpy_s(bytes,length,size);
	return bytes;
}

FILE_LENGTH StringToFileLength(char *bytes)
{
	std::stringstream strValue;
	strValue << bytes;
	FILE_LENGTH value;
	strValue >> value;
	return value;
}