#ifndef TCP_OBJ_H
#define TCP_OBJ_H

#include "tcp_exchanger.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <winsock2.h>


class TCPobj : public QObject{
Q_OBJECT
private:
    int strt{0};
    bool fail{false};
    bool shtdwn{false};
    char ip[17];
    uint16_t port;
    SOCKET serverSocket{INVALID_SOCKET};
    QThread* currentThread;
    QMutex mutex;

    int initWinSock();
    int createSocket();
    int bindSocket();
    int listenSocket();
    void closeSocket();
    int getStrt();
    void setStrt(int value);

public:
    TCPobj();

    ~TCPobj() override;

    void setIP(char *const newIp);
    void setPort(uint16_t newPort);
    void start(char* ip, uint16_t port);
    void stop();
    void shutdown();
    bool stopped();
    bool starting();
    bool started();
    bool failed();
    QThread *const getThread();

public slots:
    void process();
signals:
    void finished();
    void newTCPexchanger(SOCKET acceptSocket);
    void tcpObjStop();
};

#endif //TCP_OBJ_H
