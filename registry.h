#pragma once
#include <string>

extern uintptr_t OpenKey(const std::wstring& mainKeyStr, const std::wstring& subKey);
extern void CloseKey(uintptr_t handle);
extern unsigned int GetNewDWordValue(uintptr_t handle, const std::wstring& valueName, unsigned int defaultValue);
extern bool WaitForChanges(uintptr_t handle);