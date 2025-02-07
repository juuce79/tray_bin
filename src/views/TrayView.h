#pragma once
#include <windows.h>
#include "../models/RecycleBinModel.h"

class TrayView {
public:
    TrayView(HWND hwnd, RecycleBinModel* model);
    void UpdateIcon(const RecycleBinStats& stats);
    void ShowContextMenu(const RecycleBinStats& stats);
    void Initialize();
    void Cleanup();
    bool IsAutoStartEnabled() const;
private:
    NOTIFYICONDATA m_nid;
    HWND m_hwnd;
    RecycleBinModel* m_model; // Add this member
};
