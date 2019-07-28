#pragma once
#include <napi.h>
#include <string>

class WString : public Napi::String {
public:
    template <typename T>
    static WString From(napi_env env, const T& value);

    static WString New(napi_env env, const std::wstring& val) {
        return WString::New(env, val.c_str(), val.size());
    }

    static WString New(napi_env env, const wchar_t* val, size_t length) {
        napi_value value;
        napi_create_string_utf16(env, reinterpret_cast<const char16_t*>(val), length, &value);
        return WString(env, value);
    }
    
    WString(napi_env env, napi_value value) : Napi::String(env, value) { }

    operator std::wstring() const { return WValue(); }

    std::wstring WValue() const { 
        size_t length;
        napi_get_value_string_utf16(_env, _value, nullptr, 0, &length);

        std::wstring value;
        value.reserve(length + 1);
        value.resize(length);
        napi_get_value_string_utf16(_env, _value, reinterpret_cast<char16_t*>(&value[0]), value.capacity(), nullptr);
        return value;
    }
};
