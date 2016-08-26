// DecryptMail.cpp : Defines the entry point for the console application.
//
#include"opensslutil.h"
#include "smime.h"
#include "strutil.h"

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#include <tchar.h>
extern "C"
{
#include <openssl/applink.c>
}

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
void MyHandleError(char *s);
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
void writeFile(const char* sFileName, BYTE* data, DWORD nSize,bool append = false);

int main() {
	//HCERTSTORE hSystemStore;
	//HCERTSTORE hMemoryStore;
	//HCERTSTORE hDuplicateStore;

	//PCCERT_CONTEXT  pDesiredCert = NULL;

	//PCCERT_CONTEXT  pCertContext;
	//HANDLE  hStoreFileHandle;
	//LPWSTR  pszFileName = L"D:\\TestStor.sto";
	//SECURITY_ATTRIBUTES sa;

	//if (hMemoryStore = CertOpenStore(CERT_STORE_PROV_MEMORY, 0, NULL, 0, NULL)) {
	//	printf("Opened a memory store. \n");
	//} else {
	//	MyHandleError("Error opening a memory store.");
	//}

	//if (hSystemStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, NULL, CERT_SYSTEM_STORE_CURRENT_USER, _T("MY"))) {
	//	printf("Opened the MY system store. \n");
	//} else {
	//	MyHandleError("Could not open the MY system store.");
	//}

	//if (hDuplicateStore = CertDuplicateStore(hSystemStore)) {
	//	printf("The MY store is duplicated.\n");
	//} else {
	//	printf("Duplication of the MY store failed.\n.");
	//}

	//if (hDuplicateStore) {
	//	CertCloseStore(hDuplicateStore, CERT_CLOSE_STORE_CHECK_FLAG);
	//}

	//if (pDesiredCert = CertFindCertificateInStore(hSystemStore, MY_ENCODING_TYPE, 0, CERT_FIND_SUBJECT_STR, _T("fanjsh@inspur.com"), NULL)) {
	//	printf("The desired certificate was found. \n");
	//} else {
	//	MyHandleError("Could not find the desired certificate.");
	//}
	////HCRYPTPROV hProv = NULL;
	////DWORD dwKeyType = 0;
	////BOOL bFreeKeyProv = FALSE;
	////BOOL acquire_res = CryptAcquireCertificatePrivateKey(pDesiredCert, 0, 0, &hProv, &dwKeyType, &bFreeKeyProv);
	////if (bFreeKeyProv) {
	////	printf("aaaaaaaaaaaaa");
	////	//CryptReleaseContext(hProv,0);
	////}
	////printf("%d \n",dwKeyType);

	////if (!acquire_res)
	////	return -1;

	////HCRYPTKEY hKey = NULL;
	////acquire_res = CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hKey);
	////if (!acquire_res) {
	////	printf("%d", GetLastError());
	////	return -1;
	////}

	////BYTE* pbData = NULL;
	////DWORD dwDataLen = 0;
	////acquire_res = CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, 0, NULL, &dwDataLen);
	////printf("dwDataLen is %d\n", dwDataLen);
	////if (!acquire_res) {
	////	printf("%d\n", GetLastError());

	////}

	////if (dwDataLen == 0) {
	////	return -1;
	////}
	////pbData = new BYTE[dwDataLen + 5];
	////memset(pbData, 0, dwDataLen + 5);
	////CryptExportKey(hKey, NULL, PRIVATEKEYBLOB, 0, pbData, &dwDataLen);

	////writeFile("D:\\aaa.pfx", pbData, dwDataLen);

	////printf("%d \n", acquire_res);
	////------------------------------------------------------------------ 
	////  Create a new certificate from the encoded part of
	////  an available certificate.

	//if (pCertContext = CertCreateCertificateContext(
	//	MY_ENCODING_TYPE,            // Encoding type
	//	pDesiredCert->pbCertEncoded,   // Encoded data from
	//								   // the certificate retrieved
	//	pDesiredCert->cbCertEncoded))  // Length of the encoded data
	//{
	//	printf("A new certificate has been created.\n");
	//} else {
	//	MyHandleError("A new certificate could not be created.");
	//}

	////-------------------------------------------------------------------
	//// Add the certificate from the My store to the new memory store.

	//if (CertAddCertificateContextToStore(
	//	hMemoryStore,                // Store handle
	//	pDesiredCert,                // Pointer to a certificate
	//	CERT_STORE_ADD_USE_EXISTING,
	//	NULL)) {
	//	printf("Certificate added to the memory store. \n");
	//} else {
	//	MyHandleError("Could not add the certificate "
	//		"to the memory store.");
	//}

	//CRYPT_DATA_BLOB pfx_data;
	////ZeroMemory(&pfx_data, sizeof(pfx_data));
	//pfx_data.pbData = new BYTE[4096];
	//bool res = PFXExportCertStoreEx(hMemoryStore, &pfx_data, _T(""), NULL, EXPORT_PRIVATE_KEYS);

	//printf("%d %d \n", pfx_data.cbData, res);

	////char *cert_all = new char[pfx_data.cbData * 2 + 1];
	////ZeroMemory(cert_all, pfx_data.cbData * 2 + 1);
	////ByteToHexStr(pfx_data.pbData, cert_all, pfx_data.cbData);
	//writeFile("D:\\aaa.pfx", pfx_data.pbData, pfx_data.cbData);
	////printf(cert_all);
	////-------------------------------------------------------------------
	//// Find a different certificate in the My store, and add to it a link
	//// to the memory store.

	////-------------------------------------------------------------------
	//// Find the certificate context just added to the memory store.

	////if (pDesiredCert)
	////	CertFreeCertificateContext(pDesiredCert);

	////if (pDesiredCert = CertFindCertificateInStore(
	////	hSystemStore,
	////	MY_ENCODING_TYPE,            // Use X509_ASN_ENCODING
	////	0,                           // no dwFlags needed 
	////	CERT_FIND_SUBJECT_STR,       // Find a certificate with a
	////								 // subject that matches the 
	////								 // string in the next parameter
	////	L"Insert_cert_subject_name2",// The Unicode string to be found
	////								 // in a certificate's subject
	////	NULL))                       // NULL for the first call to the
	////								 // function 
	////								 // In all subsequent
	////								 // calls, it is the last pointer
	////								 // returned by the function
	////{
	////	printf("The second certificate was found. \n");
	////} else {
	////	MyHandleError("Could not find the second certificate.");
	////}
	//////-------------------------------------------------------------------
	////// Add a link to the second certificate from the My store to 
	////// the new memory store.

	////if (CertAddCertificateLinkToStore(
	////	hMemoryStore,           // Store handle
	////	pDesiredCert,           // Pointer to a certificate
	////	CERT_STORE_ADD_USE_EXISTING,
	////	NULL)) {
	////	printf("Certificate link added to the memory store. \n");
	////} else {
	////	MyHandleError("Could not add the certificate link to the "
	////		"memory store.");
	////}
	////--------------------------------------------------------------------
	//// Find the first certificate in the memory store.

	//if (pDesiredCert)
	//	CertFreeCertificateContext(pDesiredCert);

	////if (pDesiredCert = CertFindCertificateInStore(
	////	hMemoryStore,
	////	MY_ENCODING_TYPE,            // Use X509_ASN_ENCODING
	////	0,                           // No dwFlags needed 
	////	CERT_FIND_SUBJECT_STR,       // Find a certificate with a
	////								 // subject that matches the string
	////								 // in the next parameter
	////	L"fanjsh@inspur.com",// The Unicode string to be found
	////								 // in a certificate's subject
	////	NULL))                       // NULL for the first call to the
	////								 // function
	////								 // In all subsequent
	////								 // calls, it is the last pointer
	////								 // returned by the function
	////{
	////	printf("The desired certificate was found in the "
	////		"memory store. \n");
	////} else {
	////	printf("Certificate not in the memory store.\n");
	////}
	////-------------------------------------------------------------------
	//// Find the certificate link in the memory store.

	////if (pDesiredCert)
	////	CertFreeCertificateContext(pDesiredCert);

	////if (pDesiredCert = CertFindCertificateInStore(
	////	hMemoryStore,
	////	MY_ENCODING_TYPE,            // Use X509_ASN_ENCODING
	////	0,                           // No dwFlags needed 
	////	CERT_FIND_SUBJECT_STR,       // Find a certificate with a
	////								 // subject that matches the 
	////								 // string in the next parameter
	////	L"fanjsh@inspur.com",// The Unicode string to be found
	////								 // in a certificate's subject
	////	NULL))                       // NULL for the first call to the
	////								 // function
	////								 // In all subsequent
	////								 // calls, it is the last pointer
	////								 // returned by the function
	////{
	////	printf("The certificate link was found in the memory store. \n");
	////} else {
	////	printf("The certificate link was not in the memory store.\n");
	////}
	////-------------------------------------------------------------------
	//// Create a file in which to save the new store and certificate.

	//// Create a DACL for the file.
	////sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	////sa.bInheritHandle = FALSE;

	////// Call the function to set the DACL. The DACL
	////// is set in the SECURITY_ATTRIBUTES 
	////// lpSecurityDescriptor member.
	////// if !CreateMyDACL(&sa), call MyHandleError("CreateMyDACL failed.");

	////if (hStoreFileHandle = CreateFile(
	////	pszFileName,        // File path
	////	GENERIC_WRITE,      // Access mode
	////	0,                  // Share mode
	////	&sa,                // Security 
	////	CREATE_ALWAYS,      // How to create the file
	////	FILE_ATTRIBUTE_NORMAL,
	////	// File attributes
	////	NULL))              // Template
	////{
	////	printf("Created a new file on disk. \n");
	////} else {
	////	MyHandleError("Could not create a file on disk.");
	////}
	//////-------------------------------------------------------------------
	////// hStoreFileHandle is the output file handle.
	////// Save the memory store and its certificate to the output file.

	////if (CertSaveStore(
	////	hMemoryStore,        // Store handle
	////	0,                   // Encoding type not needed here
	////	CERT_STORE_SAVE_AS_STORE,
	////	CERT_STORE_SAVE_TO_FILE,
	////	hStoreFileHandle,    // This is the handle of an open disk file
	////	0))                  // dwFlags
	////						 // No flags needed here
	////{
	////	printf("Saved the memory store to disk. \n");
	////} else {
	////	MyHandleError("Could not save the memory store to disk.");
	////}
	////-------------------------------------------------------------------
	//// Close the stores and the file. Reopen the file store, 
	//// and check its contents.

	//if (hMemoryStore)
	//	CertCloseStore(
	//		hMemoryStore,
	//		CERT_CLOSE_STORE_CHECK_FLAG);

	//if (hSystemStore)
	//	CertCloseStore(
	//		hSystemStore,
	//		CERT_CLOSE_STORE_CHECK_FLAG);

	////if (hStoreFileHandle)
	////	CloseHandle(hStoreFileHandle);

	//printf("All of the stores and files are closed. \n");

	////-------------------------------------------------------------------
	////  Reopen the file store.

	////if (hMemoryStore = CertOpenStore(
	////	CERT_STORE_PROV_FILENAME,    // Store provider type
	////	MY_ENCODING_TYPE,            // If needed, use the usual
	////								 // encoding types
	////	NULL,                        // Use the default HCRYPTPROV
	////	0,                           // Accept the default for all
	////								 // dwFlags
	////	L"TestStor.sto"))           // The name of an existing file
	////								// as a Unicode string
	////{
	////	printf("The file store has been reopened. \n");
	////} else {
	////	printf("The file store could not be reopened. \n");
	////}
	//////-------------------------------------------------------------------
	////// Find the certificate link in the reopened file store.

	////if (pDesiredCert)
	////	CertFreeCertificateContext(pDesiredCert);

	////if (pDesiredCert = CertFindCertificateInStore(
	////	hMemoryStore,
	////	MY_ENCODING_TYPE,            // Use X509_ASN_ENCODING
	////	0,                           // No dwFlags needed 
	////	CERT_FIND_SUBJECT_STR,       // Find a certificate with a
	////								 // subject that matches the string
	////								 // in the next parameter
	////	L"fanjsh@inspur.com",// The Unicode string to be found
	////								 // in a certificate's subject
	////	NULL))                       // NULL for the first call to the
	////								 // function
	////								 // In all subsequent
	////								 // calls, it is the last pointer
	////								 // returned by the function
	////{
	////	printf("The certificate link was found in the file store. \n");
	////} else {
	////	printf("The certificate link was not in the file store.\n");
	////}
	//////-------------------------------------------------------------------
	////// Clean up memory and end.

	//if (pDesiredCert)
	//	CertFreeCertificateContext(pDesiredCert);
	//if (hMemoryStore)
	//	CertCloseStore(
	//		hMemoryStore,
	//		CERT_CLOSE_STORE_CHECK_FLAG);
	//if (hSystemStore)
	//	CertCloseStore(
	//		hSystemStore,
	//		CERT_CLOSE_STORE_CHECK_FLAG);
	////if (hStoreFileHandle)
	////	CloseHandle(hStoreFileHandle);
	//printf("All of the stores and files are closed. \n");

	//int length = pDesiredCert->pCertInfo->SubjectPublicKeyInfo.PublicKey.cbData;
	//BYTE* pubKey = pDesiredCert->pCertInfo->SubjectPublicKeyInfo.PublicKey.pbData;
	//DWORD unUsed = pDesiredCert->pCertInfo->SubjectPublicKeyInfo.PublicKey.cUnusedBits;


	//char *dest = new char[length * 2 + 1];

	//ZeroMemory(dest, length * 2 + 1);
	//ByteToHexStr(pubKey, dest, length);

	//printf(dest);
	//for (int i = 0; i < length; i++) {
	//	printf("%x ",pubKey);
	//	++pubKey;
	//}
//30 82 01 0a 02 82 01 01 00 b9 32 b0 74 32 b1 72 7c 28 45 76 c2 19 19 65 82 65 71 a0 c8 66 e2 01 17 34 87 c2 09 45 4c 00 80 65 b7 6d 15 ff cc 1a ea 22 73 60 5e 67 c6 f6 00 d8 2e 45 05 26 44 f2 4a 40 ac 99 3d ff df 14 87 e5 69 fb f2 3a 58 9d 6b ec d6 f4 22 72 89 ba 1b 1f 02 85 38 77 07 78 c0 0b c4 2c f4 e9 00 4c 9e 65 a4 51 58 ff 6c 48 70 d8 a6 9d 88 b6 18 0b 60 99 31 d4 58 fc c7 2d e8 19 d3 23 bc 9e 3d c3 a0 d5 0c 72 dc 5d 89 bc d7 fb de bb 39 d9 7b 88 8b c2 df 75 80 9d ed a7 cc 53 f9 91 d8 bb c9 4e 83 0d 2c a1 e7 ae ad 56 ca 95 58 58 e3 da e1 9c ba 33 cb 70 3e 01 7b 20 0c e3 95 49 0e f1 4c e7 5a 8d 10 57 4b 15 a3 04 8e e7 fa 70 55 79 31 81 ac b3 16 14 b2 e7 23 23 a8 41 de db fc 24 74 62 93 ab b0 ba a0 c3 ad 37 bd 23 99 78 28 2a 2f c8 5e bb 26 87 0e a9 27 15 75 09 d6 48 7d 39 2f c7 e9 02 03 01 00 01
	//if (pCertContext = CertCreateCertificateContext(
	//	MY_ENCODING_TYPE,            // Encoding type
	//	pDesiredCert->pbCertEncoded,   // Encoded data from
	//								   // the certificate retrieved
	//	pDesiredCert->cbCertEncoded))  // Length of the encoded data
	//{
	//	printf("A new certificate has been created.\n");
	//} else {
	//	MyHandleError("A new certificate could not be created.");
	//}




	//char *s = "-----BEGIN CERTIFICATE-----\n\
	//MIIHOzCCBiOgAwIBAgIKGIFLYAABAAIHhjANBgkqhkiG9w0BAQUFADBbMRMwEQYK\
	//CZImiZPyLGQBGRYDY29tMRgwFgYKCZImiZPyLGQBGRYIbGFuZ2NoYW8xFDASBgoJ\
	//kiaJk/IsZAEZFgRob21lMRQwEgYDVQQDEwtMYW5nQ2hhby1DQTAeFw0xNTA3MDIw\
	//MDI0MDdaFw0yMDA2MzAwMDI0MDdaMIGSMRMwEQYKCZImiZPyLGQBGRYDY29tMRgw\
	//FgYKCZImiZPyLGQBGRYIbGFuZ2NoYW8xFDASBgoJkiaJk/IsZAEZFgRob21lMRUw\
	//EwYDVQQLDAzmtarmva7ova/ku7YxEjAQBgNVBAMMCeaoiue7p+ehlTEgMB4GCSqG\
	//SIb3DQEJARYRZmFuanNoQGluc3B1ci5jb20wggEiMA0GCSqGSIb3DQEBAQUAA4IB\
	//DwAwggEKAoIBAQC5MrB0MrFyfChFdsIZGWWCZXGgyGbiARc0h8IJRUwAgGW3bRX/\
	//zBrqInNgXmfG9gDYLkUFJkTySkCsmT3/3xSH5Wn78jpYnWvs1vQicom6Gx8ChTh3\
	//B3jAC8Qs9OkATJ5lpFFY/2xIcNimnYi2GAtgmTHUWPzHLegZ0yO8nj3DoNUMctxd\
	//ibzX+967Odl7iIvC33WAne2nzFP5kdi7yU6DDSyh566tVsqVWFjj2uGcujPLcD4B\
	//eyAM45VJDvFM51qNEFdLFaMEjuf6cFV5MYGssxYUsucjI6hB3tv8JHRik6uwuqDD\
	//rTe9I5l4KCovyF67JocOqScVdQnWSH05L8fpAgMBAAGjggPHMIIDwzALBgNVHQ8E\
	//BAMCBaAwPgYJKwYBBAGCNxUHBDEwLwYnKwYBBAGCNxUIgvKpH4SB13qGqZE9hoD3\
	//FYPYj1yBSoairR2G15QsAgFkAgEYMEQGCSqGSIb3DQEJDwQ3MDUwDgYIKoZIhvcN\
	//AwICAgCAMA4GCCqGSIb3DQMEAgIAgDAHBgUrDgMCBzAKBggqhkiG9w0DBzAdBgNV\
	//HQ4EFgQU9eCIDRGugGPt74ONqvOEZ2AfuBEwHwYDVR0jBBgwFoAUxm7DohNuGr0G\
	//Ol63LzTY4NOc+yMwggEVBgNVHR8EggEMMIIBCDCCAQSgggEAoIH9hoG8bGRhcDov\
	//Ly9DTj1MYW5nQ2hhby1DQSxDTj1qdGpuY2EwMSxDTj1DRFAsQ049UHVibGljJTIw\
	//S2V5JTIwU2VydmljZXMsQ049U2VydmljZXMsQ049Q29uZmlndXJhdGlvbixEQz1o\
	//b21lLERDPWxhbmdjaGFvLERDPWNvbT9jZXJ0aWZpY2F0ZVJldm9jYXRpb25MaXN0\
	//P2Jhc2U/b2JqZWN0Q2xhc3M9Y1JMRGlzdHJpYnV0aW9uUG9pbnSGPGh0dHA6Ly9q\
	//dGpuY2EwMS5ob21lLmxhbmdjaGFvLmNvbS9DZXJ0RW5yb2xsL0xhbmdDaGFvLUNB\
	//LmNybDCCATAGCCsGAQUFBwEBBIIBIjCCAR4wgbMGCCsGAQUFBzAChoGmbGRhcDov\
	//Ly9DTj1MYW5nQ2hhby1DQSxDTj1BSUEsQ049UHVibGljJTIwS2V5JTIwU2Vydmlj\
	//ZXMsQ049U2VydmljZXMsQ049Q29uZmlndXJhdGlvbixEQz1ob21lLERDPWxhbmdj\
	//aGFvLERDPWNvbT9jQUNlcnRpZmljYXRlP2Jhc2U/b2JqZWN0Q2xhc3M9Y2VydGlm\
	//aWNhdGlvbkF1dGhvcml0eTBmBggrBgEFBQcwAoZaaHR0cDovL2p0am5jYTAxLmhv\
	//bWUubGFuZ2NoYW8uY29tL0NlcnRFbnJvbGwvanRqbmNhMDEuaG9tZS5sYW5nY2hh\
	//by5jb21fTGFuZ0NoYW8tQ0EoMSkuY3J0MCkGA1UdJQQiMCAGCCsGAQUFBwMCBggr\
	//BgEFBQcDBAYKKwYBBAGCNwoDBDA1BgkrBgEEAYI3FQoEKDAmMAoGCCsGAQUFBwMC\
	//MAoGCCsGAQUFBwMEMAwGCisGAQQBgjcKAwQwPwYDVR0RBDgwNqAhBgorBgEEAYI3\
	//FAIDoBMMEWZhbmpzaEBpbnNwdXIuY29tgRFmYW5qc2hAaW5zcHVyLmNvbTANBgkq\
	//hkiG9w0BAQUFAAOCAQEAl9sKELe5pO//6m5N42FwP3VS/HYU2LvOE6eAbVm1JmRK\
	//WVMQF6+yis8SVFjgnxh8HEtRjt56TtWsY6n+6gnar/G6FDGorMQYeFsNcy94RdVX\
	//EJj/wO9cRQfQzUT8c2fQ2typWTmEPnwoCWVX836DBkvKq0cbKtW8ZM01N9Wu9Mdc\
	//yO2n+KUaw4fX/g+MfnsVUs2D/cbhlUWffWDpYHbJyojDkjvSNo2E00H1hlWehREh\
	//vOEpwWPaQb7IG0GqxHnGCNLeM1vIQArzazl7TCgVpmiqaorKcLW9oVJXecc3Xjrz\
	//zQ/2glf/Mtqv4lewILQ/PYqmH73271a2ZPwJQDZNSQ==\
	//\n-----END CERTIFICATE-----";
	//	LOGI("%s", s);
		apps_startup();
		FILE *file = NULL;
		_wfopen_s(&file, L"D:\\Ñ¸À×ÏÂÔØ\\tumblr_o6hdqw0nUO1v8p3mk.mp4",L"rb");
		LOGI("%d", (file == NULL));
		BIO* bio = BIO_new_fp(file, BIO_CLOSE);
		LOGI("\n%d", (bio == NULL));
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