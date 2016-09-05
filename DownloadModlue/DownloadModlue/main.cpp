#include "stdafx.h"
#include "upload.h"

static int downloadFile(TCHAR *inUrl, TCHAR *filepath, TCHAR *uuid, TCHAR *file_name);

const int RECEIVE_NO_DATA_TIME_OUT = 9;
static FILE *fp = NULL;
static int receive_no_data_times = 0;
int time_out_times = 0;
bool is_need_reload = false;

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	int len = size * nmemb;
	int written = len;
	DownloadInfo *downloadinfo = (DownloadInfo*)stream;
	if (!fp) {
		TCHAR* filePath = downloadinfo->get_temp_file_path();
		//TCHAR* filePath = _T("D:\\go1.6.windows-amd64.msi");
		if (_taccess(filePath, 0) == -1) {
			_tfopen_s(&fp, filePath, _T("wb"));
		} else {
			_tfopen_s(&fp, filePath, _T("ab"));
		}
	}
	if (fp) {
		fwrite(ptr, size, nmemb, fp);
	}
	return written;
}

static size_t header_callback(void *ptr, size_t size, size_t rmemb, void *stream) {
	DownloadInfo *downloadInfo = (DownloadInfo*)stream;
	char *str = (char*)ptr;
	if (!downloadInfo->GetFileName() && strstr(str, "Content-Disposition")) {
		char *src = strstr(str, "filename=");
		int length = strlen(src) - 8;
		char *urlcode_filename = new char[length];
		ZeroMemory(urlcode_filename, length);
		strcpy_s(urlcode_filename, length, (src + 9));
		WCHAR* fileName = UrlDecode(urlcode_filename);
		delete[] urlcode_filename;
		downloadInfo->SetFileName(fileName);
	} else if (strstr(str, "Content-Length")) {
		int length = strlen(str) - 15;
		char *filesize = new char[length];
		strcpy_s(filesize, length, str + 16);
		FILE_LENGTH size = StringToFileLength(filesize);
		delete[] filesize;
		downloadInfo->SetFileSize(size);
	}
	return rmemb * size;
}

static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
	DownloadInfo *myp = (DownloadInfo *)p;
	double curtime = 0;
	curtime = myp->get_cur_run_time();
	if (myp->GetFileSize() == 0) {
		//if (myp->check_progress_time(curtime)) {

		//	std::cout << BuildProgressResponseJson(myp, speed, myp->GetDownloadedSize(), myp->GetFileSize(), curtime) << std::endl;
		//}
		if (myp->downloadType == 0) {
			return 0;
		}
	}
	if (myp->check_progress_time(curtime)) {
		myp->SetLastRunTime(curtime);
		FILE_LENGTH speed;
		if (myp->IsBreakPointDownload()) {
			static FILE_LENGTH prvious_download = 0;
			speed = dlnow - prvious_download;
			prvious_download = dlnow;
			myp->SetDownloadedSize(speed + myp->GetDownloadedSize());
		} else {
			speed = dlnow - myp->GetDownloadedSize();
			myp->SetDownloadedSize(dlnow);
		}

		if (speed == 0) {
			receive_no_data_times++;
			if (receive_no_data_times > RECEIVE_NO_DATA_TIME_OUT) {
				Log(_T("%s"), _T("超过10次速度为0"));
				if (!CheckNetWorkWell()) { //网络不通
					time_out_times++;
					Log(_T("%s time_out_times:%d"), _T("测试网络，网络未联通"), time_out_times);
					if (time_out_times > 9) {//联系9个周期没有接受到数据
						Log(_T("%s time_out_times:%d"), _T("网络连续10个周期未联通，退出下载"), time_out_times);
						return 1; //退出
					} else {
						receive_no_data_times = 0;//在尝试一次
						Log(_T("%s %d"), _T("在尝试一次"), receive_no_data_times);
					}
				} else {//网络连通 但是连续多次没接受到数据
					Log(_T("%s"), _T("测试网络，联通,需要重新启动下载"));
					is_need_reload = true;
					return 1;
				}
			}
		} else {
			receive_no_data_times = 0;
			time_out_times = 0;
		}
		std::cout << BuildProgressResponseJson(myp, speed, myp->GetDownloadedSize(), myp->GetFileSize(), curtime) << std::endl;
		//fprintf(stdout, BuildProgressResponseJson(speed,dlnow,myp->GetFileSize(),curtime));
	}
	return 0;
}

