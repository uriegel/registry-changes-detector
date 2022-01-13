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
        auto  = keyStr.substr(0, pos);
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
                if (key == 0)
                    break;
                auto new_value = GetNewDWordValue(key, valueName, 0);
                if (new_value != value) {
                    value = new_value;
                    SendEvent();
                }
            }
        });
        thread.detach();        
    }
    void Stop() {
        auto keyToClose = key;
        key = 0;
        CloseKey(keyToClose);
        Dispose();
    }
    unsigned int GetDWord(unsigned int defaultValue) {
        auto new_value = GetNewDWordValue(key, valueName, defaultValue);
        value = new_value;
        return value;
    }
private:
    uintptr_t key{0};
    wstring valueName;
    unsigned int value{0};
};

Value Register(const CallbackInfo& info) {
#ifdef WINDOWS    
    auto detector = new RegistryChangesDetector(info[0].As<Object>(), info[1].As<Function>());
    detector->Start();
    return Number::New(info.Env(), static_cast<double>(reinterpret_cast<uintptr_t>(detector)));
#else
    return Number::New(info.Env(), 0.0);
#endif
}

Value Unregister(const CallbackInfo& info) {
#ifdef WINDOWS        
    auto detector = reinterpret_cast<RegistryChangesDetector*>(static_cast<uintptr_t>(info[0].As<Number>().DoubleValue()));
    detector->Stop();
#endif    
    return info.Env().Undefined();
}

Number GetDWord(const CallbackInfo& info) {
#ifdef WINDOWS            
    auto default_value = info.Length() > 1 ? info[1].As<Number>().Uint32Value() : 0;
    auto detector = reinterpret_cast<RegistryChangesDetector*>(static_cast<uintptr_t>(info[0].As<Number>().DoubleValue()));
    auto value = detector->GetDWord(default_value);
    return Number::New(info.Env(), value);
#else 
    return Number::New(info.Env(), 0.0);
#endif   
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "register"), Function::New(env, Register));
    exports.Set(String::New(env, "unregister"), Function::New(env, Unregister));
    exports.Set(String::New(env, "getDWord"), Function::New(env, GetDWord));
    return exports;
}

const char* extension = "registry-changes-detector";
NODE_API_MODULE(extension, Init)