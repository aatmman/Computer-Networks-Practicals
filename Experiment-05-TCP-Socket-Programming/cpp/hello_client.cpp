#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9001;

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
        cerr << "Invalid address / Address not supported: " << serverIP << endl;
        close(sockFd);
        return 1;
    }

    cout << "Connecting to TCP Hello Server at " << serverIP << ":" << PORT << "..." << endl;
    if (connect(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(sockFd);
        return 1;
    }
    cout << "Connected successfully!\n";

    string msg = "Hello from TCP Client! Greetings.";
    send(sockFd, msg.c_str(), msg.size(), 0);
    cout << "Sent message: \"" << msg << "\"" << endl;

    char buffer[1024] = {0};
    ssize_t bytesReceived = recv(sockFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Server replied: \"" << buffer << "\"" << endl;
    }

    close(sockFd);
    cout << "Client closed socket.\n";
    return 0;
}
