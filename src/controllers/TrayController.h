#pragma once
#include "../models/RecycleBinModel.h"
#include "../views/TrayView.h"

class TrayController {
public:
    TrayController(HWND hwnd);
    void HandleCommand(WPARAM wParam);
    void HandleTrayMessage(LPARAM lParam);
    void UpdateView();
private:
    RecycleBinModel m_model;
    TrayView m_view;
};
