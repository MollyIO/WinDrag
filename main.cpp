#include <windows.h>

#include "hooks/keyboard_hook.h"
#include "hooks/mouse_hook.h"
#include "utils/tray.h"

#pragma comment(lib, "user32.lib")

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    HANDLE mutex = CreateMutex(nullptr, TRUE, "WinDrag");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBox(nullptr, "WinDrag is already running.", "WinDrag", MB_ICONINFORMATION | MB_OK);
        return 1;
    }

    HWND trayWnd = tray::CreateTrayWindow(hInst);
    if (!trayWnd) {
        CloseHandle(mutex);
        MessageBox(nullptr, "Failed to create tray window.", "WinDrag", MB_ICONERROR | MB_OK);
        return 1;
    }

    if (!keyboard::InstallKeyboardHook() || !mouse::InstallMouseHook()) {
        MessageBox(nullptr, "Failed to install hooks. Try running as Administrator.","WinDrag", MB_ICONERROR | MB_OK);
        if (IsWindow(trayWnd))
            DestroyWindow(trayWnd);

        mouse::UninstallMouseHook();
        keyboard::UninstallKeyboardHook();
        CloseHandle(mutex);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    mouse::UninstallMouseHook();
    keyboard::UninstallKeyboardHook();

    if (IsWindow(trayWnd))
        DestroyWindow(trayWnd);

    CloseHandle(mutex);
    return static_cast<int>(msg.wParam);
}
