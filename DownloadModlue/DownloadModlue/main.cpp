#include "stdafx.h"

const int RECEIVE_NO_DATA_TIME_OUT = 8;
static FILE *fp = NULL;
static int receive_no_data_times = 0;
int time_out_times = 0;
bool is_need_reload = false;

static size_t write_callback(void *ptr,size_t size,size_t nmemb,void *stream)
{

	int len = size * nmemb;
	int written = len;
	DownloadInfo *downloadinfo = (DownloadInfo*)stream;
	if(!fp)
	{
		TCHAR* filePath = downloadinfo->get_temp_file_path();
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
		downloadInfo->SetFileName(fileName);

	}else if(strstr(str,"Content-Length")){
		int length = strlen(str) - 15;
		char *filesize = new char[length];
		strcpy_s(filesize,length,str+16);
		FILE_LENGTH size = StringToFileLength(filesize);
		delete [] filesize;
		downloadInfo->SetFileSize(size);
	}
	return rmemb * size;
}

static int xferinfo(void *p,curl_off_t dltotal,curl_off_t dlnow,curl_off_t ultotal,curl_off_t ulnow)
{
	DownloadInfo *myp = (DownloadInfo *)p;
	double curtime = 0;
	if((myp->GetFileSize()) <=0)
		return 0;
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

		if(speed == 0){
			receive_no_data_times++;
				if(receive_no_data_times >RECEIVE_NO_DATA_TIME_OUT){
					printf("receive_no_data_times %d \n",receive_no_data_times);
					if(!IsNetWorkWell()){ //网络不通
						printf("net not well  \n");
						time_out_times++;
						if(time_out_times > 2){//联系两个周期没有接受到数据
							return 1; //退出
						}else{
							receive_no_data_times = 0;//在尝试一次
						}
					} else {//网络连通 但是连续多次没接受到数据
						printf("net is well need reload  \n");
						is_need_reload = true;
						return 1;
					}
				}
			} else {
				receive_no_data_times = 0;
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
			char *bytes = NULL;
			bytes = FileLengthToString(downloadInfo->GetDownloadedSize());
			char *header = new char[12+strlen(bytes)+2];
			strcpy_s(header,13,"Range:bytes=");
			strcat_s(header,strlen(header)+strlen(bytes)+1,bytes);
			strcat_s(header,strlen(header)+2,"-");
			chunk=curl_slist_append(chunk,header);
			delete[] bytes;
			curl_easy_setopt(curl,CURLOPT_HTTPHEADER,chunk);
			strcat_s(url,strlen(url)+17,"&resume_flg=true");
		}

		curl_easy_setopt(curl,CURLOPT_URL,url);
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,	write_callback);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,downloadInfo);
		curl_easy_setopt(curl,CURLOPT_HEADERDATA,downloadInfo);
		curl_easy_setopt(curl,CURLOPT_HEADER,0);
		curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION,header_callback);
		curl_easy_setopt(curl,CURLOPT_NOBODY,FALSE);
		curl_easy_setopt(curl,CURLOPT_NOPROGRESS,FALSE);
		curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT,10);//连接超时10S
		curl_easy_setopt(curl,CURLOPT_XFERINFOFUNCTION,xferinfo);
		curl_easy_setopt(curl,CURLOPT_XFERINFODATA,downloadInfo);
		
		res = curl_easy_perform(curl);
		if(res == CURLE_OK) {
			curl_easy_cleanup(curl);
			if(chunk)
				curl_slist_free_all(chunk);
			return 1;
		}
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
	sockaddr_in addr4;
	UINT ip = inet_pton(AF_INET,"115.239.211.112",&addr4.sin_addr);
	ULONG hopCount = 0;
	ULONG RTT = 0;

	if (GetRTTAndHopCount(ip, &hopCount, 30, &RTT) == TRUE) {
		printf("Hops: %ld\n", hopCount);
		printf("RTT: %ld\n", RTT);
	}else {
		printf("Error: %ld\n", GetLastError());
	}
	/*if(argc == 3){
		TCHAR * inUrl = argv[1];
		TCHAR* filepath = argv[2];
		TCHAR *doc_id = GetDocId(inUrl);
		int  iLength = WideCharToMultiByte(CP_ACP, 0, inUrl, -1, NULL, 0, NULL, NULL);
		char *url = new char[iLength];
		WideCharToMultiByte(CP_ACP, 0, inUrl, -1, url, iLength, NULL, NULL);   
		DownloadInfo *downloadInfo = new DownloadInfo(url,filepath,doc_id);
		while(1){
		if(DownLoad(url,downloadInfo)) {
			if(fp)
				fclose(fp);
			if(downloadInfo->RenameFileAfterDownload() == 0) {
				fprintf(stdout, "%s\n",BuildSuccessResponseJson().c_str());
				break;
			} else {
				fprintf(stdout, "%s\n",BuildRenameFailedJson().c_str());
				break;
			}
		} else {
			if(is_need_reload){
				delete downloadInfo;
				downloadInfo = new DownloadInfo(url,filepath,doc_id);
				receive_no_data_times = 0;
				is_need_reload = false;
				time_out_times = 0;
			} else {
				fprintf(stdout, "%s\n",BuildFailedResponseJson().c_str());
				break;
			}
		}
	}
	if(downloadInfo)
		delete downloadInfo;
	}
	return 0;*/
}