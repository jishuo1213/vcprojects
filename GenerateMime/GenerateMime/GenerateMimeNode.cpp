#include <nan.h>
#include "mime.h"

#pragma comment(lib,"D:\\WorkSpace\\C++\\vcprojects\\GenerateMime\\GenerateMime\\libmimetic_mt.lib")

void generate_multi_alternative(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Nan::Utf8String content(info[0]->ToString());
	Nan::Utf8String output_file(info[1]->ToString());

	wchar_t *w_out_file = CharToWchar_New(*output_file);
	GenerateMultiAlternative(*content, w_out_file);
	delete[] w_out_file;
}

void generate_multi_mixed(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Nan::Utf8String content(info[0]->ToString());
	Nan::Utf8String output_file(info[1]->ToString());
	Nan::Utf8String file_list(info[2]->ToString());

	wchar_t *w_out_file = CharToWchar_New(*output_file);
	GenerateMutiMix(*content, w_out_file, *file_list);
	delete[] w_out_file;
}


void Init(v8::Local<v8::Object> exports) {
	NAN_EXPORT(exports, generate_multi_alternative);
	NAN_EXPORT(exports, generate_multi_mixed);
}

NODE_MODULE(mime, Init)