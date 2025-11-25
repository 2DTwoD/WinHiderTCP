#ifndef WIN_WORK_H
#define WIN_WORK_H

#include <winsock2.h>
#include <windows.h>
#include <QString>
#include <QObject>

class WinWork: public QObject{
Q_OBJECT
private:
    static inline QList<uint16_t> ignoreKeyList= {0x20, 0x1B};
    static inline QList<uint16_t> ignoreActionList= {0x200, 0x201, 0x202, 0x204, 0x205};
    static inline bool bindMode{false};
    static inline WinWork *ths{nullptr};
    inline static HHOOK keyboardHook{};
    inline static HHOOK mouseHook{};

    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK LowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static void LowLevelKeyBoardMouse(int nCode, WPARAM wParam, PKBDLLHOOKSTRUCT keyInfo);
public:
    explicit WinWork(QObject *parent);
    ~WinWork() override;

public slots:
    static void startBind();
signals:
    void bindFinished(const QString& value);
    void keyboardMouseAction(const QString& value);
};

#endif //WIN_WORK_H
