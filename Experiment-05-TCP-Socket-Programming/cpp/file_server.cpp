#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9002;
const int BUFFER_SIZE = 4096;

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
    cout << "  TCP FILE SERVER LISTENING ON PORT " << PORT << "\n";
    cout << "========================================================\n";
    cout << "Waiting for file upload from client...\n";

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

    string outputFileName = "received_file.dat";
    ofstream outFile(outputFileName, ios::binary);
    if (!outFile) {
        cerr << "Failed to open destination file for writing.\n";
        close(clientFd);
        close(serverFd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    long long totalBytesReceived = 0;

    cout << "Receiving file stream..." << endl;
    while ((bytesRead = recv(clientFd, buffer, sizeof(buffer), 0)) > 0) {
        outFile.write(buffer, bytesRead);
        totalBytesReceived += bytesRead;
    }

    outFile.close();
    cout << "File reception complete! Saved as \"" << outputFileName << "\" (" << totalBytesReceived << " bytes).\n";

    close(clientFd);
    close(serverFd);
    return 0;
}

