#include "hider/win_work.h"

WinWork::WinWork(QObject *parent): QObject(parent) {
    ths = this;
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyBoardProc, GetModuleHandle(nullptr), 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(nullptr), 0);
}

BOOL CALLBACK WinWork::enumWindowCB(HWND window, const LPARAM lParam) {
    bool visible = *reinterpret_cast<bool*>(lParam);
    char curName[100];
    memset(curName, 0, 100);
    GetWindowTextA(window, curName, sizeof(curName));
    auto qCurName = QString::fromUtf8(curName, strlen(curName));
    if (qCurName.contains(token.getName(), Qt::CaseInsensitive)){
        qDebug("WinWork: window compare: curName: %s, targetName: %s",
               qCurName.toUtf8().data(), token.getName().toUtf8().data());
        showHide(window, visible);
        return FALSE;
    }
    return TRUE;
}

void WinWork::showHide(HWND window, bool visible) {
    if (visible) {
        ShowWindow(window, SW_SHOW);
        qDebug("WinWork signal: freeClient");
        emit ths->freeClient();
        SetForegroundWindow(window);
    } else {
        ShowWindow(window, SW_HIDE);
    }
}

void WinWork::changeWindowVisible(bool visible) {
    EnumWindows(&enumWindowCB, reinterpret_cast<LPARAM>(&visible));
}

void WinWork::newToken(const Token& tokenObj, TCPexchanger* source) {
    qDebug("WinWork slot: newToken");
    if(token.isValid()) return;
    qDebug("WinWork slot: newToken: token accepted");
    token = tokenObj;
    emit tokenAccepted(source);
    changeWindowVisible(false);
}

void WinWork::freeDone() {
    qDebug("WinWork slot: freeDone");
    token.setValid(false);
}

void WinWork::showHiddenWindow() {
    if(token.getName().isEmpty()) return;
    changeWindowVisible(true);
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
    if (ths && token.isValid() && nCode == HC_ACTION) {
        QString keyName;
        if(keyInfo && overlapWParamList.contains(wParam) && !ignoreKeyList.contains(keyInfo->vkCode)){
            keyName = "k" + QString::number(keyInfo->vkCode, 16);
        } else if(!keyInfo && !ignoreActionList.contains(wParam)){
            keyName = "m" + QString::number(wParam, 16);
        }
        if(keyName.isEmpty()) return;
        if (keyName == token.getKey()) {
            qDebug("WinWork: keyboard/mouse action");
            changeWindowVisible(true);
        }
    }
}
