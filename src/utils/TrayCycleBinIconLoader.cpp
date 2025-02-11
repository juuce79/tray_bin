#include "TrayCycleBinIconLoader.h"
#include <gdiplus.h>

HICON LoadPNGAsIcon(UINT resourceId) {
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceId), L"PNG");
    HGLOBAL hGlobal = LoadResource(NULL, hResource);
    LPVOID pData = LockResource(hGlobal);
    DWORD size = SizeofResource(NULL, hResource);
    
    IStream* pStream = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &pStream);
    pStream->Write(pData, size, NULL);
    
    Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromStream(pStream);
    HICON hIcon = NULL;
    bitmap->GetHICON(&hIcon);
    
    delete bitmap;
    pStream->Release();
    
    return hIcon;
}
