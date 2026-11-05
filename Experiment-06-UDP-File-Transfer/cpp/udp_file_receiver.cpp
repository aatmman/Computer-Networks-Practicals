#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9100;
const int CHUNK_SIZE = 1024;
const char* END_MARKER = "<<EOF>>";

int main(int argc, char* argv[]) {
    string outputFilename = "received_output";
    if (argc > 1) {
        outputFilename = argv[1];
    }

    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    int opt = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (::bind(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockFd);
        return 1;
    }

    cout << "========================================================\n";
    cout << "  UDP FILE RECEIVER LISTENING ON PORT " << PORT << "\n";
    cout << "========================================================\n";
    cout << "Output will be saved to: \"" << outputFilename << "\"\n";
    cout << "Waiting for incoming datagrams...\n";

    ofstream outFile(outputFilename, ios::binary);
    if (!outFile) {
        cerr << "Failed to open output file: " << outputFilename << endl;
        close(sockFd);
        return 1;
    }

    char buffer[CHUNK_SIZE];
    sockaddr_in senderAddr{};
    socklen_t senderLen = sizeof(senderAddr);
    long long totalBytesReceived = 0;
    int chunkCount = 0;

    while (true) {
        ssize_t n = recvfrom(sockFd, buffer, CHUNK_SIZE, 0,
                             (sockaddr*)&senderAddr, &senderLen);
        if (n < 0) {
            perror("recvfrom failed");
            break;
        }

        // Check if this datagram contains the end-of-file marker
        if (n == (ssize_t)strlen(END_MARKER) &&
            strncmp(buffer, END_MARKER, n) == 0) {
            cout << "\n[EOF Marker Received] File transfer complete.\n";
            break;
        }

        outFile.write(buffer, n);
        totalBytesReceived += n;
        chunkCount++;

        if (chunkCount % 50 == 0) {
            cout << "." << flush;
        }
    }

    char senderIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &senderAddr.sin_addr, senderIP, sizeof(senderIP));
    cout << "Received from " << senderIP << ":" << ntohs(senderAddr.sin_port) << endl;
    cout << "Total Chunks Received: " << chunkCount << endl;
    cout << "Total Bytes Saved    : " << totalBytesReceived << " bytes\n";

    outFile.close();
    close(sockFd);
    return 0;
}

