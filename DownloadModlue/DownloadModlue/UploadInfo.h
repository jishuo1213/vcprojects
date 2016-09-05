#pragma once
#include <tchar.h>
#include <json\json.h>
#include <Shlwapi.h>
#include "strutil.h"

class UploadInfo
{
public:
	UploadInfo(const TCHAR *file_path, const char*json_param) {
		Json::Value upload_info;
		Json::Reader reader;
		this->file_path = file_path;
		reader.parse(json_param, upload_info);
		LPCSTR temp_disk_id = upload_info["diskId"].asCString();
		LPCSTR temp_folder_id = upload_info["folderId"].asCString();
		LPCSTR temp_user_id = upload_info["userId"].asCString();
		LPCSTR temp_uuid = upload_info["uuid"].asCString();

		disk_id = CopyCstring(temp_disk_id);
		folder_id = CopyCstring(temp_folder_id);
		user_id = CopyCstring(temp_user_id);
		uuid = CopyCstring(temp_uuid);

		TCHAR *extension = PathFindExtensionW(file_path);
		TCHAR *fileName = PathFindFileNameW(file_path);
		char* file_extension = WcharToUTF8_New(extension);
		mime_type = GetMimeType(file_extension);
		file_name = WcharToUTF8_New(fileName);
		delete[]file_extension;
		_tfopen_s(&upload_fd, file_path, _T("rb"));
		if (upload_fd) {
			upfile_size = _filelengthi64(_fileno(upload_fd));
		}
	}

	const char* GetDiskId() {
		return disk_id;
	}

	const char* GetFloderId() {
		return folder_id;
	}

	const char* GetUserId() {
		return user_id;
	}

	const char* GetFileName() {
		return file_name;
	}

	const char* GetFileMimeType() {
		return mime_type;
	}

	const TCHAR* GetFilePath() {
		return file_path;
	}

	FILE* GetUpFileFd() {
		return upload_fd;
	}

	FILE_LENGTH GetFileSize() {
		return upfile_size;
	}

	void SetUploadCurl(CURL *curl) {
		this->curl = curl;
	}

	CURL*  GetUploadCurl() {
		return curl;
	}

	const char* GetUUid() {
		return uuid;
	}

	~UploadInfo(void) {
		delete[] file_name;
		delete[] user_id;
		delete[] disk_id;
		delete[] folder_id;
		delete[] uuid;
	}


private:
	const TCHAR* file_path;
	const char *disk_id;
	const char *folder_id;
	const char *user_id;
	const char* file_name;
	const char* mime_type;
	const char* uuid;
	FILE* upload_fd;
	FILE_LENGTH upfile_size;
	CURL *curl;
};