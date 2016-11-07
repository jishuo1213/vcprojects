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


int main() {
	//wchar_t* name = L"!！,，?？中文主ねぬもははは  　";
	////char *c_name = "!！,，?？中文主ねぬもははは  ";
	//int len = _tcsclen(name);
	////int len = strlen(c_name);
	//for (int i = 0; i < len; i++) {
	//	printf("%d,\t0x%4x\n", *(name + i),*(name + i));
	//}
	char line[1024] = { 0 };
	std::ifstream in("D:\\WorkSpace\\Python\\FirstPython\\filenames");
	std::vector<FileName> fileList = {};

	while (in.getline(line, sizeof(line))) {
		printf("%s\n", line);
		wchar_t* filename = CharToWchar_New(line);
		FileName file(filename);
		fileList.push_back(file);
	}
	in.close();
	printf("%d\n", fileList.size());

	std::sort(fileList.begin(), fileList.end());
	printf("after sort================= %d\n", fileList.size());

	int i = 0;
	for (FileName &file : fileList) {
		i++;
		file.printFileName();
	}
	printf("%d\n", i);
	return 0;
}

