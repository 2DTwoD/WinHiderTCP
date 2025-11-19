#include "win_work.h"

WinWork::WinWork(QObject *parent): QObject(parent) {
    ths = this;
    keyboardMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelKeyBoardMouseProc, GetModuleHandle(nullptr), 0);
}

LRESULT CALLBACK WinWork::LowLevelKeyBoardMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (ths && token.isValid() && nCode == HC_ACTION) {
        if (wParam == token.getKey()) {
            qDebug("WinWork: Keyboard/Mouse action");
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

void WinWork::newToken(Token tokenObj) {
    qDebug("WinWork slot: newToken");
    if(token.isValid()) return;
    token = std::move(tokenObj);
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
