#ifndef TCP_OBJ_H
#define TCP_OBJ_H

#include "tcp_exchanger.h"

#include <QObject>
#include <QThread>
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

    int initWinSock();
    int createSocket();
    int bindSocket();
    int listenSocket();
    void closeSocket();
public:
    TCPobj();

    ~TCPobj() override;

    void setIP(char *const newIp);
    void setPort(uint16_t newPort);
    void start(char* ip, uint16_t port);
    void stop();
    void shutdown();
    bool stopped() const;
    bool starting() const;
    bool started() const;
    bool failed() const;
    QThread *const getThread() const;

public slots:
    void process();
signals:
    void finished();
    void newTCPexchanger(SOCKET acceptSocket);
    void tcpObjStop();
};

#endif //TCP_OBJ_H
