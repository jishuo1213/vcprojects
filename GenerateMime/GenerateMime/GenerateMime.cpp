#include <iostream>
//#include <mimetic\mimetic.h>
#include <json\json.h>
#include <string>
#include "mime.h"

//using namespace mimetic;


int main() {
	//mimetic::MultipartAlternative ma;
	/*ma.header().from("me <me@inspur.com>");
	ma.header().to("you <you@inspur.com>");
	ma.header().subject("first");*/
	//ma.header().push_back(mimetic::Field("Mime-Version", "1.0"));

	//mimetic::MimeEntity *pme = new mimetic::MimeEntity;
	//pme->header().push_back(mimetic::Field("Content-Type: text/plain; charset=UTF-8"));
	//pme->header().push_back(mimetic::Field("Content-Transfer-Encoding", "base64"));
	//pme->body().assign("Hello World");
	//ma.body().parts().push_back(pme);

	//std::cout << ma << std::endl;
	std::string list = R"([{"mime_type":"bb","size":1234312,"mime_name":"aaaa","path":"D:\\Mail\\havefile.eml"},{"mime_type":"bb","size":1234312,"mime_name":"aaaa","path":"D:\\Mail\\havefile.eml"},{"mime_type":"bb","size":1234312,"mime_name":"aaaa","path":"D:\\Mail\\havefile.eml"}])";
	//Json::Reader reader;
	//Json::Value root;
	//if (reader.parse(list, root)) {
	//	std::cout << root.size() << std::endl;
	//}
	//GenerateMultiAlternative("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "D:\\aaa.txt");
	GenerateMutiMix("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "D:\\aaa.txt", list.c_str());
}