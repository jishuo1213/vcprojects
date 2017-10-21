// HtimeUpdate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zip.h"
#include <Windows.h>
#include <TlHelp32.h>


BOOL KillProcessByName(const TCHAR *lpszProcessName)
{
	unsigned int   pid = -1;
	BOOL    retval = TRUE;

	if (lpszProcessName == NULL)
		return -1;

	DWORD dwRet = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(PROCESSENTRY32);
	int flag = Process32First(hSnapshot, &processInfo);

	// Find the process with name as same as lpszProcessName  
	while (flag != 0) {
		if (_tcscmp(processInfo.szExeFile, lpszProcessName) == 0) {
			// Terminate the process.  
			pid = processInfo.th32ProcessID;
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);

			if (TerminateProcess(hProcess, 0) != TRUE) { // Failed to terminate it.  
				retval = FALSE;
				break;
			}
		}

		flag = Process32Next(hSnapshot, &processInfo);
	} // while (flag != 0)  

	CloseHandle(hSnapshot);

	if (pid == -1)
		return FALSE;

	return retval;
}

//int KillProcess(LPCWSTR pszClassName, LPCWSTR pszWindowTitle) {
//	HANDLE hProcessHandle;
//	ULONG nProcessID;
//	HWND TheWindow;
//	TheWindow = FindWindow(NULL, pszWindowTitle);
//	::GetWindowThreadProcessId(TheWindow, &nProcessID);
//	hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
//	return TerminateProcess(hProcessHandle, 4);
//}

char* WcharToUTF8_New(const WCHAR* str)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	char* dest = new char[len + 1];
	ZeroMemory(dest, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, dest, len, NULL, NULL);
	return dest;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 6) {
		return 0;
	}

	int res = KillProcessByName(L"NW.exe");
	TCHAR* zipPath = argv[1];
	TCHAR *toPath = argv[2];
	TCHAR* programPath = argv[3];
	TCHAR* verisonFilePath = argv[4];
	TCHAR* versionCode = argv[5];

	res = ZlibUnzip2Folder(zipPath, toPath);
	printf("==================%d\n", res);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	if (res == 0) {
		bool res =DeleteFileW(zipPath);
		FILE *versionFile;
		_wfopen_s(&versionFile, verisonFilePath, L"w");
		auto code = WcharToUTF8_New(versionCode);
		fwrite(code, sizeof(char), strlen(code), versionFile);
		//printf(" write num :%d", res);
		fclose(versionFile);
		delete[] code;
	}
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessW(NULL, programPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		//printf("CreateProcess failed (%d).\n", GetLastError());
		return 0;
	}
	return 0;
}

