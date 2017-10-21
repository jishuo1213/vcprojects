#include "stdafx.h"
#include "utils.h"
#include <cctype>


WCharType getWcharType(const wchar_t & ch) {
	if (iswalnum(ch)) {
		if (ch > 0xFF00) {
			return w_num;
		}
		return num;
	} else if (iswalpha(ch)) {
		if (ch > 0xFF00) {
			return w_letter;
		}
		return letter;
	} else if (iswspace(ch)) {
		if (ch == 0x3000) {
			return w_space;
		}
		return space;
	} else if (iswpunct(ch)) {
		if (ch > 0xFF00) {
			return w_symbol;
		}
		return symbol;
	} else {
		return unicode;
	}
}

 char* WcharToChar_New(const wchar_t  *str) {
	int  iLength = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *url = new char[iLength + 1];
	ZeroMemory(url, iLength + 1);
	WideCharToMultiByte(CP_ACP, 0, str, -1, url, iLength, NULL, NULL);
	return url;
}

bool compareUnicodeChar(wchar_t lhs, wchar_t rhs) {
	wchar_t* l_str = new wchar_t[2];
	wchar_t* r_str = new wchar_t[2];
	ZeroMemory(l_str, 2);
	ZeroMemory(r_str, 2);
	l_str[0] = lhs;
	r_str[0] = rhs;
	char *l_muti_char = WcharToChar_New(l_str);
	char *r_muti_char = WcharToChar_New(r_str);
	int res = strcmp(l_muti_char, r_muti_char);
	delete[] l_str;
	delete[] r_str;
	delete[] l_muti_char;
	delete[] r_muti_char;
	return res;
}

