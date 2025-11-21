#ifndef TCP_EXCHANGER_H
#define TCP_EXCHANGER_H

#include "hider/token.h"

#include <QObject>
#include <winsock2.h>

#define DATA_LEN 200

//TCPexchanger
class TCPexchanger : public QObject {
    Q_OBJECT
private:
    SOCKET acceptSocket;
    Token parseMessage(char *const message);
    bool shtdwn{false};
public:
    explicit TCPexchanger(SOCKET acceptSocket = SOCKET_ERROR);

    ~TCPexchanger() override;

    bool sendToClient(const char* str);
    void shutdown();
    void closeSocket();
public slots:
    void process();
    void freeClient();
signals:
    void newToken(Token tokenObj);
    void freeDone();
    void clearTCPexchanger(TCPexchanger* tcpExchager);
    void finished();
};

#endif //TCP_EXCHANGER_H
