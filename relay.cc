#include <node.h>
#include "usbrelay/usbrelay.h"

using namespace v8;

void SetRelay(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 3) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  int deviceId = (int)args[0]->NumberValue();
  int portId = (int)args[1]->NumberValue();
  int status = (int)args[2]->NumberValue();
  int ret = -2;

  libusb_device_handle *devh = NULL;
  devh = USBRELAY_Open(deviceId);

  if (devh == NULL) {
    ret = -1; 
  } else {
    ret = USBRELAY_SetRelay(devh, portId, status);
    USBRELAY_Close(devh);
  }
  Local<Number> num = Number::New(isolate, ret);
  args.GetReturnValue().Set(num);
}

void GetRelay(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  int deviceId = (int)args[0]->NumberValue();
  int portId = (int)args[1]->NumberValue();
  int status = -1;

  libusb_device_handle *devh = NULL;
  devh = USBRELAY_Open(deviceId);

  if (devh == NULL) {
    status = -1; 
  } else {
    USBRELAY_GetRelay(devh, portId, &status);
    USBRELAY_Close(devh);
  }
  Local<Number> num = Number::New(isolate, status);
  args.GetReturnValue().Set(num);
}

void ToggleRelay(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  int deviceId = (int)args[0]->NumberValue();
  int portId = (int)args[1]->NumberValue();
  int status = -1;

  libusb_device_handle *devh = NULL;
  devh = USBRELAY_Open(deviceId);

  if (devh == NULL) {
    status = -1; 
  } else {
    USBRELAY_GetRelay(devh, portId, &status);
    status = !status;
    int ret = USBRELAY_SetRelay(devh, portId, status);
    status = ret ? ret: status;
  }
  USBRELAY_Close(devh);
  Local<Number> num = Number::New(isolate, status);
  args.GetReturnValue().Set(num);
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "setRelay", SetRelay);
  NODE_SET_METHOD(exports, "getRelay", GetRelay);
  NODE_SET_METHOD(exports, "toggleRelay", ToggleRelay);
}

NODE_MODULE(relay, Init)
