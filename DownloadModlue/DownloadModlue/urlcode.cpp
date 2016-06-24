#include "stdafx.h"

LPCSTR trim(const char *str);
bool isHexNum(char c);
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