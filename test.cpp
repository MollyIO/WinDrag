#include <windows.h>
#include <iostream>
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_LBUTTONDOWN) {
        if (GetAsyncKeyState(VK_LWIN) & 0x8000) {
            POINT pt = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam)->pt;
            HWND hwnd = WindowFromPoint(pt);
            hwnd = GetAncestor(hwnd, GA_ROOT);
            if (hwnd) {
                SetForegroundWindow(hwnd);
                PostMessage(hwnd, WM_SYSCOMMAND, SC_MOVE + 2, 0);
                return 1; // block click
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
int main() {
    HHOOK hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
