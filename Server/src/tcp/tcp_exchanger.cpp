#include "tcp/tcp_exchanger.h"

#include <windows.h>


TCPexchanger::TCPexchanger(SOCKET acceptSocket): acceptSocket(acceptSocket){
}

void TCPexchanger::process() {
    char receiveBuffer[DATA_LEN];
    while(!shtdwn){
        memset(receiveBuffer, 0, DATA_LEN);
        // Receive data from the client
        int rbyteCount = recv(acceptSocket, receiveBuffer, DATA_LEN, 0);
        if (rbyteCount <= 0) {
            qDebug("TCPexchanger: server recv error: %d", WSAGetLastError());
            break;
        } else {
            qDebug("TCPexchanger: received data: %s", receiveBuffer);
        }
        if(accepted()) continue;
        Token token = std::move(parseMessage(receiveBuffer));
        if(token.isValid()){
            if(sendToClient("OK")){
                qDebug("TCPexchanger signal: newToken");
                emit newToken(token, this);
            }
        } else {
            sendToClient("BAD");
        }
    }
    emit deleteTCPexchanger(this);
    closeSocket();
    qDebug("TCPexchanger: client socket %d closed", acceptSocket);
    emit finished();
}

Token TCPexchanger::parseMessage(char *const message) {
    Token result(true);
    QString qMessage(message);
    auto list = qMessage.split(";");
    for(auto item: list){
        auto keyValue = item.split("=");
        if(keyValue[0] == "key"){
            result.setKey(std::move(keyValue[1]));
        } else if(keyValue[0] == "wname"){
            result.setWName(std::move(keyValue[1]));
        } else {
            result.setValid(false);
            break;
        }
    }
    if(result.getKey().size() > 2 && !QString("km").contains(result.getKey()[0])){
        result.setValid(false);
    }
    qDebug("TCPexchanger: parsed token: isValid: %d, key %s, wname %s",
           result.isValid(), result.getKey().toUtf8().data(), result.getName().toUtf8().data());
    return result;
}

bool TCPexchanger::sendToClient(const char* str) {
    // Send a response to the client
    int sbyteCount = send(acceptSocket, str, strlen(str), 0);
    if (sbyteCount == SOCKET_ERROR) {
        qDebug("TCPexchanger: server send error: %d", WSAGetLastError());
        return false;
    } else {
        qDebug("TCPexchanger: server: sent %d bytes", sbyteCount);
    }
    return true;
}

void TCPexchanger::freeClient() {
    qDebug("TCPexchanger: freeClient");
    if(accepted() && sendToClient("FREE")){
        qDebug("TCPexchanger: freeDone");
        accept = false;
        emit freeDone();
    }
}

void TCPexchanger::shutdown() {
    qDebug("TCPexchanger: client socket %d shutdown", acceptSocket);
    shtdwn = true;
}

TCPexchanger::~TCPexchanger() {
    qDebug("TCPexchanger destructor");
}

void TCPexchanger::closeSocket() {
    closesocket(acceptSocket);
}

void TCPexchanger::tokenAccepted(TCPexchanger* source) {
    accept = source == this;
    qDebug("TCPexchanger: client socket %d: token %saccepted", acceptSocket, accept? "": "not ");
}

bool TCPexchanger::accepted() {
    return accept;
}

void TCPexchanger::hiderBusy() {
    if(accepted()) return;
    sendToClient("BUSY");
}
