#include "RecycleBinModel.h"
#include <shlobj.h>

RecycleBinStats RecycleBinModel::GetStats() {
    RecycleBinStats stats = {0, 0, true};
    SHQUERYRBINFO queryRbInfo = {sizeof(SHQUERYRBINFO), 0, 0};
    
    if (SUCCEEDED(SHQueryRecycleBin(NULL, &queryRbInfo))) {
        stats.itemCount = (DWORD)queryRbInfo.i64NumItems;
        stats.totalSize = queryRbInfo.i64Size;
        stats.isEmpty = (queryRbInfo.i64NumItems == 0);
    }
    
    return stats;
}

bool RecycleBinModel::EmptyBin() {
    return SUCCEEDED(SHEmptyRecycleBin(NULL, NULL, SHERB_NOPROGRESSUI | SHERB_NOSOUND));
}

bool RecycleBinModel::OpenBin() {
    return (INT_PTR)ShellExecute(NULL, L"open", L"shell:RecycleBinFolder", 
        NULL, NULL, SW_SHOWNORMAL) > 32;
}

bool RecycleBinModel::IsAutoStartEnabled() {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        DWORD type;
        WCHAR path[MAX_PATH];
        DWORD pathSize = sizeof(path);
        
        bool exists = RegQueryValueEx(hKey, L"RecycleBinMonitor", NULL, &type, 
            (LPBYTE)path, &pathSize) == ERROR_SUCCESS;
        
        RegCloseKey(hKey);
        return exists;
    }
    return false;
}

bool RecycleBinModel::ToggleAutoStart() {
    WCHAR exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);
    
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
        0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS) {
        
        if (IsAutoStartEnabled()) {
            // Remove from startup
            RegDeleteValue(hKey, L"RecycleBinMonitor");
        } else {
            // Add to startup
            RegSetValueEx(hKey, L"RecycleBinMonitor", 0, REG_SZ, 
                (BYTE*)exePath, (wcslen(exePath) + 1) * sizeof(WCHAR));
        }
        
        RegCloseKey(hKey);
        return true;
    }
    return false;
}
