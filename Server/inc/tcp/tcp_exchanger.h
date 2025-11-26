#ifndef TCP_EXCHANGER_H
#define TCP_EXCHANGER_H

#include "misc/token.h"

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
    void freeClient();
public slots:
    void process();
    void tokenAccepted(TCPexchanger* source);
    void hiderBusy();
signals:
    void newToken(const Token& newToken, TCPexchanger *const sender);
    void freeDone();
    void deleteTCPexchanger(TCPexchanger *const tcpExchager);
    void finished();
};

#endif //TCP_EXCHANGER_H
