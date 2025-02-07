#include "TrayController.h"
#include <windows.h>

#define ID_TRAY_EXIT 1002
#define ID_TRAY_EMPTY 1003
#define ID_TRAY_OPEN 1005

TrayController::TrayController(HWND hwnd) : m_view(hwnd) {
    m_view.Initialize();
    UpdateView();
}

void TrayController::HandleCommand(WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case ID_TRAY_OPEN:
            m_model.OpenBin();
            break;
        case ID_TRAY_EMPTY:
            if (m_model.EmptyBin()) {
                MessageBox(NULL, L"Recycle Bin Emptied Successfully", 
                    L"Recycle Bin", MB_OK | MB_ICONINFORMATION);
            } else {
                MessageBox(NULL, L"Failed to Empty Recycle Bin", 
                    L"Error", MB_OK | MB_ICONERROR);
            }
            UpdateView();
            break;
        case ID_TRAY_EXIT:
            m_view.Cleanup();
            PostQuitMessage(0);
            break;
    }
}

void TrayController::HandleTrayMessage(LPARAM lParam) {
    if (lParam == WM_RBUTTONUP) {
        m_view.ShowContextMenu(m_model.GetStats());
    }
}

void TrayController::UpdateView() {
    m_view.UpdateIcon(m_model.GetStats());
}
