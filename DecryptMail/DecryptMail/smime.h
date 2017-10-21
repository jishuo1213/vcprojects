//
// Created by fan on 16-7-12.
//

#ifndef DECRYPTMAIL_SMIME_H
#define DECRYPTMAIL_SMIME_H

int decrypt_smime(const char *recipfile,const char *pass, const char *infile, const char *outfile);

int sign(const char *recipfile,const char *pass, const char *infile, const char *outfile);

int encrypt_mime(const char *recipfile,const char *infile, const char *outfile);

void init();

void clean();

int verify_mime(const char *infile, char *outfile, const char *CAfile);

int decrypt_cms(const char *recipfile, const char *pass, const char *infile, const char *outfile);

int parse_cert_file(const char *infile, const char *password, const char *outfile);

int w_parse_cert_file(const wchar_t *infile, const char *password, const wchar_t *outfile);

int w_decrypt_smime(const char *recipfile, const char *pass, const char *infile, const char *outfile);

int w_sign(const char *signerfile, const char *pass, const char *infile, const char *outfile);

int w_decrypt_cms(const char *recipfile, const char *pass, const char *infile, const char *outfile);

int w_encrypt_mime(const char *recipfile, const char *infile, const char *outfile);

int w_verify_mime(const char *infile, char *outfile, const char *CAfile);

#endif //DECRYPTMAIL_SMIME_H
