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

int main(int argc, char* argv[]) {
    string fileName = "send_file.txt";
    string serverIP = "127.0.0.1";

    if (argc > 1) {
        fileName = argv[1];
    }
    if (argc > 2) {
        serverIP = argv[2];
    }

    ifstream inFile(fileName, ios::binary);
    if (!inFile) {
        // Create a default sample file if none exists
        ofstream sample(fileName);
        sample << "This is sample text transmitted over a reliable TCP socket byte stream.\n";
        sample << "TCP guarantees error-checked in-order delivery of all transmitted bytes.\n";
        sample.close();
        inFile.open(fileName, ios::binary);
        cout << "Created default sample file: \"" << fileName << "\"\n";
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

    cout << "Connecting to TCP File Server at " << serverIP << ":" << PORT << "..." << endl;
    if (connect(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(sockFd);
        return 1;
    }
    cout << "Connected! Sending file: \"" << fileName << "\"...\n";

    char buffer[BUFFER_SIZE];
    long long totalBytesSent = 0;

    while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount() > 0) {
        streamsize bytesToSend = inFile.gcount();
        ssize_t bytesSent = send(sockFd, buffer, bytesToSend, 0);
        if (bytesSent < 0) {
            perror("Send failed");
            break;
        }
        totalBytesSent += bytesSent;
    }

    inFile.close();
    cout << "File transfer complete! Successfully sent " << totalBytesSent << " bytes.\n";

    close(sockFd);
    return 0;
}
