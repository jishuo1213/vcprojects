#include "stdafx.h"

static FILE *fp = NULL;

static size_t write_callback(void *ptr,size_t size,size_t nmemb,void *stream)
{

	int len = size * nmemb;
	int written = len;
	DownloadInfo *downloadinfo = (DownloadInfo*)stream;
	if(!fp)
	{
		TCHAR* filePath = downloadinfo->get_temp_file_path();
		/*std::wcout.imbue(std::locale("CHS"));
		std::wcout << filePath <<res <<std::endl;*/
		if(_taccess(filePath,0) == -1){
			 _tfopen_s(&fp,filePath,_T("wb"));	
		}else{
			_tfopen_s(&fp,filePath,_T("ab"));
		}
	}
	if(fp){
		fwrite(ptr,size,nmemb,fp);
	}
	return written;
}

static size_t header_callback(void *ptr,size_t size,size_t rmemb,void *stream)
{
	DownloadInfo *downloadInfo = (DownloadInfo*)stream;
	char *str = (char*)ptr;
	if(strstr(str,"Content-Disposition")){
		char *src = strstr(str,"filename=");
		int length = strlen(src) - 8;
		char *urlcode_filename = new char[length];
		ZeroMemory(urlcode_filename,length);

		strcpy_s(urlcode_filename,length,(src + 9));
		WCHAR* fileName = UrlDecode(urlcode_filename);
		delete [] urlcode_filename;
		_tprintf(_T("%s \n"),fileName);
		downloadInfo->SetFileName(fileName);

		//std::wcsout.imbue(std::locale("CHS"));
		//std::wcout << downloadInfo->get_file_path() << std::endl;
		//_ftprintf(stdout,_T("%s \n"),downloadInfo->get_file_path());
		//std::cout << _tcsclen(downloadInfo->get_file_path())<<std::endl;
	}else if(strstr(str,"Content-Length")){
		int length = strlen(str) - 15;
		char *filesize = new char[length];
		strcpy_s(filesize,length,str+16);
		FILE_LENGTH size = StringToFileLength(filesize);
		delete [] filesize;
		downloadInfo->SetFileSize(size);
		_tprintf(_T("file size =======%d\n"),size);
	}
	return rmemb * size;
}

static int xferinfo(void *p,curl_off_t dltotal,curl_off_t dlnow,curl_off_t ultotal,curl_off_t ulnow)
{
	DownloadInfo *myp = (DownloadInfo *)p;
	double curtime = 0;
	curtime = myp->get_cur_run_time();
	if(myp->check_progress_time(curtime)){
		myp->SetLastRunTime(curtime);
		FILE_LENGTH speed;
		if(myp->IsBreakPointDownload()) {
			static FILE_LENGTH prvious_download = 0;
			speed = dlnow - prvious_download;
			prvious_download = dlnow;
			myp->SetDownloadedSize(speed+myp->GetDownloadedSize());
		} else {
			speed = dlnow - myp->GetDownloadedSize();
			myp->SetDownloadedSize(dlnow);
		}
		std::cout <<BuildProgressResponseJson(speed,myp->GetDownloadedSize(),myp->GetFileSize(),curtime)<<std::endl;
		//fprintf(stdout, BuildProgressResponseJson(speed,dlnow,myp->GetFileSize(),curtime));
	}
	return 0;
}

int DownLoad(char *url,DownloadInfo *downloadInfo)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();;
	if(curl)
	{
		downloadInfo->SetCurl(curl);
		struct curl_slist *chunk = NULL;
		if(downloadInfo->check_is_tempfile_exits()){
			char bytes[30];
			ZeroMemory(bytes,30);
			FileLengthToString(downloadInfo->GetDownloadedSize(),bytes);
			char *header = new char[12+strlen(bytes)+2];
			strcpy_s(header,13,"Range:bytes=");
			strcat_s(header,strlen(header)+strlen(bytes)+1,bytes);
			strcat_s(header,strlen(header)+2,"-");
			chunk=curl_slist_append(chunk,header);
			curl_easy_setopt(curl,CURLOPT_HTTPHEADER,chunk);
			strcat_s(url,strlen(url)+17,"&resume_flg=true");
		}

		curl_easy_setopt(curl,CURLOPT_URL,url);
		curl_easy_setopt(curl,CURLOPT_VERBOSE,1L);
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,	write_callback);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,downloadInfo);
		curl_easy_setopt(curl,CURLOPT_HEADERDATA,downloadInfo);
		curl_easy_setopt(curl,CURLOPT_HEADER,0);
		curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION,header_callback);
		curl_easy_setopt(curl,CURLOPT_NOBODY,FALSE);
		curl_easy_setopt(curl,CURLOPT_NOPROGRESS,FALSE);
		curl_easy_setopt(curl,CURLOPT_XFERINFOFUNCTION,xferinfo);
		curl_easy_setopt(curl,CURLOPT_XFERINFODATA,downloadInfo);
		
		res = curl_easy_perform(curl);
		if(res == CURLE_OK){
			curl_easy_cleanup(curl);
			if(chunk)
				curl_slist_free_all(chunk);
			return 1;
		}
		 fprintf(stdout, "%s\n", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		if(chunk)
			curl_slist_free_all(chunk);
		return 0;
	}
	return 0;
}

int GetDownloadInfo(DownloadInfo *downlaodInfo,char *url)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl){
		downlaodInfo->SetCurl(curl);
		curl_easy_setopt(curl,CURLOPT_URL,url);
		curl_easy_setopt(curl,CURLOPT_VERBOSE,0L);
		curl_easy_setopt(curl,CURLOPT_HEADERDATA,downlaodInfo);
		curl_easy_setopt(curl,CURLOPT_HEADER,1);
		curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION,header_callback);
		curl_easy_setopt(curl,CURLOPT_NOBODY,true);
		res = curl_easy_perform(curl);
		if(res == CURLE_OK){
			return 1;
		}

		std::cerr<<"res"<<res<<curl_easy_strerror(res);
		return 0;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc == 3){
		TCHAR * inUrl = argv[1];
		TCHAR* filepath = argv[2];
		TCHAR *doc_id = GetDocId(inUrl);
		int  iLength = WideCharToMultiByte(CP_ACP, 0, inUrl, -1, NULL, 0, NULL, NULL);
		char *url = new char[iLength];
		WideCharToMultiByte(CP_ACP, 0, inUrl, -1, url, iLength, NULL, NULL);   
		DownloadInfo downloadInfo(url,filepath,doc_id);
		//if(GetDownloadInfo(&downloadInfo,url)){
		//	_ftprintf(stdout,_T("GetDownloadInfo Success\n"));
		//}else{
		//	_ftprintf(stdout,_T("GetDownloadInfo fail\n"));
		//	return 0;
		//}

		if(DownLoad(url,&downloadInfo)){
			_ftprintf(stdout,_T("Download OK \n"));
			std::wcout<<downloadInfo.get_temp_file_path()<<std::endl;
			std::wcout<<downloadInfo.get_file_path()<<std::endl;
			
			std::cout <<_taccess(downloadInfo.get_temp_file_path(),0)<<std::endl;
			int moveres = 0;
			if(fp)
				fclose(fp);
			if(downloadInfo.RenameFileAfterDownload()){
				std::cout << "rename success"<<std::endl;
			}else{
				std::cout <<"rename failed"<<std::endl;
			}
		} else {
			_ftprintf(stdout,_T("error \n"));
		}
		return 0;
	}
	return 0;
}