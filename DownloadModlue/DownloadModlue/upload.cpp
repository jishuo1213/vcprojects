#include"stdafx.h"

static FILE_LENGTH uploaded_size = 0;
static time_t last_progress_time = 0;
static time_t start_time = time(0);

static size_t read_callback(void *ptr, size_t size, size_t mmemb, void *stream) {
	curl_off_t nread;
	time_t now_time = time(0);
	tm timeinfo;
	localtime_s(&timeinfo, &now_time);
	time_t run_time = now_time - start_time;
	UploadInfo *uploadInfo = (UploadInfo*)stream;
	size_t retcode = fread(ptr, size, mmemb, uploadInfo->GetUpFileFd());
	nread = (curl_off_t)retcode;
	uploaded_size += retcode;
	if ((now_time - last_progress_time) >= 1) {
		last_progress_time = now_time;
		std::cout << BuildUploadProgressResJson(uploadInfo, uploaded_size, run_time) << std::endl;
	}
	return retcode;
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	std::cout << BuildUploadSuccessResJson((UploadInfo*)stream, (char*)ptr) << std::endl;
	return size * nmemb;
}

int UploadFile(UploadInfo *uploadInfo) {

	if (!uploadInfo->GetUpFileFd()) {
		std::cout << BuildUploadFailedResJson(uploadInfo, -2, "can not open file") << std::endl;
		return -2;
	}

	CURL *curl;
	//FILE_LENGTH upfile_size = 0;
	double speed_upload = 0, total_time = 0;
	//FILE *fd = NULL;
	CURLM *multi_handle = NULL;
	int still_running;
	curl_global_init(CURL_GLOBAL_ALL);
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;

	//_tfopen_s(&fd, uploadInfo->GetFilePath(), _T("rb"));


	static const char buf[] = "Expect:";
	//upfile_size = _filelengthi64(_fileno(fd));
	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, "disk_id",
		CURLFORM_COPYCONTENTS, uploadInfo->GetDiskId(),
		CURLFORM_END);

	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, "folder_id",
		CURLFORM_COPYCONTENTS, uploadInfo->GetFloderId(),
		CURLFORM_END);


	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, "user_id",
		CURLFORM_COPYCONTENTS, uploadInfo->GetUserId(),
		CURLFORM_END);

	curl_formadd(&formpost, &lastptr,
		CURLFORM_COPYNAME, "uploadfile",
		CURLFORM_FILENAME, uploadInfo->GetFileName(),
		CURLFORM_CONTENTTYPE, uploadInfo->GetFileMimeType(),
		CURLFORM_CONTENTLEN, (curl_off_t)uploadInfo->GetFileSize(),
		CURLFORM_STREAM, uploadInfo,
		CURLFORM_END);

	curl = curl_easy_init();

	multi_handle = curl_multi_init();

	headerlist = curl_slist_append(headerlist, buf);

	if (curl && multi_handle) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://dcp.inspur.com:81/wpserver/upload-api");
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);


		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, uploadInfo);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);


		curl_multi_add_handle(multi_handle, curl);

		CURLMcode performRec = curl_multi_perform(multi_handle, &still_running);
		//printf("performRec = %d\n", performRec);
		uploaded_size = 0;
		tm timeinfo;
		localtime_s(&timeinfo, &start_time);
		last_progress_time = 0;
		do {
			struct timeval timeout;
			int rc;
			CURLMcode mc;

			fd_set fdread;
			fd_set fdwrite;
			fd_set fdexcep;
			int maxfd = -1;

			long curl_timeo = -1;

			FD_ZERO(&fdread);
			FD_ZERO(&fdwrite);
			FD_ZERO(&fdexcep);

			timeout.tv_sec = 1;
			timeout.tv_usec = 0;

			curl_multi_timeout(multi_handle, &curl_timeo);
			if (curl_timeo >= 0) {
				timeout.tv_sec = curl_timeo / 1000;
				if (timeout.tv_sec > 1)
					timeout.tv_sec = 1;
				else
					timeout.tv_usec = (curl_timeo % 1000) * 1000;
			}

			mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

			//printf("mc = %d\n",mc);
			if (mc != CURLM_OK) {
				//printf("mc != CURLM_OK\n");
				return -1;
				break;
			}

			if (maxfd == -1) {
				Sleep(100);
				rc = 0;
			} else {
				rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
			}
			//printf("rc = %d\n", rc);
			switch (rc) {
			case -1:
				//printf("rc == -1\n");
				break;
			case 0:
			default:
				CURLMcode performRec2 = curl_multi_perform(multi_handle, &still_running);
				//printf("performRec2 = %d\n", performRec2);
				break;
			}
			//printf("still_running = %d\n", still_running);
		} while (still_running);

		//if (!still_running) {
		//	std::cout << BuildUploadFailedResJson(uploadInfo, -1, "net work error") << std::endl;
		//}

		fclose(uploadInfo->GetUpFileFd());
		curl_multi_cleanup(multi_handle);
		curl_multi_cleanup(curl);
		curl_formfree(formpost);
		curl_slist_free_all(headerlist);
	}
	//printf("return\n");
	return 0;
}