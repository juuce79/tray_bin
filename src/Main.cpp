#include <windows.h>
#include "controllers/TrayController.h"
#include "resource.h"

#define TIMER_ID 1

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static TrayController* controller = nullptr;
    
    switch (msg) {
        case WM_CREATE:
            controller = new TrayController(hwnd);
            break;
            
        case WM_APP + 1:
            if (controller) controller->HandleTrayMessage(lParam);
            break;
            
        case WM_APP + 2:
            if (controller) controller->UpdateView();
            break;
            
        case WM_COMMAND:
            if (controller) controller->HandleCommand(wParam);
            break;
            
        case WM_TIMER:
            if (wParam == TIMER_ID && controller) {
                controller->UpdateView();
            }
            break;
            
        case WM_DESTROY:
            if (controller) {
                KillTimer(hwnd, TIMER_ID);
                delete controller;
            }
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX), 0, WndProc, 0, 0, hInstance,
        LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FULL_ICON_LIGHT)), // Changed here
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1), NULL,
        L"MyRecycleBinClass",
        LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FULL_ICON_LIGHT))  // And here
    };

    RegisterClassEx(&wc);
    
    HWND hwnd = CreateWindowEx(0, L"MyRecycleBinClass", 
        L"Recycle Bin Tray", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) {
        return -1;
    }    

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
