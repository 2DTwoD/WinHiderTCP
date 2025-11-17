#ifndef TCP_OBJ_H
#define TCP_OBJ_H
#include <QObject>

#include <winsock2.h>

#include "win_work.h"

class TCPobj : public QObject{
Q_OBJECT
private:
    bool strt{false};
    bool fail{false};
    bool shtdwn{false};
    char ip[17];
    uint16_t port;
    SOCKET serverSocket{INVALID_SOCKET};

    WinWork winWork;
    int initWinSock();
    int createSocket();
    int bindSocket();
    int listenSocket();
public:
    TCPobj(QObject *parent = nullptr);

    ~TCPobj() override;

    void setIP(char *const newIp);
    void setPort(uint16_t newPort);
    void start(char* ip, uint16_t port);
    void stop();
    void shutdown();
    bool started();
    bool failed();
public slots:
    void process();
signals:
    void finished();
};

#endif //TCP_OBJ_H
