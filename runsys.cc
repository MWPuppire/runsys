#include <nan.h>
#include <iostream>
#include <array>
#include <string>
using namespace v8;
using namespace Nan;
using namespace std;
NAN_METHOD(RunSystem) {
  Isolate* isolate = info.GetIsolate();
  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No string provided")));
    return;
  }
  if (!info[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be a string")));
    return;
  }
  String::Utf8Value val(info[0]->ToString());
  string str (*val);
  const char *command = str.c_str();
  array<char, 128> buffer;
  string result;
  shared_ptr<FILE> pipe(popen(command, "r"), pclose);
  if (!pipe) {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Error starting command")));
    return;
  }
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
      result += buffer.data();
    }
  }
  Local<String> retval = Nan::New<String>(result.c_str()).ToLocalChecked();
  info.GetReturnValue().Set(retval);
}
NAN_METHOD(Print) {
  Isolate* isolate = info.GetIsolate();
  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No string provided")));
    return;
  }
  if (!info[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be a string")));
    return;
  }
  String::Utf8Value val(info[0]->ToString());
  string str (*val);
  cout << str;
}
NAN_METHOD(PrintLine) {
  Isolate* isolate = info.GetIsolate();
  if (info.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "No string provided")));
    return;
  }
  if (!info[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "First argument must be a string")));
    return;
  }
  String::Utf8Value val(info[0]->ToString());
  string str (*val);
  cout << str << endl;
}
NAN_METHOD(Input) {
  if (info.Length() > 0) {
    String::Utf8Value val(info[0]->ToString());
    string str (*val);
    cout << str;
  }
  string result;
  getline(cin, result);
  Local<String> retval = Nan::New<String>(result.c_str()).ToLocalChecked();
  info.GetReturnValue().Set(retval);
}
void Init(Local<Object> exports, Local<Object> module, Local<Object> context) {
  SetMethod(exports, "system", RunSystem);
  SetMethod(exports, "print", Print);
  SetMethod(exports, "println", PrintLine);
  SetMethod(exports, "input", Input);
}
NODE_MODULE(runsys, Init)
