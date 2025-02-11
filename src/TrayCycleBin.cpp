#include <windows.h>
#include <gdiplus.h>
#include "controllers/TrayCycleBinController.h"
#include "resource.h"
#include "utils/TrayCycleBinIconLoader.h"

#define TIMER_ID 1

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static TrayCycleBinController* controller = nullptr;
    
    switch (msg) {
        case WM_CREATE:
            controller = new TrayCycleBinController(hwnd);
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
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX), 
        0, 
        WndProc, 
        0, 
        0, 
        hInstance,
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON)),
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1), 
        NULL,
        L"MyTrayCycleBinClass",
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON))
    };

    RegisterClassEx(&wc);
    
    HWND hwnd = CreateWindowEx(0, L"MyTrayCycleBinClass", 
        L"TrayCycle Bin", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    
    if (hwnd == NULL) {
        Gdiplus::GdiplusShutdown(gdiplusToken);
        return -1;
    }    

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}
