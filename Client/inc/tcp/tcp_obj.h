#ifndef TCP_OBJ_H
#define TCP_OBJ_H

#include <winsock2.h>

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QThread>


class TCPobj : public QObject {
Q_OBJECT

private:
    int cnct{0};
    bool shtdwn{false};
    bool fail{false};
    bool sendFlag{false};
    char ip[17];
    uint16_t port;
    SOCKET clientSocket{INVALID_SOCKET};
    QMutex mutex;
    QTimer *sendFlagTimer{nullptr};
    QThread *currentThread;

    int initWinSock();

    int createSocket();

    int connectToServer();

    int receiveLoop();

    void closeSocket();

    void setSendFlag(bool value);

    int getCnct();

    void setCnct(int value);

    bool sendMessage(const QString& message);

public:

    TCPobj();
    ~TCPobj() override;

    void setIP(char *const newIp);

    void setPort(uint16_t newPort);

    void connect(char *ip, uint16_t port);

    void disconnect();

    void shutdown();

    bool connecting();

    bool connected();

    bool disconnected();

    bool failed();

    void sendNewToken(const QString &key, const QString &wname);

    bool getSendFlag();

    QThread *getThread();

    bool isBusy();

public slots:

    void process();

    void resetSendFlag();

signals:

    void stopSendFlagTimer();

    void finished();
};

#endif //TCP_OBJ_H
