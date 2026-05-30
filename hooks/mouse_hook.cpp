#include "mouse_hook.h"

LRESULT CALLBACK mouse::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0)
        return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);

    const auto *ms = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
    const POINT pt = ms->pt;

    switch (wParam) {
        case WM_LBUTTONDOWN:
            if (drag_session::IsWinKeyDown()) {
                if (HWND hwnd = drag_session::FindDraggableWindow(pt)) {
                    drag_session::BeginDrag(hwnd, pt);
                    return 1;
                }
            }
            break;

        case WM_MOUSEMOVE:
            if (drag_session::IsDraggingWindow())
                drag_session::UpdateDrag(pt);
            break;

        case WM_LBUTTONUP:
            if (drag_session::IsDraggingWindow())
                drag_session::EndDrag();
            break;

        default:
            break;
    }

    return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

bool mouse::InstallMouseHook() {
    g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0);
    return g_mouseHook != nullptr;
}

void mouse::UninstallMouseHook() {
    if (g_mouseHook) {
        UnhookWindowsHookEx(g_mouseHook);
        g_mouseHook = nullptr;
    }
}
