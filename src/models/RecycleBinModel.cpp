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
