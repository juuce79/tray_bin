#pragma once
#include <windows.h>
#include <shellapi.h>

struct TrayCycleBinStats {
    DWORD itemCount;
    DWORDLONG totalSize;
    bool isEmpty;
};

enum class IconTheme {
    Dark,
    Light
};

class TrayCycleBinModel {
    public:
        TrayCycleBinStats GetStats();
        IconTheme GetIconTheme() const;
        void SetIconTheme(IconTheme theme);
        bool EmptyBin();
        bool OpenBin();
        bool IsAutoStartEnabled();
        bool ToggleAutoStart();        
        bool IsSystemInDarkMode() const;
    private:
        IconTheme m_iconTheme = IconTheme::Dark;
        void NotifyObservers();
};
