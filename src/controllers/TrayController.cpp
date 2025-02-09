#include "TrayController.h"
#include <windows.h>
#include <shlobj.h>

#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 1002
#define ID_TRAY_EMPTY 1003
#define ID_TRAY_STATS 1004
#define ID_TRAY_OPEN 1005
#define ID_TRAY_AUTOSTART 1006
#define ID_TRAY_THEME_DARK 1007
#define ID_TRAY_THEME_LIGHT 1008
#define ID_TRAY_THEME_SYSTEM 1009

#define TIMER_ID 1
#define UPDATE_INTERVAL 1000

TrayController::TrayController(HWND hwnd) : m_model(), m_view(hwnd, &m_model) {
    m_view.Initialize();
    UpdateView();
    
    // Set up timer for periodic updates
    SetTimer(hwnd, TIMER_ID, UPDATE_INTERVAL, NULL);
    
    // Register for shell change notifications
    SHChangeNotifyEntry shCne;
    shCne.pidl = NULL;
    shCne.fRecursive = TRUE;
    
    SHChangeNotifyRegister(hwnd, 
        SHCNRF_InterruptLevel | SHCNRF_ShellLevel,
        SHCNE_ALLEVENTS,
        WM_APP + 2,
        1, &shCne);
}

void TrayController::HandleCommand(WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case ID_TRAY_OPEN:
            m_model.OpenBin();
            break;
        case ID_TRAY_EMPTY:
            if (!m_model.EmptyBin()) {
                MessageBox(NULL, L"Failed to Empty Recycle Bin", 
                    L"Error", MB_OK | MB_ICONERROR);
            }
            UpdateView();
            break;
        case ID_TRAY_EXIT:
            m_view.Cleanup();
            PostQuitMessage(0);
            break;
        case ID_TRAY_AUTOSTART:
            if (m_model.ToggleAutoStart()) {
                UpdateView();
            }
            break;
        case ID_TRAY_THEME_DARK:
            m_model.SetIconTheme(IconTheme::Dark);
            UpdateView();
            break;
        case ID_TRAY_THEME_LIGHT:
            m_model.SetIconTheme(IconTheme::Light);
            UpdateView();
            break;
        case ID_TRAY_THEME_SYSTEM:
            m_model.SetIconTheme(IconTheme::System);
            UpdateView();
            break;
    }
}

void TrayController::HandleTrayMessage(LPARAM lParam) {
    if (lParam == WM_RBUTTONUP) {
        m_view.ShowContextMenu(m_model.GetStats());
    }
    else if (lParam == WM_LBUTTONDBLCLK) {
        m_model.OpenBin();
    }
}

void TrayController::UpdateView() {
    m_view.UpdateIcon(m_model.GetStats());
}
