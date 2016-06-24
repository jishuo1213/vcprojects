#include "stdafx.h"


bool DownloadInfo::check_progress_time(double &curtime)
{
	return  (curtime -last_run_time) >= ::MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL;
}

double DownloadInfo::get_cur_run_time()
{
	double res = 0;
	curl_easy_getinfo(current_curl,CURLINFO_TOTAL_TIME,&res);
	return res;
}

bool DownloadInfo::check_is_tempfile_exits()
{
	FILE* fp = NULL;
	if(_taccess(temp_file_path,0) == 0){
		_tfopen_s(&fp,temp_file_path,_T("rb"));	
		if(fp){
			downloaded_size = _filelength(_fileno(fp));
			fclose(fp);
			return true;
		}
		return false;
	}else{
		downloaded_size = 0;
		return false;
	}
}

DownloadInfo::~DownloadInfo(void)
{
	delete [] file_name;
	delete [] temp_file_path;
}
