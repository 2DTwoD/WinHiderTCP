#ifndef WIN_WORK_H
#define WIN_WORK_H

#include "misc/token.h"

#include <winsock2.h>
#include <windows.h>
#include <QString>
#include <QObject>

struct WinNameVisible{
    bool visible{false};
    const QString& winName;
};

class WinWork: public QObject{
Q_OBJECT
private:
    static inline QList<uint16_t> ignoreKeyList= {0x20, 0x1B};
    static inline QList<uint16_t> overlapWParamList= {0x100, 0x104};
    static inline QList<uint16_t> ignoreActionList= {0x200, 0x201, 0x202, 0x204, 0x205};
    static inline WinWork *ths{nullptr};
    inline static HHOOK keyboardHook{};
    inline static HHOOK mouseHook{};
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK LowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static void LowLevelKeyBoardMouse(int nCode, WPARAM wParam, const PKBDLLHOOKSTRUCT& keyInfo);
    static BOOL CALLBACK enumWindowCB(HWND window, const LPARAM lParam);
    static void showHide(HWND window, bool visible);
public:

    explicit WinWork(QObject *parent);
    ~WinWork() override;
    void changeWindowVisible(bool visible, const QString& winName);
signals:
    void freeClient();
    void keyboardMouseAction(const QString& keyName);
};

#endif //WIN_WORK_H
