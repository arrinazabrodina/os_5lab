#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

#define BUFSIZE 1024

class Server {
private:
    SOCKET sock, newsock;
    int len, fromlen;
    DWORD n, retval;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buffer[BUFSIZE];
    WORD version;
    WSADATA stWSAData;
    void setup();
    bool shouldRun;
public:
    unsigned short port;
    int* result;
    Server(unsigned short port) {
        this->port = port;
        setup();
        shouldRun = false;
    }
    void startListening();
};