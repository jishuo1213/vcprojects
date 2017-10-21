// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <curl\curl.h>
#include <io.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <algorithm>
#include <WinInet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "strutil.h"
#include "DownloadInfo.h"
#include "json\json.h"
#include "sysutil.h"
#include "Ping.h"
#include "md5.h"
#include "UploadInfo.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif // DEBUG






// TODO: reference additional headers your program requires here
