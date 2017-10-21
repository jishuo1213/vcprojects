#include "stdafx.h"
#include <ShlDisp.h>
#include <zip.h>

zip_t *windows_open(const wchar_t *name, int flags) {
	zip_source_t *src;
	zip_t *za;
	zip_error_t error;

	zip_error_init(&error);
	/* create source from buffer */
	if ((src = zip_source_win32w_create(name, 0, -1, &error)) == NULL) {
		fprintf(stderr, "can't create source: %s\n", zip_error_strerror(&error));
		zip_error_fini(&error);
		return NULL;
	}

	/* open zip archive from source */
	if ((za = zip_open_from_source(src, flags, &error)) == NULL) {
		fprintf(stderr, "can't open zip from source: %s\n", zip_error_strerror(&error));
		zip_source_free(src);
		zip_error_fini(&error);
		return NULL;
	}
	zip_error_fini(&error);

	return za;
}

int Unzip2Folder(BSTR lpZipFile, BSTR lpFolder) {
	IShellDispatch *pISD;
	Folder *pZippedFile = 0L;
	Folder *pDestination = 0L;

	long FilesCount = 0;
	IDispatch *pItem = 0L;
	FolderItems *pFilesInside = 0L;

	VARIANT Options, OutFolder, InZipFile, Item;
	CoInitialize(NULL);
	__try {
		if (CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_IShellDispatch, (void **)&pISD) != S_OK)
			return 8;
		InZipFile.vt = VT_BSTR;
		InZipFile.bstrVal = lpZipFile;
		pISD->NameSpace(InZipFile, &pZippedFile);
		if (!pZippedFile) {
			pISD->Release();
			return 2;
		}

		OutFolder.vt = VT_BSTR;
		OutFolder.bstrVal = lpFolder;
		pISD->NameSpace(OutFolder, &pDestination);
		if (!pDestination) {
			pZippedFile->Release();
			pISD->Release();
			return 3;
		}
		pZippedFile->Items(&pFilesInside);
		if (!pFilesInside) {
			pDestination->Release();
			pZippedFile->Release();
			pISD->Release();
			return 4;
		}

		pFilesInside->get_Count(&FilesCount);
		if (FilesCount < 1) {
			pFilesInside->Release();
			pDestination->Release();
			pZippedFile->Release();
			pISD->Release();
			return 6;
		}

		pFilesInside->QueryInterface(IID_IDispatch, (void **)&pItem);

		Item.vt = VT_DISPATCH;
		Item.pdispVal = pItem;

		Options.vt = VT_I4;
		Options.lVal = 1024 | 512 | 16 | 4;

		bool retval = pDestination->CopyHere(Item, Options) == S_OK;

		pItem->Release(); pItem = 0L;
		pFilesInside->Release(); pFilesInside = 0L;
		pDestination->Release(); pDestination = 0L;
		pZippedFile->Release(); pZippedFile = 0L;
		pISD->Release(); pISD = 0L;

		return retval;
	} __finally {
		CoUninitialize();
	}
}

//void print0x(const char * src)
//{
//	const char *temp = src;
//	int i = 0;
//	while (*temp) {
//		unsigned char c = *temp;
//		printf("%x", c);
//		i++;
//		if (i % 2 == 0) {
//			printf(" ");
//		}
//		temp++;
//	}
//}

WCHAR* CharToWchar_New(const char *str) {
	const char* temp = str;
	//print0x(str);
	int nLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	if (nLen == 0) {
		printf("====%d first failed %s\n", nLen, str);
		return NULL;
	}
	//printf("====%d %s\n", nLen, str);
	//WCHAR *str1 = new WCHAR[sizeof(WCHAR) * n];
	wchar_t* pResult = new wchar_t[nLen];
	ZeroMemory(pResult, nLen);
	int res = MultiByteToWideChar(CP_UTF8, 0, str, -1, pResult, nLen);
	if (res == 0) {
		printf("====%d second failed %s\n", nLen, str);
	}

	return pResult;
}

static const int BUF_SIZE = 4 * 1024;

int ZlibUnzip2Folder(LPCWSTR lpSrcFile, LPCWSTR destFolder) {
	zip *z = NULL;
	z = windows_open(lpSrcFile, 0);
	if (z == NULL) {
		printf("cant open zip file \n");
		return 2;
	}
	int count = zip_get_num_entries(z, 0);
	struct zip_stat sb;
	char buf[BUF_SIZE];
	for (int i = 0; i < count; i++) {
		if (zip_stat_index(z, i, 0, (zip_stat_t*)&sb) == 0) {
			int len = strlen(sb.name);

			printf("%s\n", sb.name);
			WCHAR* w_name = CharToWchar_New(sb.name);
			int name_len = wcslen(w_name);
			int folder_len = wcslen(destFolder);
			WCHAR * w_path = new WCHAR[name_len + folder_len + 2];
			ZeroMemory(w_path, name_len + folder_len + 1);
			wcscpy_s(w_path, folder_len + 1, destFolder);
			wcscat_s(w_path,  folder_len + 2, L"/");
			wcscat_s(w_path, name_len + folder_len + 2, w_name);
			delete[]w_name;
			if (sb.name[len - 1] == '/') {
				//printf("%s",w_path)
				bool res =  CreateDirectory(w_path, NULL);
				if (!res) {
					printf("CreateDirectory failed (%d)\n", GetLastError());
				}
				delete[]w_path;
			} else {
				struct zip_file * zf = zip_fopen_index(z, i, 0);
				if (!zf) {
					return -1;
				}
				FILE * file = NULL;
				_wfopen_s(&file, w_path, L"wb");
				printf("open file failed (%d)\n", GetLastError());
				if (file == NULL) {
					zip_fclose(zf);
					printf("namelen=%d\n", name_len);
					printf("file == null %s\n", sb.name);
					delete[] w_path;
					return -1;
				}
				long sum = 0;
				while (sum < sb.size) {
					len = zip_fread(zf, buf, BUF_SIZE);
					if (len < 0) {
						delete[] w_path;
						return -1;
					}
					fwrite(buf, 1, len, file);
					sum += len;
				}
				delete[] w_path;
				fclose(file);
				zip_fclose(zf);
			}
		} else {
			printf("File[%s] Line[%d]\n", __FILE__, __LINE__);
		}
	}
	if (zip_close(z) == -1) {
		return -1;
	}
	return 0;
}
