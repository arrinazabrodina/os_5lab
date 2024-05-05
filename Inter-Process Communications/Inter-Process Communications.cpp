// Inter-Process Communications.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <chrono>
#include <thread>
#include "server.hpp"
#include "client.hpp"

using namespace std;

void sleep(int seconds) {
    chrono::seconds timespan(seconds);
    this_thread::sleep_for(timespan);
}

int f(int x) {
    sleep(5);
    return x + 2;
}

int g(int x) {
    sleep(10);
    return x * 4;
}

void calculateF(int x, unsigned short port) {
    int result = f(x);
    Client client(port);
    client.sendMessage(to_string(result));
}

void calculateG(int x, unsigned short port) {
    int result = g(x);
    Client client(port);
    client.sendMessage(to_string(result));
}

//−1, false; 0, unknown; +1, true
int calculateResult(int* lhs, int* rhs) {
    // &&
    if (lhs == NULL || rhs == NULL)
        return 0;

    if (*lhs == 0 || *rhs == 0)
        return -1;

    return 1;
}

int main() {
    int x;
    cout << "Enter x: ";
    cin >> x;
    cout << "Start calculating" << endl;

    unsigned short portF = 1029;
    unsigned short portG = 1030;

    Server serverF(portF);
    Server serverG(portG);

    thread clientFThread(calculateF, x, portF);
    thread clientGThread(calculateG, x, portG);

    thread threadServerF(&Server::startListening, &serverF);
    thread threadServerG(&Server::startListening, &serverG);

    bool didFinishCalculating = false;
    bool shouldContinueWaiting = true;
    bool shouldContinueAsking = true;

    auto lastCheckTimestamp = chrono::high_resolution_clock::now();
    auto checksDuration = chrono::seconds{ 2 };

    while (!didFinishCalculating && shouldContinueWaiting) {
        if (serverF.result != NULL && serverG.result != NULL) {
            didFinishCalculating = true;
        }

        if (!shouldContinueAsking) { continue; }

        auto currentTime = chrono::high_resolution_clock::now();
        auto timePassed = chrono::duration_cast<chrono::seconds>(currentTime - lastCheckTimestamp);
        if (timePassed > checksDuration) {
            cout << endl << "1. Continue calculating values" << endl
                << "2. Stop and show current result" << endl
                << "3. Continue calculating values without stops" << endl
                << "Your input: ";
            int choice;
            cin >> choice;
            if (choice == 1) {
                lastCheckTimestamp = chrono::high_resolution_clock::now();
            }
            if (choice == 2) {
                shouldContinueWaiting = false;
            }
            else if (choice == 3) {
                shouldContinueAsking = false;
            }
        }
    }
    cout << endl;
    if (serverF.result != NULL)
        cout << "F(x)=" << *(serverF.result) << endl;
    else
        cout << "F(x)=unknown" << endl;
    if (serverG.result != NULL)
        cout << "G(x)=" << *(serverG.result) << endl;
    else
        cout << "G(x)=unknown" << endl;

    //−1, false; 0, unknown; +1, true
    int result = calculateResult(serverF.result, serverG.result);

    if (result == -1) {
        cout << "Result is false" << endl;
    }
    else if (result == 0) {
        cout << "Result is unknown" << endl;
    }
    if (result == 1) {
        cout << "Result is true" << endl;
    }

    exit(1);
    return 0;
}
