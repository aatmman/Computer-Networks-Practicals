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

atomic<bool> running(true);

void receiveLoop(int sockFd) {
    char buffer[1024];
    while (running) {
        ssize_t n = recv(sockFd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            if (running) {
                cout << "\n[Notice] Peer disconnected. Press Enter to exit." << endl;
                running = false;
            }
            break;
        }
        buffer[n] = '\0';
        cout << "\r[Peer]: " << buffer << "\n[You]: " << flush;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage:\n";
        cout << "  Listen mode  : ./p2p_chat server [port]\n";
        cout << "  Connect mode : ./p2p_chat client [peer_ip] [port]\n";
        cout << "Default port is 9200.\n";
        return 1;
    }

    string mode = argv[1];
    int port = 9200;
    string peerIP = "127.0.0.1";
    int connFd = -1;

    cout << "========================================================\n";
    cout << "        TCP PEER-TO-PEER (P2P) CHAT - C++               \n";
    cout << "========================================================\n";

    if (mode == "server") {
        if (argc > 2) port = atoi(argv[2]);

        int listenFd = socket(AF_INET, SOCK_STREAM, 0);
        if (listenFd < 0) {
            perror("Socket creation failed");
            return 1;
        }

        int opt = 1;
        setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (::bind(listenFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Bind failed");
            close(listenFd);
            return 1;
        }

        if (listen(listenFd, 1) < 0) {
            perror("Listen failed");
            close(listenFd);
            return 1;
        }

        cout << "Listening on port " << port << "... Waiting for peer to connect.\n";
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        connFd = accept(listenFd, (sockaddr*)&clientAddr, &clientLen);
        if (connFd < 0) {
            perror("Accept failed");
            close(listenFd);
            return 1;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        cout << "Peer connected from " << clientIP << ":" << ntohs(clientAddr.sin_port) << endl;
        close(listenFd); // Original listen socket can be closed in P2P

    } else if (mode == "client") {
        if (argc > 2) peerIP = argv[2];
        if (argc > 3) port = atoi(argv[3]);

        connFd = socket(AF_INET, SOCK_STREAM, 0);
        if (connFd < 0) {
            perror("Socket creation failed");
            return 1;
        }

        sockaddr_in peerAddr{};
        peerAddr.sin_family = AF_INET;
        peerAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, peerIP.c_str(), &peerAddr.sin_addr) <= 0) {
            cerr << "Invalid peer IP address: " << peerIP << endl;
            close(connFd);
            return 1;
        }

        cout << "Connecting to peer at " << peerIP << ":" << port << "...\n";
        if (connect(connFd, (sockaddr*)&peerAddr, sizeof(peerAddr)) < 0) {
            perror("Connection failed");
            close(connFd);
            return 1;
        }
        cout << "Connected to peer successfully!\n";
    } else {
        cerr << "Unknown mode: " << mode << " (use 'server' or 'client')\n";
        return 1;
    }

    cout << "Chat session active. Type messages and press Enter. Type 'exit' to quit.\n";

    thread receiver(receiveLoop, connFd);

    string msg;
    while (running) {
        cout << "[You]: " << flush;
        if (!getline(cin, msg)) break;
        if (msg == "exit") {
            running = false;
            break;
        }
        if (!msg.empty() && running) {
            if (send(connFd, msg.c_str(), msg.size(), 0) < 0) {
                perror("Send failed");
                break;
            }
        }
    }

    running = false;
    close(connFd);
    if (receiver.joinable()) {
        receiver.join();
    }

    cout << "Chat ended.\n";
    return 0;
}

