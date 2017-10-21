#pragma once
#include <Windows.h>

enum WCharType
{
	space,
	w_space,
	temp1,
	symbol,
	w_symbol,
	temp2,
	num,
	w_num,
	temp3,
	letter,
	w_letter,
	temp4,
	unicode
};

WCharType getWcharType(const wchar_t &ch);

char* WcharToChar_New(const wchar_t  *str);

bool compareUnicodeChar(wchar_t lhs, wchar_t rhs);