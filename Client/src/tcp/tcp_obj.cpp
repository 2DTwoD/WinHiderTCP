#include "tcp/tcp_obj.h"
#include "misc/thread_builder.h"

#include <windows.h>

#define SEND_NEW_TOKEN_RETRY_MS 3000

TCPobj::TCPobj(): currentThread(newThread(this)){
}

int TCPobj::initWinSock() {
    // Initialize WSA variables
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    int wsaerr = WSAStartup(wVersionRequested, &wsaData);

    // Check for initialization success
    if (wsaerr != 0) {
        qDebug("TCPobj: the Winsock dll not found!");
        return 0;
    } else {
        qDebug("TCPobj: the Winsock dll found");
        qDebug("TCPobj: the status: %s", wsaData.szSystemStatus);
    }
    return 1;
}

int TCPobj::createSocket() {
    // Create a socket
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (clientSocket == INVALID_SOCKET) {
        qDebug("TCPobj: error at socket(): %d", WSAGetLastError());
        WSACleanup();
        return 0;
    } else {
        qDebug("TCPobj: socket is OK!");
    }
    return 1;
}

int TCPobj::connectToServer() {
    // Bind the socket to an IP address and port number
    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(ip);
    service.sin_port = htons(port);

    // Use the connect function
    if (::connect(clientSocket, reinterpret_cast<sockaddr*>(&service), sizeof(service)) == SOCKET_ERROR) {
        qDebug("TCPobj: client: connect() - Failed to connect: %d", WSAGetLastError());
        WSACleanup();
        return 0;
    } else {
        qDebug("TCPobj: client connect() is OK!");
        qDebug("TCPobj: client can connect sending and receiving data...");
    }
    return 1;
}

int TCPobj::receiveLoop() {
    // Receiving data from the server
    char receiveBuffer[10];
    int rbyteCount;
    while(true) {
        memset(receiveBuffer, 0, 10);
        rbyteCount = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
        if (rbyteCount <= 0) {
            int lastError = WSAGetLastError();
            qDebug("TCPobj: client recv error:  %d", lastError);
            return lastError == WSAECONNABORTED;
        } else {
            qDebug("TCPobj: client received data: %s", receiveBuffer);
            if(!strcmp(receiveBuffer, "OK") && sendFlagTimer->isActive()){
                qDebug("TCPobj: stop sendFlagTimer");
                emit stopSendFlagTimer();
            } else if(!strcmp(receiveBuffer, "FREE")){
                qDebug("TCPobj: reset sendFlag");
                setSendFlag(false);
            }
        }
    }
}

void TCPobj::process() {
    while(!shtdwn) {
        if(disconnected()) continue;
        clientSocket = INVALID_SOCKET;
        fail = false;
        switch(clientSocket){
            case INVALID_SOCKET:
                fail = !initWinSock();
                if(fail) break;
            case 0:
                fail = !createSocket();
                if(fail) break;
            case 1:
                fail = !connectToServer();
                if(fail) break;
            default:
                setCnct(2);
                fail = !receiveLoop();
        }
        disconnect();
    }
    emit finished();
}

void TCPobj::connect(const QString &ip, uint16_t port) {
    if(connected()) return;
    setIP(ip.toUtf8().data());
    setPort(port);
    setCnct(1);
}

void TCPobj::disconnect() {
    if (disconnected()) return;
    closeSocket();
    setCnct(0);
    resetSendFlag();
}

void TCPobj::shutdown() {
    qDebug("TCPobj: shutdown");
    disconnect();
    shtdwn = true;
}

bool TCPobj::connected() {
    return getCnct() == 2;
}

TCPobj::~TCPobj() {
    qDebug("TCPobj: destructor");
}

void TCPobj::setIP(char *const newIp) {
    strncpy(ip, newIp, 16);
    ip[16]  = 0;
}

void TCPobj::setPort(uint16_t newPort) {
    port = newPort;
}

bool TCPobj::failed() {
    return fail;
}

void TCPobj::closeSocket() {
    if(clientSocket != INVALID_SOCKET){
        closesocket(clientSocket);
    }
}


bool TCPobj::sendMessage(QString message) {
    if(!connected()) return false;
    // Sending data to the server
    char utf8data[200];
    memcpy(utf8data, message.toUtf8().data(), 200);
    //utf8data = message.toUtf8().data();
    int sbyteCount = send(clientSocket, utf8data, strlen(utf8data), 0);
    if (sbyteCount == SOCKET_ERROR) {
        qDebug("TCPobj: client send error:  %d", WSAGetLastError());
        return false;
    } else {
        qDebug("TCPobj: client sent %d bytes, message: %s", sbyteCount, utf8data);
    }
    return true;
}

bool TCPobj::connecting() {
    return getCnct() == 1;
}

bool TCPobj::disconnected() {
    return getCnct() == 0;
}

void TCPobj::sendNewToken(const QString& key, const QString& wname) {
    if(getSendFlag()) return;
    if(sendMessage("key=" + key + ";wname=" + wname)){
        setSendFlag(true);
        sendFlagTimer = new QTimer(QThread::currentThread()->parent());
        QObject::connect(sendFlagTimer, &QTimer::timeout, this, &TCPobj::resetSendFlag, Qt::DirectConnection);
        QObject::connect(this, &TCPobj::stopSendFlagTimer, sendFlagTimer, &QTimer::stop);
        sendFlagTimer->start(SEND_NEW_TOKEN_RETRY_MS);
        qDebug("TCPobj: send new Token");
    }
}

void TCPobj::setSendFlag(bool value) {
    mutex.lock();
    sendFlag = value;
    mutex.unlock();
}

bool TCPobj::getSendFlag() {
    mutex.lock();
    bool result = sendFlag;
    mutex.unlock();
    return result;
}

void TCPobj::resetSendFlag() {
    qDebug("TCPobj: reset sendFlag");
    setSendFlag(false);
    if(sendFlagTimer) sendFlagTimer->stop();
}

QThread *TCPobj::getThread() {
    return currentThread;
}

bool TCPobj::isBusy() {
    return sendFlagTimer && sendFlagTimer->isActive();
}

int TCPobj::getCnct() {
    mutex.lock();
    int result = cnct;
    mutex.unlock();
    return result;
}

void TCPobj::setCnct(int value) {
    mutex.lock();
    cnct = value;
    mutex.unlock();
}
