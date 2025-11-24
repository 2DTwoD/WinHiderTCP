#include <QApplication>
#include <QPushButton>

#include <iostream>
#include <winsock2.h>
#include <thread>

int socketClient(){

    // Sending data to the server
    char buffer[200];
    std::cout << "Enter the message: ";
    std::cin.getline(buffer, 200);
    int sbyteCount = send(clientSocket, buffer, 200, 0);
    if (sbyteCount == SOCKET_ERROR) {
        std::cout << "Client send error: " << WSAGetLastError() << std::endl;
        return -1;
    } else {
        std::cout << "Client: Sent " << sbyteCount << " bytes" << std::endl;
    }

    // Receiving data from the server
    char receiveBuffer[200];
    int rbyteCount = recv(clientSocket, receiveBuffer, 200, 0);
    if (rbyteCount < 0) {
        std::cout << "Client recv error: " << WSAGetLastError() << std::endl;
        return 0;
    } else {
        std::cout << "Client: Received data: " << receiveBuffer << std::endl;
    }

    // Continue with the client setup...
    return 0;
}

int main(int argc, char *argv[]) {
    std::thread t(socketClient);
    QApplication a(argc, argv);
    QPushButton button("Hello client!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
