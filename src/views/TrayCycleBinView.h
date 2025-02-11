#pragma once
#include <windows.h>
#include "../models/TrayCycleBinModel.h"

class TrayCycleBinView {
public:
    TrayCycleBinView(HWND hwnd, TrayCycleBinModel* model);
    void UpdateIcon(const TrayCycleBinStats& stats);
    void ShowContextMenu(const TrayCycleBinStats& stats);
    void Initialize();
    void Cleanup();
    bool IsAutoStartEnabled() const;
private:
    NOTIFYICONDATA m_nid;
    HWND m_hwnd;
    TrayCycleBinModel* m_model; // Add this member
};
