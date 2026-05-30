#include "tray.h"

#include <shellapi.h>
#include <strsafe.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "user32.lib")

void tray::AddTrayIcon(HWND hwnd) {
    NOTIFYICONDATAW nid = {};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY_ICON;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    //nid.hIcon = static_cast<HICON>(LoadImageW(nullptr, MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON, 0, 0, LR_SHARED));

    Shell_NotifyIconW(NIM_ADD, &nid);
}

void tray::RemoveTrayIcon(HWND hwnd) {
    NOTIFYICONDATAW nid = {};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = ID_TRAY_ICON;
    Shell_NotifyIconW(NIM_DELETE, &nid);
}

void tray::ShowTrayMenu(HWND hwnd) {
    HMENU menu = CreatePopupMenu();
    AppendMenuW(menu, MF_STRING, ID_TRAY_EXIT, L"Exit");

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hwnd);
    TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);
    DestroyMenu(menu);
}

LRESULT CALLBACK tray::TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP || lParam == WM_CONTEXTMENU)
                ShowTrayMenu(hwnd);
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_TRAY_EXIT)
                DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            RemoveTrayIcon(hwnd);
            PostQuitMessage(0);
            return 0;

        default:
            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

bool tray::RegisterTrayClass(HINSTANCE hInst) {
    static bool registered = false;
    if (registered)
        return true;

    WNDCLASSW wc = {};
    wc.lpfnWndProc = TrayWndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = kTrayClassName;

    if (!RegisterClassW(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
        return false;

    registered = true;
    return true;
}

HWND tray::CreateTrayWindow(HINSTANCE hInst) {
    if (!RegisterTrayClass(hInst))
        return nullptr;

    HWND hwnd = CreateWindowExW(0, kTrayClassName, kTrayWindowTitle, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, hInst, nullptr);
    if (!hwnd)
        return nullptr;

    AddTrayIcon(hwnd);
    return hwnd;
}
