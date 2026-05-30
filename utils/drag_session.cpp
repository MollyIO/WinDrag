#include "drag_session.h"

HWND drag_session::GetTopMoveableWindow(POINT pt) {
    HWND hwnd = WindowFromPoint(pt);
    if (!hwnd)
        return nullptr;

    HWND root = GetAncestor(hwnd, GA_ROOT);
    if (!root)
        return nullptr;

    if (!IsWindowVisible(root))
        return nullptr;
    if (IsIconic(root))
        return nullptr;

    if (LONG style = GetWindowLong(root, GWL_STYLE); !(style & WS_CAPTION))
        return nullptr;

    return root;
}

HWND drag_session::FindDraggableWindow(POINT pt) {
    return GetTopMoveableWindow(pt);
}

bool drag_session::IsWinKeyDown() {
    return g_winKeyDown;
}

void drag_session::SetWinKeyDown(bool down) {
    g_winKeyDown = down;
    if (!down)
        EndDrag();
}

bool drag_session::IsDraggingWindow() {
    return g_dragging;
}

void drag_session::BeginDrag(HWND hwnd, POINT cursorScreen) {
    RECT rc;
    GetWindowRect(hwnd, &rc);
    g_dragOffset.x = cursorScreen.x - rc.left;
    g_dragOffset.y = cursorScreen.y - rc.top;
    g_dragWnd = hwnd;
    g_dragging = true;

    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void drag_session::UpdateDrag(POINT cursorScreen) {
    if (!g_dragWnd || !g_dragging)
        return;

    if (!IsWindow(g_dragWnd)) {
        EndDrag();
        return;
    }

    const int newX = cursorScreen.x - g_dragOffset.x;
    const int newY = cursorScreen.y - g_dragOffset.y;

    SetWindowPos(g_dragWnd, nullptr, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void drag_session::EndDrag() {
    g_dragging = false;
    g_dragWnd = nullptr;
}
