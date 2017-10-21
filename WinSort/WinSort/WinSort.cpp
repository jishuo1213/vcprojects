// WinSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include<cctype>
//#include <cstring>
#include <algorithm>
#include "FileName.h"
#include <vector>
#include <fstream>
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include "utils.h"


wchar_t* CharToWchar_New(const char *str) {
	int nLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	if (nLen == 0) {
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen + 1];
	ZeroMemory(pResult, nLen + 1);
	MultiByteToWideChar(CP_ACP, 0, str, -1, pResult, nLen);
	return pResult;
}

bool compareFileName(FileName *left, FileName *right) {
	return *left < *right;
}

int main() {
	//wchar_t ext[_MAX_EXT] = { 0 };
	//_wsplitpath_s(L"aaa", NULL, 0, NULL, 0, NULL, 0, ext, _MAX_EXT);
	//std::wcout << _tcslen(ext) << "====" << ext << wcscmp(L"aaab", L"aaa") << std::endl;

	//wchar_t* name = L"!！,，?？中文主ねぬもははは  　";
	////char *c_name = "!！,，?？中文主ねぬもははは  ";
	//int len = _tcsclen(name);
	////int len = strlen(c_name);
	//for (int i = 0; i < len; i++) {
	//	printf("%d,\t0x%4x\n", *(name + i),*(name + i));
	//}


	char line[1024] = { 0 };
	std::ifstream in("D:\\WorkSpace\\Python\\FirstPython\\filenames");
	std::vector<FileName*> fileList = {};

	while (in.getline(line, sizeof(line))) {
		printf("%s\n", line);
		wchar_t* filename = CharToWchar_New(line);
		FileName* file = new FileName(filename,false);
		fileList.push_back(file);
	}
	in.close();
	printf("%d\n", fileList.size());

	std::sort(fileList.begin(), fileList.end(),compareFileName);
	printf("after sort================= %d\n", fileList.size());

	int i = 0;
	for (FileName *file : fileList) {
		i++;
		(*file).printFileName();
	}
	printf("%d\n", i);
	return 0;
}

