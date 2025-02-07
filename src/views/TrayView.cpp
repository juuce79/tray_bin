#include "TrayView.h"
#include "resource.h"
#include <shellapi.h>
#include <iostream>

#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 1002
#define ID_TRAY_EMPTY 1003
#define ID_TRAY_STATS 1004
#define ID_TRAY_OPEN 1005
#define ID_TRAY_AUTOSTART 1006

TrayView::TrayView(HWND hwnd, RecycleBinModel* model) : m_hwnd(hwnd), m_model(model) {
    memset(&m_nid, 0, sizeof(m_nid));
}

bool TrayView::IsAutoStartEnabled() const {
    return m_model->IsAutoStartEnabled();
}

void TrayView::Initialize() {
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd = m_hwnd;
    m_nid.uID = ID_TRAY_APP_ICON;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_APP + 1;
    m_nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_EMPTY_ICON));
    
    Shell_NotifyIcon(NIM_ADD, &m_nid);
}

void TrayView::UpdateIcon(const RecycleBinStats& stats) {
    HICON newIcon = LoadIcon(GetModuleHandle(NULL), 
        MAKEINTRESOURCE(stats.isEmpty ? IDI_EMPTY_ICON : IDI_FULL_ICON));
    
    if (newIcon) {
        m_nid.hIcon = newIcon;
        swprintf_s(m_nid.szTip, L"Recycle Bin\nItems: %lu\nSize: %.2f MB", 
            stats.itemCount, stats.totalSize / (1024.0 * 1024.0));
        Shell_NotifyIcon(NIM_MODIFY, &m_nid);
    }
}

void TrayView::ShowContextMenu(const RecycleBinStats& stats) {
    POINT pt;
    GetCursorPos(&pt);
    
    HMENU hMenu = CreatePopupMenu();
    
    wchar_t statsText[256];
    swprintf_s(statsText, L"Items: %lu (%.2f MB)", 
        stats.itemCount, stats.totalSize / (1024.0 * 1024.0));
    
    InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING | 
        (stats.isEmpty ? (MF_DISABLED | MF_GRAYED) : MF_ENABLED), 
        ID_TRAY_STATS, statsText);
    InsertMenu(hMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenu(hMenu, 2, MF_BYPOSITION | MF_STRING, ID_TRAY_OPEN, L"Open Recycle Bin");
    InsertMenu(hMenu, 3, MF_BYPOSITION | MF_STRING | 
        (stats.isEmpty ? MF_GRAYED : MF_ENABLED), ID_TRAY_EMPTY, L"Empty Recycle Bin");
    InsertMenu(hMenu, 4, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    bool isAutoStartEnabled = IsAutoStartEnabled();
    InsertMenu(hMenu, 5, MF_BYPOSITION | MF_STRING | (isAutoStartEnabled ? MF_CHECKED : MF_UNCHECKED),
        ID_TRAY_AUTOSTART, L"Start with Windows");
    InsertMenu(hMenu, 6, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
    InsertMenu(hMenu, 7, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, L"Exit");
    
    SetForegroundWindow(m_hwnd);
    TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hwnd, NULL);
    PostMessage(m_hwnd, WM_NULL, 0, 0);
    
    DestroyMenu(hMenu);
}

void TrayView::Cleanup() {
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}
