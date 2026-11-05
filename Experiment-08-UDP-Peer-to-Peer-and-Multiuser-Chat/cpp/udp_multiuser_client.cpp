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

const int PORT = 9300;
atomic<bool> running(true);

void receiveLoop(int sockFd) {
    char buffer[1024];
    sockaddr_in fromAddr{};
    socklen_t fromLen = sizeof(fromAddr);

    while (running) {
        ssize_t n = recvfrom(sockFd, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&fromAddr, &fromLen);
        if (n > 0) {
            buffer[n] = '\0';
            cout << "\r" << buffer << "\n[You]: " << flush;
        } else if (n < 0 && running) {
            break;
        }
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

    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
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

    cout << "========================================================\n";
    cout << "        UDP MULTIUSER CHAT CLIENT - C++                 \n";
    cout << "========================================================\n";
    cout << "Server target: " << serverIP << ":" << port << endl;

    string username;
    cout << "Enter your display username: ";
    getline(cin, username);
    if (username.empty()) username = "Anonymous";

    // Send JOIN datagram
    string joinPayload = "JOIN:" + username;
    sendto(sockFd, joinPayload.c_str(), joinPayload.size(), 0,
           (sockaddr*)&serverAddr, sizeof(serverAddr));

    cout << "Joined UDP chat session. Type messages and press Enter. Type 'exit' to quit.\n";

    thread receiver(receiveLoop, sockFd);

    string msg;
    while (running) {
        cout << "[You]: " << flush;
        if (!getline(cin, msg)) break;
        if (msg == "exit") {
            sendto(sockFd, "exit", 4, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
            running = false;
            break;
        }
        if (!msg.empty() && running) {
            sendto(sockFd, msg.c_str(), msg.size(), 0,
                   (sockaddr*)&serverAddr, sizeof(serverAddr));
        }
    }

    running = false;
    close(sockFd);
    if (receiver.joinable()) {
        receiver.detach();
    }

    cout << "You left the UDP chat.\n";
    return 0;
}
