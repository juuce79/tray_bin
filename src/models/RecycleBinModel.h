#pragma once
#include <windows.h>
#include <shellapi.h>

struct RecycleBinStats {
    DWORD itemCount;
    DWORDLONG totalSize;
    bool isEmpty;
};

enum class IconTheme {
    Dark,
    Light
};

class RecycleBinModel {
    public:
        RecycleBinStats GetStats();
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
