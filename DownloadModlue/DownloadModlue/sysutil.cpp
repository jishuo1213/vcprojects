#include "stdafx.h"

bool IsNetWorkWell()
{
	if(InternetCheckConnection(_T("http://hr.inspur.com/"),FLAG_ICC_FORCE_CONNECTION,0)){
		return true;
	}
	return false;
}