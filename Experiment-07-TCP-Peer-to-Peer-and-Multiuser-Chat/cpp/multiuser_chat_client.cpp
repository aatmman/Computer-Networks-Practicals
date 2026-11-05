#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9201;
atomic<bool> running(true);

void receiveLoop(int sockFd) {
    char buffer[1024];
    while (running) {
        ssize_t n = recv(sockFd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            if (running) {
                cout << "\n[Notice] Disconnected from multiuser server. Press Enter to exit." << endl;
                running = false;
            }
            break;
        }
        buffer[n] = '\0';
        cout << "\r" << buffer << "\n[You]: " << flush;
    }
}

int main(int argc, char* argv[]) {
    string serverIP = "127.0.0.1";
    int port = PORT;

    if (argc > 1) {
        serverIP = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        cerr << "Invalid server IP address: " << serverIP << endl;
        close(sockFd);
        return 1;
    }

    cout << "Connecting to Multiuser Chat Server at " << serverIP << ":" << port << "..." << endl;
    if (connect(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection to server failed");
        close(sockFd);
        return 1;
    }

    // Read username prompt from server
    char promptBuffer[256] = {0};
    recv(sockFd, promptBuffer, sizeof(promptBuffer) - 1, 0);
    cout << promptBuffer << flush;

    string username;
    getline(cin, username);
    if (username.empty()) username = "Anonymous";
    send(sockFd, username.c_str(), username.size(), 0);

    thread receiver(receiveLoop, sockFd);

    string msg;
    while (running) {
        if (!getline(cin, msg)) break;
        if (msg == "exit") {
            send(sockFd, "exit", 4, 0);
            running = false;
            break;
        }
        if (!msg.empty() && running) {
            if (send(sockFd, msg.c_str(), msg.size(), 0) < 0) {
                perror("Send failed");
                break;
            }
        }
    }

    running = false;
    close(sockFd);
    if (receiver.joinable()) {
        receiver.detach(); // detach so it does not block on exit
    }

    cout << "You have left the chat.\n";
    return 0;
}
