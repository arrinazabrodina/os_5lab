#include "client.hpp"

using namespace std;


void Client::error(char* msg)
{
#if defined __linux__ || defined __APPLE__ 
    cout << "Error, " << msg << ", errno is\n" << errno;
#else
    cout << "Error, " << msg << ", errno is\n" << WSAGetLastError();
#endif
    exit(0);
}

void Client::mybcopy(void* to, void* from, int n)
{
    int i;
    char* tempto = (char*)to;
    char* tempfrom = (char*)from;
    for (i = 0; i < n; i++)
        tempto[i] = tempfrom[i];
}

void Client::openSocket() {
#if defined __linux__ || defined __APPLE__ 
#else
    WSADATA stWSAData;
    WORD version = MAKEWORD(2, 2);
    DWORD retval = WSAStartup(version, &stWSAData);
    if (retval != 0) error((char*)"WSAStartUp");
#endif
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) error((char*)"Opening socket");
}

void Client::setupHost(const char* hostName) {
    server.sin_family = AF_INET;
    void* addr;
#if defined __linux__ || defined __APPLE__
    struct hostent* hp = gethostbyname(hostName);
    if (hp == NULL) error((char*)"Unknown host");
    addr = hp->h_addr;
#else
    struct addrinfo* result;
    int err = getaddrinfo(hostName, NULL, NULL, &result);
    if (err != 0) {
        error((char*)"getaddrinfo");
    }
    addr = &((struct sockaddr_in*)result->ai_addr)->sin_addr;
#endif
    mybcopy(&(server.sin_addr), addr, 4);
}

void Client::setupPort() {
    server.sin_port = htons(port);
}

void Client::connectToServer() {
    DWORD retval = connect(sock, (struct sockaddr*)(&server), sizeof(server));
    if (retval != 0)
        error((char*)"Connecting");
}

void Client::sendMessage(string msg) {
    const int length = msg.length();
    const char* str = msg.c_str();
    
    unsigned long n = send(sock, str, length, 0);

    if (n < length)
        error((char*)"Writing to socket");
}

void Client::receiveMessage() {
    unsigned long n = recv(sock, buffer, 1024, 0);
    if (n < 1) error((char*)"reading from socket");
    else {
        buffer[n] = '\0';
        printf((char*)"The message from the server is %s\n", buffer);
    }
}

void Client::closeSocket() {
    closesocket(sock);
}
