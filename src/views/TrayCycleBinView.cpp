#include "TrayCycleBinView.h"
#include "resource.h"
#include "../utils/TrayCycleBinIconLoader.h"
#include <shellapi.h>
#include <iostream>
#include <gdiplus.h>

namespace {
    int GetIconResourceForItemCount(bool isDark, DWORD itemCount) {
        int index = (itemCount >= 43) ? 43 : static_cast<int>(itemCount);
        return isDark ? (IDI_BIN_DARK_00 + index) : (IDI_BIN_LIGHT_00 + index);
    }
}

#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 1002
#define ID_TRAY_EMPTY 1003
#define ID_TRAY_STATS 1004
#define ID_TRAY_OPEN 1005
#define ID_TRAY_AUTOSTART 1006
#define ID_TRAY_THEME_DARK 1007
#define ID_TRAY_THEME_LIGHT 1008
#define ID_TRAY_THEME_SYSTEM 1009

TrayCycleBinView::TrayCycleBinView(HWND hwnd, TrayCycleBinModel* model) : m_hwnd(hwnd), m_model(model) {
    memset(&m_nid, 0, sizeof(m_nid));
}

bool TrayCycleBinView::IsAutoStartEnabled() const {
    return m_model->IsAutoStartEnabled();
}

void TrayCycleBinView::Initialize() {
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd = m_hwnd;
    m_nid.uID = ID_TRAY_APP_ICON;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_APP + 1;
    bool isDark = m_model->GetIconTheme() == IconTheme::Dark;
    
    // Use isDark to set initial icon
    m_nid.hIcon = LoadPNGAsIcon(GetIconResourceForItemCount(isDark, 0));
    
    Shell_NotifyIcon(NIM_ADD, &m_nid);
}

void TrayCycleBinView::UpdateIcon(const TrayCycleBinStats& stats) {
    bool isDark = m_model->GetIconTheme() == IconTheme::Dark;
    int iconResource = GetIconResourceForItemCount(isDark, stats.itemCount);
    
    HICON newIcon = LoadPNGAsIcon(iconResource);
    
    if (newIcon) {
        if (m_nid.hIcon) DestroyIcon(m_nid.hIcon);
        m_nid.hIcon = newIcon;
        swprintf_s(m_nid.szTip, L"TrayCycle Bin\nItems: %lu\nSize: %.2f MB", 
            stats.itemCount, stats.totalSize / (1024.0 * 1024.0));
        Shell_NotifyIcon(NIM_MODIFY, &m_nid);
    }
}

void TrayCycleBinView::ShowContextMenu(const TrayCycleBinStats& stats) {
    POINT pt;
    GetCursorPos(&pt);
    
    HMENU hMenu = CreatePopupMenu();
    
    wchar_t statsText[256];
    swprintf_s(statsText, L"Items: %lu (%.2f MB)", 
        stats.itemCount, stats.totalSize / (1024.0 * 1024.0));
    
    InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING | 
        (stats.isEmpty ? (MF_DISABLED | MF_GRAYED) : MF_ENABLED), 
        ID_TRAY_STATS, statsText);
    InsertMenu(hMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
    InsertMenu(hMenu, 2, MF_BYPOSITION | MF_STRING, ID_TRAY_OPEN, L"Open Bin");
    InsertMenu(hMenu, 3, MF_BYPOSITION | MF_STRING | 
        (stats.isEmpty ? MF_GRAYED : MF_ENABLED), ID_TRAY_EMPTY, L"Empty Bin");
    InsertMenu(hMenu, 4, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
    bool isAutoStartEnabled = IsAutoStartEnabled();
    InsertMenu(hMenu, 5, MF_BYPOSITION | MF_STRING | (isAutoStartEnabled ? MF_CHECKED : MF_UNCHECKED),
        ID_TRAY_AUTOSTART, L"Start at Logon");
    InsertMenu(hMenu, 6, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
    InsertMenu(hMenu, 7, MF_BYPOSITION | MF_STRING | 
        (m_model->GetIconTheme() == IconTheme::Dark ? MF_CHECKED : MF_UNCHECKED),
        ID_TRAY_THEME_DARK, L"Dark Bin");
    InsertMenu(hMenu, 8, MF_BYPOSITION | MF_STRING |
        (m_model->GetIconTheme() == IconTheme::Light ? MF_CHECKED : MF_UNCHECKED),
        ID_TRAY_THEME_LIGHT, L"Light Bin");
    InsertMenu(hMenu, 9, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
    InsertMenu(hMenu, 10, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, L"Exit");
    
    SetForegroundWindow(m_hwnd);
    TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hwnd, nullptr);
    PostMessage(m_hwnd, WM_NULL, 0, 0);
    
    DestroyMenu(hMenu);
}

void TrayCycleBinView::Cleanup() {
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}
