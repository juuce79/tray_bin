#pragma once
#include <windows.h>
#include <shellapi.h>

struct RecycleBinStats {
    DWORD itemCount;
    DWORDLONG totalSize;
    bool isEmpty;
};

class RecycleBinModel {
public:
    RecycleBinStats GetStats();
    bool EmptyBin();
    bool OpenBin();
private:
    void NotifyObservers(); // For future observer pattern implementation
};
