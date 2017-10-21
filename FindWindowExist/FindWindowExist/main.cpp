#include<Windows.h>
#include<TlHelp32.h>
#include <tchar.h>
#include <iostream>

//wchar_t *fileType;
wchar_t *fileName;
BOOL isFind = false;
BOOL isOpen = false;

char *WcharToChar(const wchar_t *str, int codePage = CP_UTF8)
{
	int iLength = WideCharToMultiByte(codePage, 0, str, -1, NULL, 0, NULL, NULL);
	char *url = new char[iLength];
	ZeroMemory(url, iLength);
	WideCharToMultiByte(codePage, 0, str, -1, url, iLength, NULL, NULL);
	return url;
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	//wchar_t class_name[500];
	wchar_t title[MAX_PATH];
	//GetClassName(hwnd, class_name, sizeof(class_name));
	GetWindowText(hwnd, title, sizeof(title));
	//	std::cout <<"find:" <<WcharToChar(title,CP_ACP) << std::endl;
		//std::cout << "Argument:"<<WcharToChar(fileName, CP_ACP) << std::endl;
	//	std::cout << wcsstr(title, fileName) << std::endl;
		//std::cout << "====================="<<std::endl;
	wchar_t* this_res = wcsstr(title, L"FindWindowExist");
	if (wcslen(title) > 0 && !this_res) {

		wchar_t* res = wcsstr(title, fileName);

		//	std::cout << (res == title) << std::endl;
			//std::cout << "=====================" << std::endl;
		if (res /*&& (res == title)*/) {
			std::cout << "find:" << WcharToChar(title, CP_ACP) << std::endl;
			std::cout << "Argument:" << WcharToChar(fileName, CP_ACP) << std::endl;
			isFind = true;
			if (!isOpen) {
				isOpen = true;
			}
		}
	}
	return TRUE;
}

void openApp(const TCHAR * arg) {
	SHELLEXECUTEINFO ShExecInfo;
	if (true) {
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = _T("open");
		ShExecInfo.lpFile = arg;
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_MAXIMIZE;
		ShExecInfo.hInstApp = NULL;

		ShellExecuteEx(&ShExecInfo);
	/*	std::cout << ShExecInfo.hInstApp << std::endl;
		std::cout << (ShExecInfo.hProcess == NULL) << std::endl;*/
		auto res = WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		if (res == WAIT_FAILED) {
			std::cout << GetLastError() << std::endl;
			std::cout << res << std::endl;
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3) {
		return 0;
	}
	fileName = argv[1];

	openApp(argv[2]);
	isOpen = true;

	//fileType = argv[2];
	while (true) {
		EnumWindows(EnumWindowsProc, NULL);
	/*	std::cout << isFind << std::endl;
		std::cout << isOpen << std::endl;*/

		if (!isFind && isOpen) {
			char* temp = WcharToChar(fileName);
			std::cout << "{\"name\":" << "\"" << temp << "\"" << "," << "\"status\":" << "\"close\"}" << std::endl;
			delete[] temp;
			break;
		}
		isFind = false;
		//break;
		Sleep(1000);
	}
	return 0;
}