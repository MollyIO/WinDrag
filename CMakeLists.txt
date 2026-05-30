cmake_minimum_required(VERSION 4.2)
project(WinDrag)

set(CMAKE_CXX_STANDARD 20)

add_executable(WinDrag
        main.cpp

        utils/tray.cpp
        utils/drag_session.cpp

        hooks/mouse_hook.cpp
        hooks/keyboard_hook.cpp
)
