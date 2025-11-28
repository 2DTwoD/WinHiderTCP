cmake_minimum_required(VERSION 3.28)

project(Client)

link_libraries(ws2_32 wsock32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "D:\\Distrib\\Qt\\Qt 6_7_2\\mingw_64\\")

find_package(
        Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(
        Client
        #.cpp
        main.cpp
        src/panels/bind_panel.cpp
        src/panels/com_panel.cpp
        src/panels/main_panel.cpp
        src/panels/main_window_with_tray.cpp
        src/misc/file_work.cpp
        src/tcp/tcp_obj.cpp
        src/hider/win_work.cpp
        #.h
        inc/panels/bind_panel.h
        inc/panels/com_panel.h
        inc/panels/main_panel.h
        inc/panels/main_window_with_tray.h
        inc/misc/file_work.h
        inc/tcp/tcp_obj.h
        inc/misc/thread_builder.h
        inc/hider/win_work.h
)

include_directories(inc)

target_link_libraries(
        Client
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()