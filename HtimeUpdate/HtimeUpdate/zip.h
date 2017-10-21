#pragma once
#include<Windows.h>
#include <zip.h>	

int Unzip2Folder(BSTR lpZipFile, BSTR lpFolder);

int ZlibUnzip2Folder(LPCWSTR lpSrcFile, LPCWSTR destFolder);

zip_t *windows_open(const wchar_t *name, int flags);