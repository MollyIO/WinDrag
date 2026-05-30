#pragma once
#include "../utils/drag_session.h"

namespace mouse {
    bool InstallMouseHook();
    void UninstallMouseHook();

    inline HHOOK g_mouseHook = nullptr;
    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
}
