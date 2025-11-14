#ifndef TCP_EXCHANGER_H
#define TCP_EXCHANGER_H

#include "token.h"

#include <QObject>
#include <winsock2.h>

#define DATA_LEN 200

//TCPexchanger
class TCPexchanger : public QObject {
    Q_OBJECT
private:
    SOCKET acceptSocket;
    Token parseMessage(char *const message);
public:
    explicit TCPexchanger(SOCKET acceptSocket = SOCKET_ERROR);
    bool sendToClient(const char* str);
public slots:
    void process();
    signals:
    void newToken(Token& tokenObj);
    void finished();
};

#endif //TCP_EXCHANGER_H
