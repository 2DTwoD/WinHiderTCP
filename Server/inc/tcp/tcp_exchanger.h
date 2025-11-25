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
    bool accept{false};
public:
    explicit TCPexchanger(SOCKET acceptSocket = SOCKET_ERROR);

    ~TCPexchanger() override;

    bool sendToClient(const char* str);
    void shutdown();
    void closeSocket();
    bool accepted();
public slots:
    void process();
    void freeClient();
    void tokenAccepted(TCPexchanger* source);
signals:
    void newToken(const Token& tokenObj, TCPexchanger* source);
    void freeDone();
    void clearTCPexchanger(TCPexchanger* tcpExchager);
    void finished();
};

#endif //TCP_EXCHANGER_H
