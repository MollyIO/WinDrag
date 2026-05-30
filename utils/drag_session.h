#pragma once

#include <windows.h>

namespace drag_session {
    inline HWND g_dragWnd = nullptr;
    inline POINT g_dragOffset = {};
    inline bool g_winKeyDown = false;
    inline bool g_dragging = false;

    HWND FindDraggableWindow(POINT pt);

    bool IsWinKeyDown();
    void SetWinKeyDown(bool down);

    HWND GetTopMoveableWindow(POINT pt);
    bool IsDraggingWindow();
    void BeginDrag(HWND hwnd, POINT cursorScreen);
    void UpdateDrag(POINT cursorScreen);
    void EndDrag();
}
