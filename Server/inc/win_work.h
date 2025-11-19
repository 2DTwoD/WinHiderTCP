#ifndef WIN_WORK_H
#define WIN_WORK_H
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <QString>
#include <QObject>
#include "token.h"

class WinWork: public QObject{
Q_OBJECT
private:
    static inline Token token;
    static inline WinWork *ths{nullptr};
    inline static HHOOK keyboardMouseHook{};
    static LRESULT CALLBACK LowLevelKeyBoardMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static BOOL CALLBACK enumWindowCB(HWND window, const LPARAM lParam);
    static void showHide(HWND window, bool visible);
    static void changeWindowVisible(bool visible);
public:
    explicit WinWork(QObject *parent);

    ~WinWork() override;

public slots:
    void newToken(Token tokenObj);
    void freeDone();
    void showHiddenWindow();
signals:
    void freeClient();
};

#endif //WIN_WORK_H
