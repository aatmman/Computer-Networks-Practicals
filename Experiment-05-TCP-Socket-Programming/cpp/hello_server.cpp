#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9001;

int main() {
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (::bind(serverFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverFd);
        return 1;
    }

    if (listen(serverFd, 5) < 0) {
        perror("Listen failed");
        close(serverFd);
        return 1;
    }

    cout << "========================================================\n";
    cout << "  TCP HELLO SERVER LISTENING ON PORT " << PORT << "\n";
    cout << "========================================================\n";
    cout << "Waiting for incoming client connection...\n";

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(serverFd, (sockaddr*)&clientAddr, &clientLen);
    if (clientFd < 0) {
        perror("Accept failed");
        close(serverFd);
        return 1;
    }

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
    cout << "Client connected from " << clientIP << ":" << ntohs(clientAddr.sin_port) << endl;

    char buffer[1024] = {0};
    ssize_t bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Client says: \"" << buffer << "\"" << endl;

        string reply = "Hello from TCP Server! Connection verified.";
        send(clientFd, reply.c_str(), reply.size(), 0);
        cout << "Sent reply: \"" << reply << "\"" << endl;
    }

    close(clientFd);
    close(serverFd);
    cout << "Server closed connection gracefully.\n";
    return 0;
}

