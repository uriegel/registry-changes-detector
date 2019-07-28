#include <windows.h>
#include <string>
#include "registry.h"
using namespace std;

uintptr_t OpenKey(const wstring& mainKeyStr, const wstring& subKey) {
    auto mainKey = mainKeyStr == L"HKEY_CURRENT_USER"
        ? HKEY_CURRENT_USER
        : HKEY_LOCAL_MACHINE;
    HKEY key{0};        
    RegOpenKeyW(mainKey, subKey.c_str(), &key);        
    return reinterpret_cast<uintptr_t>(key);
}

void CloseKey(uintptr_t handle) {
    auto key = reinterpret_cast<HKEY>(handle);
    if (key)
        RegCloseKey(key);
}

unsigned int GetNewDWordValue(uintptr_t handle, const wstring& valueName) {
    auto key = reinterpret_cast<HKEY>(handle);
    DWORD value{0};
    DWORD cbData = sizeof(DWORD);
    RegQueryValueExW(key, valueName.c_str(), nullptr, nullptr, (LPBYTE)&value, &cbData);
    return value;
}

bool WaitForChanges(uintptr_t handle) {
    auto key = reinterpret_cast<HKEY>(handle);
    auto status = RegNotifyChangeKeyValue(key, FALSE, REG_NOTIFY_CHANGE_LAST_SET, nullptr, FALSE);
    return status == 0;
}


    