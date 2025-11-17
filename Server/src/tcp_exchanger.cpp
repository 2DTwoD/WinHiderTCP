#include "tcp_exchanger.h"

#include <windows.h>
#include <iostream>


TCPexchanger::TCPexchanger(SOCKET acceptSocket): acceptSocket(acceptSocket){
}

void TCPexchanger::process() {
    char receiveBuffer[DATA_LEN];
    while(1){
        memset(receiveBuffer, 0, DATA_LEN);
        // Receive data from the client
        int rbyteCount = recv(acceptSocket, receiveBuffer, DATA_LEN, 0);
        if (rbyteCount < 0) {
            std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
            break;
        } else {
            std::cout << "Received data: " << receiveBuffer << std::endl;
        }
        Token token = parseMessage(receiveBuffer);
        if(token.isValid()){
            sendToClient("OK");
            emit newToken(token);
        } else{
            sendToClient("BAD");
        }
    }
    closesocket(acceptSocket);
    std::cout << "Client socket " << acceptSocket << " closed" << std::endl;
    emit finished();
}

Token TCPexchanger::parseMessage(char *const message) {
    Token result(true);
    QString qMessage(message);
    auto list = qMessage.split(";");
    for(auto item: list){
        auto keyValue = item.split("=");
        if(keyValue[0] == "key"){
            result.setKey(static_cast<WPARAM>(keyValue[1].toUInt()));
        } else if(keyValue[0] == "wname"){
            result.setWName(std::move(keyValue[1]));
        } else {
            result.setValid(false);
            break;
        }
    }
    std::cout << "tokenObj: isValid: " << result.isValid() << ", key: " << result.getKey()
              << ", wname: " << result.getName().toStdString() << std:: endl;
    return result;
}

bool TCPexchanger::sendToClient(const char* str) {
    // Send a response to the client
    int sbyteCount = send(acceptSocket, str, strlen(str), 0);
    if (sbyteCount == SOCKET_ERROR) {
        std::cout << "Server send error: " << WSAGetLastError() << std::endl;
        return false;
    } else {
        std::cout << "Server: Sent " << sbyteCount << " bytes" << std::endl;
    }
    return true;
}

void TCPexchanger::freeClient() {
    if(sendToClient("FREE")){
        emit freeDone();
    }
}
