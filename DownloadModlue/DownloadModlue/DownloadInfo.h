#pragma once
#include <tchar.h>
#include <curl\curl.h>

const int MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL = 1;

class DownloadInfo
{
public:
	DownloadInfo(CURL *curl,char *download_url,TCHAR *filepath,TCHAR *docId):
		current_curl(curl),url(download_url),file_path(filepath),doc_id(docId)
	{
		file_name = NULL;
		file_size = 0;
		last_run_time = 0;
		downloaded_size = 0;
		init_temp_file_name(filepath,docId);
	}

	DownloadInfo(char *download_url,TCHAR *filepath,TCHAR *docid):url(download_url),file_path(filepath),doc_id(docid)
	{
		current_curl = NULL;
		file_name = NULL;
		file_size = 0;
		last_run_time = 0;
		downloaded_size = 0;
		init_temp_file_name(filepath,docid);
	}

	DownloadInfo()
	{
		current_curl = NULL;
		url = NULL;
		file_path = NULL;
		file_name = NULL;
		temp_file_path = NULL;
		doc_id = NULL;
		file_size = 0;
		last_run_time = 0;
		downloaded_size = 0;
	}

	void SetFileName(const TCHAR *filename)
	{
		this->file_name = filename;
		TCHAR *fix = _T(".htdownload");
		_tcscat_s(file_path,_tcsclen(file_path)+_tcsclen(file_name)+1,file_name);
		//temp_file_path = new TCHAR[_tcsclen(file_path)+_tcsclen(fix)+_tcsclen(doc_id)+1];
		//_tcscpy_s(temp_file_path,_tcsclen(file_path)+1,file_path);
		//_tcscat_s(temp_file_path,_tcsclen(temp_file_path)+_tcsclen(doc_id)+1,doc_id);
		//_tcscat_s(temp_file_path,_tcsclen(temp_file_path)+_tcsclen(fix)+1,fix);
		//std::wcout << temp_file_path << std::endl;
	}

	void SetCurl(CURL *curl)
	{
		this->current_curl = curl;
	}

	long GetDownloadedSize()
	{
		return downloaded_size;
	}

	bool check_progress_time(double &curtime);
	double get_cur_run_time();
	bool check_is_tempfile_exits();

	void SetFileSize(long size)
	{
		file_size = size;
	}

	void SetLastRunTime(double time)
	{
		this->last_run_time = time;
	}

	TCHAR* get_file_path()
	{
		 return file_path;
	}

	TCHAR* get_temp_file_path()
	{
		 return temp_file_path;
	}

	long GetFileSize()
	{
		return file_size;
	}

	CURL* GetCurl()
	{
		return current_curl;
	}

	~DownloadInfo(void);

private:
	CURL *current_curl;
	char *url;
	TCHAR *file_path;
	TCHAR *temp_file_path;
	TCHAR *doc_id;
	const TCHAR *file_name;
	long file_size;
	long downloaded_size;
	double last_run_time;

	void init_temp_file_name(TCHAR *filepath,TCHAR *docid);
};

