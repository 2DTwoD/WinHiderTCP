#ifndef TCP_OBJ_H
#define TCP_OBJ_H

#include <winsock2.h>

#include <QObject>
#include <QMutex>
#include <QTimer>

#define SEND_NEW_TOKEN_RETRY_MS 2000


class TCPobj : public QObject {
Q_OBJECT

private:
    int cnct{0};
    bool fail{false};
    bool shtdwn{false};
    bool sendFlag{false};
    char ip[17];
    uint16_t port;
    SOCKET clientSocket{INVALID_SOCKET};
    QMutex mutex;
    QTimer *sendFlagTimer;

    int initWinSock();

    int createSocket();

    int connectToServer();

    int receiveLoop();

    void closeSocket();

    bool getSendFlag();

    void setSendFlag(bool value);

    bool sendMessage(const QString& message) const;

public:
    TCPobj(QObject *parent);

    ~TCPobj() override;

    void setIP(char *const newIp);

    void setPort(uint16_t newPort);

    void connect(char *ip, uint16_t port);

    void disconnect();

    void shutdown();

    bool connecting() const;

    bool connected() const;

    bool disconnected() const;

    bool failed() const;

    void sendNewToken(const QString &key, const QString &wname);

public slots:

    void process();

    void resetSendFlag();

signals:

    void stopSendFlagTimer();

    void finished();
};

#endif //TCP_OBJ_H
