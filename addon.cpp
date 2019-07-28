#include <napi.h>
#include <thread>
#include <event-source-base.h>
#include "wstring.h"
#include "registry.h"
using namespace Napi;
using namespace EventSourceBase;
using namespace std;

class RegistryChangesDetector : public EventSourceBase {
public:
    RegistryChangesDetector(const Napi::Object& arg, const Napi::Function& callback) 
    : EventSourceBase(callback)
    {
        auto keyStr = arg.Get("key").As<WString>().WValue();
        valueName = arg.Get("value").As<WString>().WValue();
        auto type = arg.Get("type").As<WString>().WValue();
        auto pos = keyStr.find(L"\\");
        auto mainKeyStr = keyStr.substr(0, pos);
        auto subKey = keyStr.substr(pos + 1);
        key = OpenKey(mainKeyStr, subKey);
    }
    ~RegistryChangesDetector() { CloseKey(key); }
    void OnEvent() override {
        HandleScope scope(callback.Env());
        vector<napi_value> args;
        args.push_back(Number::New(callback.Env(), value));
        callback.Call(args);
    }
    void Start() {
        Initialize();
        thread thread([this] {
            while (true) {
                if (!WaitForChanges(key))
                    break;
                auto new_value = GetNewDWordValue(key, valueName);
                if (new_value != value) {
                    value = new_value;
                    SendEvent();
                }
            }
        });
        thread.detach();        
    }
    void Stop() {
        CloseKey(key);
        key = 0;
        Dispose();
    }
    DWORD GetDWord() {
        auto new_value = GetNewDWordValue(key, valueName);
        value = new_value;
        return value;
    }
private:
    uintptr_t key{0};
    wstring valueName;
    DWORD value{0};
};

Value Register(const CallbackInfo& info) {
    auto detector = new RegistryChangesDetector(info[0].As<Object>(), info[1].As<Function>());
    detector->Start();
    return Number::New(info.Env(), static_cast<double>(reinterpret_cast<uintptr_t>(detector)));
}

Value Unregister(const CallbackInfo& info) {
    auto detector = reinterpret_cast<RegistryChangesDetector*>(static_cast<uintptr_t>(info[0].As<Number>().DoubleValue()));
    detector->Stop();
    return info.Env().Undefined();
}

Number GetDWord(const CallbackInfo& info) {
    auto detector = reinterpret_cast<RegistryChangesDetector*>(static_cast<uintptr_t>(info[0].As<Number>().DoubleValue()));
    auto value = detector->GetDWord();
    return Number::New(info.Env(), value);
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "register"), Function::New(env, Register));
    exports.Set(String::New(env, "unregister"), Function::New(env, Unregister));
    exports.Set(String::New(env, "getDWord"), Function::New(env, GetDWord));
    return exports;
}

const char* extension = "registry-changes-detector";
NODE_API_MODULE(extension, Init)