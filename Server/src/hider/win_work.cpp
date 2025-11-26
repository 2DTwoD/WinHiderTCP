#include "hider/win_work.h"

WinWork::WinWork(QObject *parent): QObject(parent) {
    ths = this;
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyBoardProc, GetModuleHandle(nullptr), 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(nullptr), 0);
}

BOOL CALLBACK WinWork::enumWindowCB(HWND window, const LPARAM lParam) {
    auto pars = *reinterpret_cast<WinNameVisible*>(lParam);
    char curName[100];
    memset(curName, 0, 100);
    GetWindowTextA(window, curName, sizeof(curName));
    auto qCurName = QString::fromUtf8(curName, strlen(curName));
    if (qCurName.contains(pars.winName, Qt::CaseInsensitive)){
        qDebug("WinWork: window compare: curName: %s, targetName: %s",
               qCurName.toUtf8().data(), pars.winName.toUtf8().data());
        showHide(window, pars.visible);
        return FALSE;
    }
    return TRUE;
}

void WinWork::showHide(HWND window, bool visible) {
    if (visible) {
        ShowWindow(window, SW_SHOW);
        if(!IsWindowVisible(window)) {
            SetForegroundWindow(window);
        }
        qDebug("WinWork signal: freeClient");
        emit ths->freeClient();
    } else {
        ShowWindow(window, SW_HIDE);
    }
}

void WinWork::changeWindowVisible(bool visible, const QString& winName) {
    WinNameVisible pars = {visible, winName};
    EnumWindows(&enumWindowCB, reinterpret_cast<LPARAM>(&pars));
}


WinWork::~WinWork() {
    qDebug("WinWork: destructor");
}

LRESULT CALLBACK WinWork::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    LowLevelKeyBoardMouse(nCode, wParam, nullptr);
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WinWork::LowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    auto keyInfo = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
    LowLevelKeyBoardMouse(nCode, wParam, keyInfo);
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void WinWork::LowLevelKeyBoardMouse(int nCode, WPARAM wParam, PKBDLLHOOKSTRUCT const &keyInfo) {
//    if(keyInfo) qDebug("WinWork: keyName: %#04x, %#04x", keyInfo->vkCode, wParam);
    if (ths && nCode == HC_ACTION) {
        QString keyName;
        if(keyInfo && overlapWParamList.contains(wParam) && !ignoreKeyList.contains(keyInfo->vkCode)){
            keyName = "k" + QString::number(keyInfo->vkCode, 16);
        } else if(!keyInfo && !ignoreActionList.contains(wParam)){
            keyName = "m" + QString::number(wParam, 16);
        }
        if(keyName.isEmpty()) return;
        emit ths->keyboardMouseAction(keyName);
    }
}
