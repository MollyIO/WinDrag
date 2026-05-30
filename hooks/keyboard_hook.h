#pragma once
#include "../utils/drag_session.h"

namespace keyboard {
    bool InstallKeyboardHook();
    void UninstallKeyboardHook();

    inline HHOOK g_kbHook = nullptr;
    LRESULT CALLBACK KbProc(int nCode, WPARAM wParam, LPARAM lParam);
}
