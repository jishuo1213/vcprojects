// DecryptMail.cpp : Defines the entry point for the console application.
//
#include"opensslutil.h"
#include "smime.h"
#include "strutil.h"

#include <stdio.h>
//#include <windows.h>
//#include <Wincrypt.h>
#include <tchar.h>
//
//extern "C"
//{
//#include <openssl\applink.c>
//}

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
void MyHandleError(char *s);
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
void writeFile(const char* sFileName, BYTE* data, DWORD nSize,bool append = false);

int main() {
		apps_startup();
		/*FILE *file = NULL;
		_wfopen_s(&file, L"D:\\迅雷下载\\tumblr_o6hdqw0nUO1v8p3mk.mp4",L"rb");
		LOGI("%d", (file == NULL));
		BIO* bio = BIO_new_fp(file, BIO_CLOSE);
		LOGI("\n%d", (bio == NULL));*/
		//char * in = WcharToUTF8_New(w_recip);
		//char * out = WcharToUTF8_New(w_recip);

		//w_decrypt_smime("D:\\邮件\\樊.pfx", "jishuo1213?", "D:\\邮件\\havefile.eml", "D:\\邮件\\w_decrypt2.eml");

		int res = sign("D:\\邮件\\樊.pfx", "jishuo1213?", "D:\\邮件\\srcsss.eml","D:\\邮件\\w_sign.eml");
		printf("%d\n", res);
		//int res = sign("D:\\mail\\1.pfx", "jishuo1213?", "D:\\mail\\mailTest.eml", "D:\\mail\\mailTestsigned.eml");
		//LOGI("res = %d\n", res);
		/*res = decrypt_smime("D:\\Mail\\1.pfx", "jishuo1213?", "D:\\Mail\\havefile.eml", "D:\\Mail\\decrypt.eml");
		LOGI("res = %d\n", res);
		res = encrypt_mime(s, "D:\\Mail\\mailTestsigned.eml", "D:\\Mail\\mailTestEncrypt.eml");
		LOGI("res = %d\n", res);*/
	return 0;
}


void MyHandleError(char *s) {
	fprintf(stderr, "An error occurred in running the program. \n");
	fprintf(stderr, "%s\n", s);
	fprintf(stderr, "Error number %x.\n", GetLastError());
	fprintf(stderr, "Program terminating. \n");
	exit(1);
} // end MyHandleError


void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen) {
	short i;
	//printf("%d \n", sourceLen);
	//for (DWORD ii = 0; ii < sourceLen; ii++) {
	//	printf("%02x ", source[ii]);
	//	if ((ii + 1) % 16 == 0) printf("\n");
	//}
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i++) {
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	}
	return;
}

void writeFile(const char* sFileName, BYTE* data, DWORD nSize, bool append) {
	FILE* fp = NULL;
	if (append) {
		fopen_s(&fp, sFileName, "ab");
	} else {
		fopen_s(&fp, sFileName, "wb");
	}
	if (fp == NULL) {
		printf("Can not open output file '%s'! \n", sFileName);
		return;
	}

	if (fwrite(data, 1, nSize, fp) != nSize) {
		fclose(fp);
		printf("Write to file '%s' failed! \n", sFileName);
		return;
	}

	fclose(fp);
	printf("Write %d bytes to file '%s'! \n", nSize, sFileName);
}