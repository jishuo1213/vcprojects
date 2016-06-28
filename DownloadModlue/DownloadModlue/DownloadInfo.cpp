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
		if(fp) {
			downloaded_size = _filelengthi64(_fileno(fp));
			is_break_point_download = true;
			fclose(fp);
			return true;
		} else {
			DeleteFile(temp_file_path);
			is_break_point_download = false;
		}
		return false;
	} else {
		downloaded_size = 0;
		is_break_point_download = false;
		return false;
	}
}

void DownloadInfo::init_temp_file_name(TCHAR *filepath,TCHAR *docid)
{
	TCHAR *fix = _T(".htdownload");
	int length = _tcslen(filepath)+_tcslen(docid)+_tcslen(fix)+ 1;
	temp_file_path = new TCHAR[length];
	ZeroMemory(temp_file_path,length);
	_tcscpy_s(temp_file_path,_tcslen(filepath)+1,filepath);
	_tcscat_s(temp_file_path,_tcslen(temp_file_path)+_tcslen(docid)+1,docid);
	_tcscat_s(temp_file_path,_tcslen(temp_file_path)+_tcslen(fix)+1,fix);
}

int DownloadInfo::RenameFileAfterDownload()
{
	//int moveres;
	//TCHAR fix[5];
	//TCHAR *same_file_path = new TCHAR[_tcslen(file_path)+4+1];
	//_tcscpy_s(same_file_path,_tcslen(file_path) + 1,file_path);
	//int i = 1;
	//while(_taccess_s(same_file_path,0) == 0){
	//	_tcscpy_s(fix,2,_T("("));
	//	TCHAR num[3];
	//	_itow_s(i,num,3,10);
	//	_tcscat_s(fix,4,num);
	//	_tcscat_s(fix,5,_T(")"));
	//	_tcscat_s(same_file_path,_tcslen(file_path) + 5,fix);
	//	i++;
	//}
	//if(moveres = MoveFile(temp_file_path,same_file_path)){
	//	std::wcout << "rename success"<<same_file_path<<std::endl;
	//	return true;
	//}
	//return false;
	int moveres;
	if(_taccess_s(file_path,0) == 0){
		if(!DeleteFile(file_path)){
			return 2;
		}
	}
	if(moveres = MoveFile(temp_file_path,file_path)){
		std::wcout << L"rename success"<<file_path<<std::endl;
		return 0;
	} else {
		return 1;
	}
}

DownloadInfo::~DownloadInfo(void)
{
	if(file_name)
		delete [] file_name;
	_tprintf(_T("%s \n"),temp_file_path);
	if(temp_file_path)
		delete [] temp_file_path;
	if(doc_id)
		delete [] doc_id;
}