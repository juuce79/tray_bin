#pragma once
#include "../models/TrayCycleBinModel.h"
#include "../views/TrayCycleBinView.h"

class TrayCycleBinController {
public:
    TrayCycleBinController(HWND hwnd);
    void HandleCommand(WPARAM wParam);
    void HandleTrayMessage(LPARAM lParam);
    void UpdateView();
private:
    TrayCycleBinModel m_model;
    TrayCycleBinView m_view;
};
