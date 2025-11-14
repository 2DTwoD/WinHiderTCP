#include <iostream>
#include "win_work.h"

struct WinOpt{
    WinOpt(bool visible, const QString &name) : visible(visible), name(name) {}
    bool visible;
    QString name;
};

void showHide(HWND window, bool visible)
{
    if (visible) {
        ShowWindow(window, SW_SHOW);
        SetForegroundWindow(window);
    } else {
        ShowWindow(window, SW_HIDE);
    }
}

BOOL CALLBACK enumWindowCB(HWND window, const LPARAM lParam)
{
    WinOpt winOpt = *((WinOpt*) lParam);
    char curName[100];
    memset(curName, 0, 100);
    GetWindowTextA(window, curName, sizeof(curName));
    auto qCurName = QString::fromUtf8(curName, 100);
    if (qCurName.contains(winOpt.name, Qt::CaseInsensitive)){
        std::cout << "curName: " << qCurName.toStdString() << "targetName: " << winOpt.name.toStdString() << std::endl;
        showHide(window, winOpt.visible);
        return FALSE;
    }
    return TRUE;
}

void changeWindowVisible(const QString& winName, bool visible){
    WinOpt opt(visible, winName);
    EnumWindows(&enumWindowCB, (LPARAM) &opt);
}