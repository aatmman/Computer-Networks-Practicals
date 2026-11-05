#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9003;

int main(int argc, char* argv[]) {
    string serverIP = "127.0.0.1";
    if (argc > 1) {
        serverIP = argv[1];
    }

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        cerr << "Invalid address: " << serverIP << endl;
        close(sockFd);
        return 1;
    }

    cout << "Connecting to TCP Calculator Server at " << serverIP << ":" << PORT << "..." << endl;
    if (connect(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(sockFd);
        return 1;
    }
    cout << "Connected!\n";

    string expr;
    cout << "Enter arithmetic expression (e.g., 25 + 17 or 100 / 4): ";
    getline(cin, expr);

    if (expr.empty()) {
        expr = "12 + 7";
        cout << "Defaulting to: " << expr << endl;
    }

    send(sockFd, expr.c_str(), expr.size(), 0);
    cout << "Sent: \"" << expr << "\"\n";

    char buffer[512] = {0};
    ssize_t bytesReceived = recv(sockFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Server Response: " << buffer << endl;
    }

    close(sockFd);
    return 0;
}

