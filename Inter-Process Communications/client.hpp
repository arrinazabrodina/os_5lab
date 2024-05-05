#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

#define BUFSIZE 1024




class Client {
private:
    void openSocket();
    void setupHost(const char* hostName);
    void setupPort();
    void connectToServer();

    void error(char* msg);
    void mybcopy(void* to, void* from, int n);
public:
    SOCKET sock;
    unsigned short port;
    struct sockaddr_in server;
    char buffer[BUFSIZE];
    Client(unsigned short port) {
        this->port = port;

        const char* hostName = "192.168.0.11";
        const char* msg = "Hello from the client!:)";
        openSocket();
        setupHost(hostName);
        setupPort();
        connectToServer();
        /*sendMessage(msg);
        receiveMessage();
        closeSocket();*/
    }
    void sendMessage(std::string msg);
    void receiveMessage();
    void closeSocket();
};
