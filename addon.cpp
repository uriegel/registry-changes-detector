#include <napi.h>
#include <thread>
#include "event-source-base.h"
using namespace Napi;
using namespace EventSourceBase;
using namespace std;

class RegistryChangesDetector : public EventSourceBase {
public:
    RegistryChangesDetector(const Napi::Function& callback) 
    : EventSourceBase(callback)
    {}
    void OnEvent() override {
        HandleScope scope(callback.Env());
        vector<napi_value> args;
        args.push_back(Number::New(callback.Env(), state));
        callback.Call(args);
    }
    void Start() {
        Initialize();
        thread thread([this] {
            Sleep(2000);
            while (isRunning)
            {
                state++;
                SendEvent();
                Sleep(2000);
            }
        });
        thread.detach();        
    }
    void Stop() {
        Dispose();
        isRunning = false;
    }
private:
    int state{0};
    bool isRunning{true};
};

Value Register(const CallbackInfo& info) {
    auto detector = new RegistryChangesDetector(info[0].As<Function>());
    detector->Start();
    return Number::New(info.Env(), static_cast<double>(reinterpret_cast<uintptr_t>(detector)));
}

Value Unregister(const CallbackInfo& info) {
    auto detector = reinterpret_cast<RegistryChangesDetector*>(static_cast<uintptr_t>(info[0].As<Number>().DoubleValue()));
    detector->Stop();
    return info.Env().Undefined();
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "register"), Function::New(env, Register));
    exports.Set(String::New(env, "unregister"), Function::New(env, Unregister));
    return exports;
}

const char* extension = "registry-changes-detector";
NODE_API_MODULE(extension, Init)