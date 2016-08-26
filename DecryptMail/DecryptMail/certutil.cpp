#include "certutil.h"
#include <wincrypt.h>
//
//
//void GetUserCert(const char* user, PCCERT_CONTEXT pcert) {
//}

void GetUserCert(const char * user_id, PCCERT_CONTEXT hCert) {
	HCERTSTORE hCertStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, NULL, CERT_SYSTEM_STORE_CURRENT_USER, L"MY");
	PCCERT_CONTEXT user_cert = NULL;
	while (user_cert = CertEnumCertificatesInStore(hCertStore, hCert)) {
		
	}
}
