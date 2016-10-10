#include <nan.h>
#include "smime.h"


#pragma comment(lib,"D:\\Tools\\Include\\lib\\libeay32.lib")
#pragma comment(lib,"D:\\Tools\\Include\\lib\\ssleay32.lib")


void SignMail(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 4) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	Nan::Utf8String recipfile(info[0]->ToString());
	Nan::Utf8String pass(info[1]->ToString());
	Nan::Utf8String infile(info[2]->ToString());
	Nan::Utf8String outfile(info[3]->ToString());

	int ret = sign(*recipfile, *pass, *infile, *outfile);
	v8::Local<v8::Number> res = Nan::New(ret);
	info.GetReturnValue().Set(res);
}

void DecryptMail(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 4) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	Nan::Utf8String recipfile(info[0]->ToString());
	Nan::Utf8String pass(info[1]->ToString());
	Nan::Utf8String infile(info[2]->ToString());
	Nan::Utf8String outfile(info[3]->ToString());

	int ret = decrypt_smime(*recipfile, *pass, *infile, *outfile);
	v8::Local<v8::Number> res = Nan::New(ret);
	info.GetReturnValue().Set(res);
}


void EncryptMail(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 3) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	Nan::Utf8String recipfile(info[0]->ToString());
	Nan::Utf8String infile(info[1]->ToString());
	Nan::Utf8String outfile(info[2]->ToString());

	int ret = encrypt_mime(*recipfile, *infile, *outfile);
	v8::Local<v8::Number> res = Nan::New(ret);
	info.GetReturnValue().Set(res);
}

void VerifyMail(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 3) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	Nan::Utf8String infile(info[0]->ToString());
	Nan::Utf8String outfile(info[1]->ToString());
	Nan::Utf8String recipfile(info[2]->ToString());
	int ret = verify_mime(*infile, *outfile, *recipfile);
	v8::Local<v8::Number> res = Nan::New(ret);
	info.GetReturnValue().Set(res);
}

void DecryptCMSMail(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 4) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	Nan::Utf8String recipfile(info[0]->ToString());
	Nan::Utf8String pass(info[1]->ToString());
	Nan::Utf8String infile(info[2]->ToString());
	Nan::Utf8String outfile(info[3]->ToString());

	int ret = decrypt_cms(*recipfile, *pass, *infile, *outfile);
	v8::Local<v8::Number> res = Nan::New(ret);
	info.GetReturnValue().Set(res);
}

void CheckPass(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Nan::Utf8String recipfile(info[0]->ToString());
	Nan::Utf8String pass(info[1]->ToString());

	int ret = parse_cert_file(*recipfile, *pass,NULL);
	v8::Local<v8::Number> res = Nan::New(ret);
	info.GetReturnValue().Set(res);
}


void DecryptInit(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	init();
}

void DecryptClean(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	clean();
}

void Init(v8::Local<v8::Object> exports) {
	NAN_EXPORT(exports, DecryptInit);
	NAN_EXPORT(exports, DecryptClean);
	NAN_EXPORT(exports, SignMail);
	NAN_EXPORT(exports, EncryptMail);
	NAN_EXPORT(exports, DecryptMail);
	NAN_EXPORT(exports, VerifyMail);
	NAN_EXPORT(exports, DecryptCMSMail);
	NAN_EXPORT(exports, CheckPass);
}

NODE_MODULE(DecryptMail, Init)