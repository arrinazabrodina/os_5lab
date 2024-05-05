#include "server.hpp"
#include <string>
#include <stdio.h>
#include "client.hpp"

using namespace std;

void error(const char* msg)
{
    std::cout << "Error, " << msg << ", errno is\n" << WSAGetLastError();
    exit(0);
}

void Server::setup() {
    version = MAKEWORD(2, 2);
    retval = WSAStartup(version, &stWSAData);
    if (retval != 0) error("WSAStartup");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) error("Opening socket");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    len = sizeof(server);
    if (bind(sock,
        (struct sockaddr*)&server,
        len) != 0)
        error("binding socket");
    fromlen = sizeof(from);
}

void Server::startListening() {
    const char* msg = "I got your message";
    //std::cout << "listen";
    listen(sock, 5);
    shouldRun = true;
    while (shouldRun) {
        newsock = accept(sock,
            (struct sockaddr*)&from,
            &fromlen);
        //printf("A connection has been accepted\n");

        n = recv(newsock, buffer, BUFSIZE, 0);
        if (n < 1) {
            error("Reading from socket");
        }
        else {
            buffer[n] = '\0';
            string result = buffer;
            int number = stoi(result);
            this->result = &number;
            /*printf("The message from %s is %s\n",
                inet_ntoa(from.sin_addr),
                buffer);*/
        }
        n = send(newsock, msg, strlen(msg), 0);
        if (n != strlen(msg)) error("Writing");
        closesocket(newsock);
    }
}