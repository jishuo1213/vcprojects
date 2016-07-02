#include "stdafx.h"

bool IsNetWorkWell()
{
	if(InternetCheckConnection(_T("http://hr.inspur.com/"),FLAG_ICC_FORCE_CONNECTION,0)){
		return true;
	}
	return false;
}

bool CreateMultiplePath(LPCWSTR path)
{
	LPCWSTR prefix = L"\\\\?\\";
	//const TCHAR* . _tcschr(path, _T('\\'));

	int level = 0;
	LPCWSTR loc = path;
	LPCWSTR last = loc + _tcslen(path) - 1;
	while (loc != last && (loc =_tcschr(loc+1, _T('\\')))) {
		int len = loc - path + 1;
		TCHAR * temp = new TCHAR[len +4+1];
		ZeroMemory(temp, len + 5);
		_tcscpy_s(temp,5,prefix);
		TCHAR* part_path = new TCHAR[len + 1];
		ZeroMemory(part_path, len + 1);
		_tcsncpy_s(part_path, len + 1, path, len);
		if (level == 0) {
			if (_taccess_s(part_path, 0) != 0) {
				delete[] temp;
				delete[] part_path;
				return false;
			} else {
				delete[] temp;
				delete[] part_path;
				level++;
				continue;
			}
		} else {
			if (_taccess_s(part_path, 0) != 0) {
				_tcscat_s(temp, len + 4 + 1, part_path);
				if (CreateDirectory(temp, NULL) == 0) {
					delete[] temp;
					delete[] part_path;
					return false;
				} else {
					delete[] temp;
					delete[] part_path;
					level++;
					continue;
				}
			} else {
				delete[] temp;
				delete[] part_path;
				level++;
				continue;
			}
		}
	}
	return true;
}
