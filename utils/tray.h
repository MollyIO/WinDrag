#pragma once

#include <windows.h>

#define WM_TRAYICON  (WM_USER + 1)
#define ID_TRAY_EXIT  1001
#define ID_TRAY_ICON  1

constexpr wchar_t kTrayClassName[] = L"WinDragTrayWindowClass";
constexpr wchar_t kTrayWindowTitle[] = L"WinDrag Tray";

namespace tray {
    bool RegisterTrayClass(HINSTANCE hInst);
    HWND CreateTrayWindow(HINSTANCE hInst);

    void AddTrayIcon(HWND hwnd);
    void RemoveTrayIcon(HWND hwnd);
    void ShowTrayMenu(HWND hwnd);

    LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
