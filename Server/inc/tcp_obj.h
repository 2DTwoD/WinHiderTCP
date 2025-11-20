#ifndef TCP_OBJ_H
#define TCP_OBJ_H

#include "win_work.h"
#include "tcp_exchanger.h"

#include <QObject>
#include <winsock2.h>


class TCPobj : public QObject{
Q_OBJECT
private:
    bool strt{false};
    bool fail{false};
    bool shtdwn{false};
    char ip[17];
    uint16_t port;
    SOCKET serverSocket{INVALID_SOCKET};
    WinWork* winWork;
    std::unique_ptr<QList<TCPexchanger*>> clientList;

    int initWinSock();
    int createSocket();
    int bindSocket();
    int listenSocket();
    void closeSocket();
public:
    TCPobj(QObject *parent);

    ~TCPobj() override;

    void setIP(char *const newIp);
    void setPort(uint16_t newPort);
    void start(char* ip, uint16_t port);
    void stop();
    void shutdown();
    bool started() const;
    bool failed() const;

public slots:
    void process();
signals:
    void finished();
};

#endif //TCP_OBJ_H