int DownLoad(char *url, DownloadInfo *downloadInfo) {
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	char* get_url;
	if (curl) {
		downloadInfo->SetCurl(curl);
		struct curl_slist *chunk = NULL;
		int src_url_length = strlen(url);
		if (downloadInfo->check_is_tempfile_exits()) {
			char *bytes = NULL;
			bytes = FileLengthToString(downloadInfo->GetDownloadedSize());
			char *header = new char[12 + strlen(bytes) + 2];
			strcpy_s(header, 13, "Range:bytes=");
			strcat_s(header, strlen(header) + strlen(bytes) + 1, bytes);
			strcat_s(header, strlen(header) + 2, "-");
			chunk = curl_slist_append(chunk, header);
			delete[] bytes;
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
			get_url = new char[src_url_length + 17];
			strcpy_s(get_url, src_url_length + 1, url);
			strcat_s(get_url, src_url_length + 17, "&resume_flg=true");
			Log(_T("开始断点下载 %d"), downloadInfo->GetDownloadedSize());
		} else {
			get_url = new char[src_url_length + 1];
			strcpy_s(get_url, src_url_length + 1, url);
		}

		curl_easy_setopt(curl, CURLOPT_URL, get_url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, downloadInfo);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, downloadInfo);
		curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
		curl_easy_setopt(curl, CURLOPT_NOBODY, FALSE);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);//连接超时10S
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, downloadInfo);
		//curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, xferinfo);
		///* pass the struct pointer into the progress function */
		//curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, downloadInfo);

		res = curl_easy_perform(curl);
		if (res == CURLE_OK) {
			curl_easy_cleanup(curl);
			if (chunk)
				curl_slist_free_all(chunk);
			delete get_url;
			return 1;
		} else {
			TCHAR * error = CharToWchar_New(curl_easy_strerror(res));
			Log(_T("下载出错，error：%s %d"), error, res);
			delete[] error;
			if (res == CURLE_RECV_ERROR)
				is_need_reload = true;
		}
		curl_easy_cleanup(curl);
		if (chunk)
			curl_slist_free_all(chunk);
		delete get_url;
		return 0;
	}
	return 0;
}

int GetDownloadInfo(DownloadInfo *downlaodInfo, char *url) {
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if (curl) {
		downlaodInfo->SetCurl(curl);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, downlaodInfo);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
		curl_easy_setopt(curl, CURLOPT_NOBODY, true);
		res = curl_easy_perform(curl);
		if (res == CURLE_OK) {
			return 1;
		}

		std::cerr << "res" << res << curl_easy_strerror(res);
		return 0;
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[]) {
	if (argc == 1)
		return -1;

	if (_tcscmp(argv[1], _T("/d")) == 0) {
		if (argc >= 5) {
			TCHAR * inUrl = argv[2];
			TCHAR* filepath = argv[3];
			TCHAR* uuid = argv[4];
			TCHAR * file_name = NULL;

			if (argc > 6) {
				if (_tcscmp(argv[5], _T("/name")) == 0) {
					file_name = argv[6];
				} else {
					Log(_T("%s"), _T("参数传递错误，应该是/name"));
					return -1;
				}
			} else if (argc == 6) {
				Log(_T("%s"), _T("参数传递少，/name 后面应该有文件名"));
				return -1;
			}
			return  downloadFile(inUrl, filepath, uuid, file_name);
		}
	} else if (_tcscmp(argv[1], _T("/upload")) == 0) {
		char *uploadInfo = WcharToUTF8_New(argv[3]);
		//UploadInfo *info = new UploadInfo(argv[2], uploadInfo);
		UploadInfo info(argv[2], uploadInfo);
		UploadFile(&info);
		delete[]uploadInfo;
	} else {
		if (argc >= 4) {
			TCHAR * inUrl = argv[1];
			TCHAR* filepath = argv[2];
			TCHAR* uuid = argv[3];
			TCHAR * file_name = NULL;

			if (argc > 5) {
				if (_tcscmp(argv[4], _T("/name")) == 0) {
					file_name = argv[5];
				} else {
					Log(_T("%s"), _T("参数传递错误，应该是/name"));
					return -1;
				}
			} else if (argc == 5) {
				Log(_T("%s"), _T("参数传递少，/name 后面应该有文件名"));
				return -1;
			}
			 return downloadFile(inUrl, filepath, uuid, file_name);
		}
	}
	return 0;
}

