//
// Created by fan on 16-7-12.
//
#include "strutil.h"
#include <string.h>

int strToHex(unsigned char *ch, int length, char *hex) {
	int high, low;
	unsigned int tmp = 0;
	if (ch == NULL || hex == NULL) {
		return -1;
	}

	if (length == 0) {
		return -2;
	}

	while (*ch) {
		tmp = *ch;
		high = tmp >> 4;
		low = tmp & 15;
		*hex++ = valueToHexCh(high); //��д���ֽ�
		*hex++ = valueToHexCh(low); //���д���ֽ�
		ch++;
	}
	*hex = '\0';
	return 0;
}

int hexToStr(char *hex, char *ch) {
	int high, low;
	int tmp = 0;
	if (hex == NULL || ch == NULL) {
		return -1;
	}

	if (strlen(hex) % 2 == 1) {
		return -2;
	}

	while (*hex) {
		high = hexCharToValue(*hex);
		if (high < 0) {
			*ch = '\0';
			return -3;
		}
		hex++; //ָ���ƶ�����һ���ַ���
		low = hexCharToValue(*hex);
		if (low < 0) {
			*ch = '\0';
			return -3;
		}
		tmp = (high << 4) + low;
		*ch++ = (char)tmp;
		hex++;
	}
	*ch = '\0';
	return 0;
}

int hexCharToValue(const char ch) {
	int result = 0;
	//��ȡ16���Ƶĸ��ֽ�λ����
	if ((ch >= '0') && (ch <= '9')) {
		result = (int)(ch - '0');
	}
	else if (ch >= 'a' && ch <= 'z') {
		result = (int)(ch - 'a') + 10;
	}
	else if (ch >= 'A' && ch <= 'Z') {
		result = (int)(ch - 'A') + 10;
	}
	else {
		result = -1;
	}
	return result;
}

char valueToHexCh(const int value) {
	char result = '\0';
	if (value >= 0 && value <= 9) {
		result = (char)(value + 48); //48Ϊascii����ġ�0���ַ�����ֵ
	}
	else if (value >= 10 && value <= 15) {
		result = (char)(value - 10 + 65); //��ȥ10���ҳ�����16���Ƶ�ƫ������65Ϊascii��'A'���ַ�����ֵ
	}
	else {
		;
	}

	return result;
}
