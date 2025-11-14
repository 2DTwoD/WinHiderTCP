#ifndef TCP_OBJ_H
#define TCP_OBJ_H
#include <QObject>

#include <winsock2.h>

#include "token.h"

class TCPobj : public QObject{
Q_OBJECT
private:
    bool strt{false};
    bool shtdwn{false};
    char ip[17];
    uint16_t port;
    SOCKET serverSocket{INVALID_SOCKET};
    static inline Token token;
    int initWinSock();
    int createSocket();
    int bindSocket();
    int listenSocket();
    void clearTokenObject();

    inline static HHOOK mouseHook{};
    inline static HHOOK keyboardHook{};
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
public:
    TCPobj(QObject *parent = nullptr);

    ~TCPobj() override;

    void setIP(char *const newIp);
    void setPort(uint16_t newPort);
    void start(char* ip, uint16_t port);
    void stop();
    void shutdown();
    bool started();
public slots:
    void process();
    void receiveToken(Token& tokenObj);
signals:
    void finished();
};

#endif //TCP_OBJ_H
