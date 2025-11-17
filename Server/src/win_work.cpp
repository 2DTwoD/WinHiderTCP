#include <iostream>
#include "win_work.h"

WinWork::WinWork(QObject *parent): QObject(parent) {
    ths = this;
    keyboardMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelKeyBoardMouseProc, GetModuleHandle(nullptr), 0);
}

LRESULT CALLBACK WinWork::LowLevelKeyBoardMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (ths && token.isValid() && nCode == HC_ACTION) {
        if (wParam == token.getKey()) {
            std::cout << "Keyboard/Mouse action"<< std::endl;
            changeWindowVisible(true);
        }
    }
    return CallNextHookEx(keyboardMouseHook, nCode, wParam, lParam);
}

BOOL CALLBACK WinWork::enumWindowCB(HWND window, const LPARAM lParam) {
    bool visible = *reinterpret_cast<bool*>(lParam);
    char curName[100];
    memset(curName, 0, 100);
    GetWindowTextA(window, curName, sizeof(curName));
    auto qCurName = QString::fromUtf8(curName, strlen(curName));
    if (qCurName.contains(token.getName(), Qt::CaseInsensitive)){
        std::cout << "curName: " << qCurName.toStdString() << ", targetName: " << token.getName().toStdString() << std::endl;
        showHide(window, visible);
        return FALSE;
    }
    return TRUE;
}

void WinWork::showHide(HWND window, bool visible) {
    if (visible) {
        ShowWindow(window, SW_SHOW);
        emit ths->freeClient();
        SetForegroundWindow(window);
    } else {
        ShowWindow(window, SW_HIDE);
    }
}

void WinWork::changeWindowVisible(bool visible) {
    EnumWindows(&enumWindowCB, reinterpret_cast<LPARAM>(&visible));
}

void WinWork::newToken(Token& tokenObj) {
    if(token.isValid()) return;
    token = std::move(tokenObj);
    changeWindowVisible(false);
}

void WinWork::freeDone() {
    token.setValid(false);
}