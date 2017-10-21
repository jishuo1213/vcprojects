#include "stdafx.h"
#include "FileName.h"
#include <algorithm>
#include "utils.h"
#include <iostream>
#include <stdlib.h>
#include <Shlwapi.h>

FileName::FileName(const wchar_t  *file_name, bool isFolder) {
	this->fileName = file_name;
	this->isFolder = isFolder;
}


FileName::~FileName() {
	//delete[] this->fileName;
	//delete[] this->suffix;
	std::cout << "~~~~~~~~~~" << std::endl;
}

bool FileName::operator<(const FileName &rhs) {

	int res = StrCmpLogicalW(this->fileName, rhs.fileName);
	return res < 0;
}

void FileName::printFileName() {
	std::cout << WcharToChar_New(this->fileName) << std::endl;
}
