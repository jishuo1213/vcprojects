#include "mime.h"
#include <mimetic\mimetic.h>
#include <fstream>
#include <json\json.h>
#include <tchar.h>
#include <Windows.h>

static WCHAR* CharToWchar_New(const char *str);

void GenerateMultiAlternative(const char *content, const char *outFileName) {
	mimetic::MultipartAlternative ma;
	ma.header().push_back(mimetic::Field("Mime-Version", "1.0"));

	mimetic::MimeEntity *pme = new mimetic::MimeEntity;
	pme->header().push_back(mimetic::Field("Content-Type: text/html; charset=UTF-8"));
	pme->header().push_back(mimetic::Field("Content-Transfer-Encoding", "quoted-printable"));
	pme->body().assign(content);
	ma.body().parts().push_back(pme);

	std::ofstream output(outFileName,std::ofstream::binary);
	output << ma;;
	output.close();
}

void GenerateMutiMix(const char *content, const char *out_file_name, const char *file_list) {

	mimetic::MultipartMixed *me = new mimetic::MultipartMixed();
	(*me).header().push_back(mimetic::Field("Mime-Version", "1.0"));

	mimetic::MimeEntity *mime_content = new mimetic::MimeEntity();

	mime_content->header().push_back(mimetic::Field("Content-Type: text/html; charset=UTF-8"));
	mime_content->header().push_back(mimetic::Field("Content-Transfer-Encoding", "quoted-printable"));
	mime_content->body().assign(content);
	(*me).body().parts().push_back(mime_content);

	Json::Value list;
	Json::Reader reader;
	reader.parse(file_list, list);
	int size = list.size();
	mimetic::MimeEntity * file_part_list = new mimetic::MimeEntity[size];
	for (int i = 0; i < size; i++) {
		Json::Value file = list[i];
		const char* file_name = file["mime_name"].asCString();
		const char* file_path = file["path"].asCString();
		uint64_t file_size = file["size"].asInt64();
		const char* file_type = file["mime_type"].asCString();
		mimetic::MimeEntity *file_part = &file_part_list[i];
		int file_type_len = strlen(file_type);
		int file_name_len = strlen(file_name);
		char* content_type = new char[file_type_len + file_name_len + 22];
		ZeroMemory(content_type, file_type_len + file_name_len + 22);
		strcat_s(content_type, 15, "Content-Type: ");
		strcat_s(content_type, file_type_len + 15, file_type);
		strcat_s(content_type, file_type_len + 22, "; name=");
		strcat_s(content_type, file_type_len + file_name_len + 22, file_name);
		file_part->header().push_back(mimetic::Field(content_type));

		char *content_dis = new char[44 + file_name_len];
		ZeroMemory(content_dis, 44 + file_name_len);
		strcat_s(content_dis, 44, "Content-Disposition : attachment; filename=");
		strcat_s(content_dis, file_name_len + 44, file_name);
		file_part->header().push_back(mimetic::Field(content_dis));


		std::ostringstream oss;
		oss << file_size;
		std::string ss_size = oss.str();
		file_part->header().push_back(mimetic::Field("size", ss_size));
		file_part->header().push_back(mimetic::Field("Content-Transfer-Encoding", "base64"));

		WCHAR *w_path = CharToWchar_New(file_path);
		FILE *pfile = NULL;
		_wfopen_s(&pfile, w_path, L"rb");
		mimetic::Base64::Encoder b64;
		//mimetic::MaxLineLen mll(64);
		if (pfile) {
			char buffer[4560];
			while (!feof(pfile)) {
				uint64_t read_bytes = fread_s(buffer, 4560, 1, 4560, pfile);
				std::cout << read_bytes << std::endl;
				if (ferror(pfile) || read_bytes == 0) {
					break;
				}
				std::stringstream temp;
				std::ostreambuf_iterator<char> out(temp);
				mimetic::code(buffer, buffer + read_bytes, b64, out);
				std::string str = temp.str();
				file_part->load(str.begin(), str.end(), mimetic::imNone);
			}
			(*me).body().parts().push_back(file_part);
			fclose(pfile);
			//delete[] content_type;
			//delete[] content_dis;
			delete[] w_path;
		}
	}
	std::ofstream output(out_file_name, std::ofstream::binary);
	output << *me;
	//printMimeStructure(me, output, 0);
	output.close();
	//delete[] file_part_list;
	//delete mime_content;
	//delete me;
}




static WCHAR* CharToWchar_New(const char *str) {
	int nLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	if (nLen == 0) {
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen + 1];
	ZeroMemory(pResult, nLen + 1);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, pResult, nLen);
	return pResult;
}