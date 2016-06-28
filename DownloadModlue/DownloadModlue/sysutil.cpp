#include "stdafx.h"

bool IsNetWorkWell()
{
	if(system("ping 10.110.9.197 "))//1>nul 2>nul
	{
		std::cout <<"ping success "<<std::endl;
		return true;
	}else{

		return false;	
	}
}