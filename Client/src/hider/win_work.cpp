#include "hider/win_work.h"

WinWork::WinWork(QObject *parent): QObject(parent) {
    ths = this;
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyBoardProc, GetModuleHandle(nullptr), 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(nullptr), 0);
}

LRESULT CALLBACK WinWork::LowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    auto keyInfo = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
    LowLevelKeyBoardMouse(nCode, wParam, keyInfo);
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK WinWork::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    LowLevelKeyBoardMouse(nCode, wParam, nullptr);
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void WinWork::LowLevelKeyBoardMouse(int nCode, WPARAM wParam, const PKBDLLHOOKSTRUCT& keyInfo) {
    if (ths && nCode == HC_ACTION) {
        QString keyName;
        if(keyInfo && overlapWParamList.contains(wParam) && !ignoreKeyList.contains(keyInfo->vkCode)){
            keyName = "k" + QString::number(keyInfo->vkCode, 16);
        } else if(!keyInfo && !ignoreActionList.contains(wParam)){
            keyName = "m" + QString::number(wParam, 16);
        }
        if(keyName.isEmpty()) return;
        if(bindMode){
            qDebug("WinWork: binding complete");
            bindMode = false;
            emit ths->bindFinished(keyName);
            return;
        } else {
            qDebug("WinWork: emit keyboard/Mouse action");
            emit ths->keyboardMouseAction(keyName);
        }
    }
}


WinWork::~WinWork() {
    qDebug("WinWork: destructor");
}

void WinWork::startBind() {
    qDebug("WinWork: connect binding");
    bindMode = true;
}

