#include "stdafx.h"
#include "FileName.h"
#include <algorithm>
#include "utils.h"
#include <iostream>

FileName::FileName(const wchar_t  *file_name) {
	this->fileName = file_name;
	this->isFolder = false;
}


FileName::~FileName() {
}

bool FileName::operator<(const FileName &rhs) {
	if (this->isFolder && !rhs.isFolder) {
		return true;
	}

	if (!this->isFolder && rhs.isFolder) {
		return false;
	}

	int left_len = _tcslen(this->fileName);
	int right_len = _tcslen(rhs.fileName);
	if (right_len > left_len) {
		const wchar_t* index;
		if ((index = _tcsstr(rhs.fileName, this->fileName)) > 0) {
			if (index == rhs.fileName) {
				return iswspace(*(rhs.fileName + left_len));
			}
		}
	} else if (left_len > right_len) {
		const wchar_t* index;
		if ((index = _tcsstr(this->fileName, rhs.fileName)) > 0) {
			if (index == this->fileName) {
				return iswspace(*(this->fileName + right_len));
			}
		}
	}
	int count = std::min<int>(left_len, right_len);
	for (int i = 0; i < count; i++) {
		wchar_t lch = *(this->fileName + i);
		wchar_t rch = *(rhs.fileName + i);
		if (lch == rch) {
			if (i == count - 1) {

			}
			continue;
		} else {
			WCharType ltype = getWcharType(lch);
			WCharType rtype = getWcharType(rch);
			if (ltype == rtype) {
				if (ltype != unicode) {
					return lch < rch;
				} else {
					return compareUnicodeChar(lch, rch);
				}
			} else {
				if (std::abs(ltype - rtype) == 1) {
					bool res;
					switch (ltype) {
					case space:
						res = true;
						break;
					case w_space:
						res = false;
						break;
					case symbol:
					case num:
					case letter:
					{
						char temp1 = 0;
						temp1 = rch - 65248;
						if (temp1 == lch) {
							if (i < count - 1) {
								continue;
							} else if (i == count - 1) {
								return true;
							}
						}
						res = lch < temp1;
						break;
					}
					case w_symbol:
					case w_num:
					case w_letter:
					{
						char temp2 = 0;
						temp2 = lch - 65248;
						res = temp2 < rch;
						break;
					}
					default:
						res = false;
						break;
					}
				} else {
					return ltype < rtype;
				}
			}
		}
	}
	return false;
}

void FileName::printFileName() {
	std::cout << WcharToChar_New(this->fileName) << std::endl;
}
