#pragma once
#include <tchar.h>

class FileName
{
public:
	FileName(const wchar_t *file_name);
	~FileName();
	
	bool operator<(const FileName &rhs);

	void printFileName();

private:
	const wchar_t *fileName;
	bool isFolder;
};

