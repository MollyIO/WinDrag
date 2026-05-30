#include "keyboard_hook.h"

LRESULT CALLBACK keyboard::KbProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0)
        return CallNextHookEx(g_kbHook, nCode, wParam, lParam);

    if (const auto *ks = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam); ks->vkCode == VK_LWIN || ks->vkCode == VK_RWIN) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
            drag_session::SetWinKeyDown(true);
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
            drag_session::SetWinKeyDown(false);
    }

    return CallNextHookEx(g_kbHook, nCode, wParam, lParam);
}

bool keyboard::InstallKeyboardHook() {
    g_kbHook = SetWindowsHookEx(WH_KEYBOARD_LL, KbProc, nullptr, 0);
    return g_kbHook != nullptr;
}

void keyboard::UninstallKeyboardHook() {
    if (g_kbHook) {
        UnhookWindowsHookEx(g_kbHook);
        g_kbHook = nullptr;
    }
}
