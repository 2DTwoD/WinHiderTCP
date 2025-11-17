#include "tcp_obj.h"
#include "thread_builder.h"
#include "tcp_exchanger.h"
#include "win_work.h"

#include <iostream>
#include <windows.h>
#include <QThread>

TCPobj::TCPobj(QObject *parent): QObject(parent), winWork(parent) {
    newThread(this);
}

int TCPobj::initWinSock() {
    // Initialize WSA variables
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    int wsaerr = WSAStartup(wVersionRequested, &wsaData);

    // Check for initialization success
    if (wsaerr != 0) {
        std::cout << "The Winsock dll not found!" << std::endl;
        fail = true;
        return 0;
    } else {
        std::cout << "The Winsock dll found" << std::endl;
        std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
    }
    return 1;
}

int TCPobj::createSocket() {
    // Create a socket
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    } else {
        std::cout << "Socket is OK!" << std::endl;
    }
    return 1;
}

int TCPobj::bindSocket() {
    // Bind the socket to an IP address and port number
    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(ip);  // Replace with your desired IP address
    service.sin_port = htons(port);  // Choose a port number

    // Use the bind function
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    } else {
        std::cout << "bind() is OK!" << std::endl;
    }
    return 1;
}

int TCPobj::listenSocket() {
    // Listen for incoming connections
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
        return 0;
    } else {
        std::cout << "listen() is OK! I'm waiting for new connections..." << std::endl;
    }

    // Accept incoming connections
    SOCKET acceptSocket;
    while(started()){
        acceptSocket = accept(serverSocket, nullptr, nullptr);

        // Check for successful connection
        if (acceptSocket == INVALID_SOCKET) {
            std::cout << "accept failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            break;
        } else {
            std::cout << "accept() is OK!" << std::endl;
        }
        auto tcpExchanger = new TCPexchanger(acceptSocket);
        QObject::connect(tcpExchanger, &TCPexchanger::newToken,
                         &winWork, &WinWork::newToken, Qt::DirectConnection);
        QObject::connect(&winWork, &WinWork::freeClient,
                         tcpExchanger, &TCPexchanger::freeClient, Qt::DirectConnection);
        QObject::connect(tcpExchanger, &TCPexchanger::freeDone,
                         &winWork, &WinWork::freeDone, Qt::DirectConnection);
        newThread(tcpExchanger);
    }
    std::cout << "Server socket " << acceptSocket << " closed" << std::endl;
    return 1;
}

void TCPobj::process() {
    while(!shtdwn){
        if(!started()) continue;
        serverSocket = INVALID_SOCKET;
        fail = false;
        switch(serverSocket){
            case INVALID_SOCKET:
                if((fail = !initWinSock())) break;
            case 0:
                if((fail = !createSocket())) break;
            case 1:
                if((fail = !bindSocket())) break;
            default:
                fail = listenSocket();
        }
        stop();
    }
}

void TCPobj::start(char* ip, uint16_t port) {
    if(started()) return;
    setIP(ip);
    setPort(port);
    strt = true;
}

void TCPobj::stop() {
    if (!started()) return;
    closesocket(serverSocket);
    strt = false;
}

void TCPobj::shutdown() {
    stop();
    shtdwn = true;
    emit finished();
}

bool TCPobj::started() {
    return strt;
}

TCPobj::~TCPobj() {
    std::cout << "TCPobj destructor" << std::endl;
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
