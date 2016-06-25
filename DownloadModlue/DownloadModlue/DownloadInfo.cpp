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

void DownloadInfo::init_temp_file_name(TCHAR *filepath,TCHAR *docid)
{
	TCHAR *fix = _T(".htdownload");
	temp_file_path = new TCHAR[_tcslen(filepath)+_tcslen(docid)+_tcslen(fix)+ 1];
	_tcscpy_s(temp_file_path,_tcslen(filepath)+1,filepath);
	_tcscat_s(temp_file_path,_tcslen(temp_file_path)+_tcslen(docid)+1,docid);
	_tcscat_s(temp_file_path,_tcslen(temp_file_path)+_tcslen(fix)+1,fix);
	std::wcout << temp_file_path <<std::endl;
}

DownloadInfo::~DownloadInfo(void)
{
	delete [] file_name;
	delete [] temp_file_path;
	delete [] doc_id;
}
