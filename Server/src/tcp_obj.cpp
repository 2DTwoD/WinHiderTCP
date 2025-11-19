#include "tcp_obj.h"
#include "thread_builder.h"
#include "win_work.h"

#include <windows.h>
#include <QThread>

TCPobj::TCPobj(QObject *parent): winWork(new WinWork(this)), clientList(std::make_unique<QList<TCPexchanger*>>()) {
    newThread(parent, this);
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
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for socket creation success
    if (serverSocket == INVALID_SOCKET) {
        qDebug("TCPobj: error at socket(): %d", WSAGetLastError());
        WSACleanup();
        return 0;
    } else {
        qDebug("TCPobj: socket is OK!");
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
        qDebug("TCPobj: bind() failed: %d", WSAGetLastError());
        closeSocket();
        WSACleanup();
        return 0;
    } else {
        qDebug("TCPobj: bind() is OK!");
    }
    return 1;
}

int TCPobj::listenSocket() {
    // Listen for incoming connections
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        qDebug("TCPobj: listen(): Error listening on socket: %d", WSAGetLastError());
        return 0;
    } else {
        qDebug("TCPobj: listen() is OK! I'm waiting for new connections...");
    }
    // Accept incoming connections
    SOCKET acceptSocket;
    while(started()){
        acceptSocket = accept(serverSocket, nullptr, nullptr);
        // Check for successful connection
        if (acceptSocket == INVALID_SOCKET) {
            qDebug("TCPobj: accept failed: %d", WSAGetLastError());
            WSACleanup();
            break;
        } else {
            qDebug("TCPobj: accept() is OK!");
        }
        auto tcpExchanger = new TCPexchanger(acceptSocket);
        QObject::connect(tcpExchanger, &TCPexchanger::newToken,
                         winWork, &WinWork::newToken, Qt::DirectConnection);
        QObject::connect(winWork, &WinWork::freeClient,
                         tcpExchanger, &TCPexchanger::freeClient, Qt::DirectConnection);
        QObject::connect(tcpExchanger, &TCPexchanger::freeDone,
                         winWork, &WinWork::freeDone, Qt::DirectConnection);
        newThread(this, tcpExchanger);
        clientList->append(tcpExchanger);
    }
    qDebug("TCPobj: server socket %d closed", acceptSocket);
    return 1;
}

void TCPobj::process() {
    while(!shtdwn){
        if(!started()) continue;
        serverSocket = INVALID_SOCKET;
        fail = false;
        switch(serverSocket){
            case INVALID_SOCKET:
                fail = !initWinSock();
                if(fail) break;
            case 0:
                fail = !createSocket();
                if(fail) break;
            case 1:
                fail = !bindSocket();
                if(fail) break;
            default:
                fail = !listenSocket();
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
    closeSocket();
    strt = false;
    winWork->showHiddenWindow();
}

void TCPobj::shutdown() {
    qDebug("TCPobj shutdown");
    for(auto item: *clientList){
        item->shutdown();
    }
    stop();
    shtdwn = true;
    emit finished();
}

bool TCPobj::started() const {
    return strt;
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

bool TCPobj::failed() const {
    return fail;
}

void TCPobj::closeSocket() {
    if(serverSocket != INVALID_SOCKET){
        closesocket(serverSocket);
    }
}
