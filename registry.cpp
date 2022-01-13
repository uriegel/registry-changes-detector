
bool WaitForChanges(uintptr_t handle) {
    auto key = reinterpret_cast<HKEY>(handle);
    auto status = RegNotifyChangeKeyValue(key, FALSE, REG_NOTIFY_CHANGE_LAST_SET, nullptr, FALSE);
    return status == 0;
}


    