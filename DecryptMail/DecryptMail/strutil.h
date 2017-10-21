//
// Created by fan on 16-7-12.
//

#ifndef DECRYPTMAIL_STRUTIL_H
#define DECRYPTMAIL_STRUTIL_H

#include <stdio.h>

#define LOGI(...) \
  ((void)printf( __VA_ARGS__))

int strToHex(unsigned char *ch, int length, char *hex);

int hexToStr(char *hex, char *ch);

int hexCharToValue(const char ch);

char valueToHexCh(const int value);

wchar_t* CharToWchar_New(const char *str);

char* WcharToUTF8_New(const wchar_t* str);

#endif //DECRYPTMAIL_STRUTIL_H