int downloadFile(TCHAR *inUrl, TCHAR *filepath, TCHAR *uuid, TCHAR *file_name) {
	int download_result = 0;
	char* url = WcharToChar_New(inUrl);
	TCHAR *doc_id = GetDocId(url);
	int downloadType = GetDownloadType(url);
	char* char_uuid = WcharToChar_New(uuid);
	DownloadInfo *downloadInfo = new DownloadInfo(url, filepath, doc_id);
	downloadInfo->downloadType = downloadType;
	downloadInfo->SetUUid(char_uuid);
	if (_taccess_s(filepath, 0) != 0) { //如果文件夹不存在
		int res = CreateMultiplePath(filepath);
		if (res == 0) {
			Log(_T("%s, %s"), _T("文件夹不存在，创建文件夹失败"), filepath);
			fprintf(stdout, "%s\n", BuildFailedResponseJson(downloadInfo, 4, _T("文件夹路径不存在，而且创建失败")).c_str());
			delete[] url;
			delete[] char_uuid;
			download_result = -1;
			return download_result;
		}
	}
	if (file_name) {
		int length = _tcslen(file_name) + 1;
		TCHAR *fileName = new TCHAR[length];
		_tcscpy_s(fileName, length, file_name);
		downloadInfo->SetFileName(fileName);
	}

	while (1) {
		Log(_T("%s %s"), _T("开始下载"), inUrl);
		if (DownLoad(url, downloadInfo)) {
			if (fp)
				fclose(fp);
			if (downloadInfo->RenameFileAfterDownload() == 0) {
				fprintf(stdout, "%s\n", BuildSuccessResponseJson(downloadInfo).c_str());
				break;
			} else {
				download_result = -1;
				fprintf(stdout, "%s\n", BuildRenameFailedJson(downloadInfo).c_str());
				break;
			}
		} else {
			Log(_T("%s %d"), _T("下载失败，判断是否需要重新下载"), is_need_reload);
			if (is_need_reload) {//重新连接 下载
				delete downloadInfo;
				if (fp) {
					fclose(fp);
					fp = NULL;
				}
				doc_id = GetDocId(url);
				downloadInfo = new DownloadInfo(url, filepath, doc_id);
				downloadInfo->SetUUid(char_uuid);
				downloadInfo->downloadType = downloadType;
				if (file_name) {
					int length = _tcslen(file_name) + 1;
					TCHAR *fileName = new TCHAR[length];
					_tcscpy_s(fileName, length, file_name);
					downloadInfo->SetFileName(fileName);
				}
				receive_no_data_times = 0;
				is_need_reload = false;
				time_out_times = 0;
			} else {
				fprintf(stdout, "%s\n", BuildFailedResponseJson(downloadInfo).c_str());
				download_result = -1;
				break;
			}
		}
	}
	if (downloadInfo)
		delete downloadInfo;
	if (url)
		delete[] url;
	if (char_uuid)
		delete[] char_uuid;
	return download_result;
}