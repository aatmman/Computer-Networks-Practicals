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
    if (argc < 2) {
        cout << "Usage: ./udp_send <filename> [receiver_ip]\n";
        cout << "Example: ./udp_send sample.txt 127.0.0.1\n";
        return 1;
    }

    string fileName = argv[1];
    string receiverIP = argc > 2 ? argv[2] : "127.0.0.1";

    ifstream inFile(fileName, ios::binary);
    if (!inFile) {
        // Create sample file if it doesn't exist
        ofstream sample(fileName, ios::binary);
        sample << "#!/bin/bash\n";
        sample << "# Sample file transfer via UDP Datagrams\n";
        sample << "echo 'Hello from transferred UDP script file!'\n";
        sample.close();
        inFile.open(fileName, ios::binary);
        cout << "Created sample file: \"" << fileName << "\"\n";
    }

    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in receiverAddr{};
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, receiverIP.c_str(), &receiverAddr.sin_addr) <= 0) {
        cerr << "Invalid receiver IP address: " << receiverIP << endl;
        close(sockFd);
        return 1;
    }

    cout << "========================================================\n";
    cout << "  UDP FILE SENDER -> " << receiverIP << ":" << PORT << "\n";
    cout << "========================================================\n";
    cout << "Sending file: \"" << fileName << "\"...\n";

    char buffer[CHUNK_SIZE];
    long long totalBytesSent = 0;
    int chunkCount = 0;

    while (inFile.read(buffer, CHUNK_SIZE) || inFile.gcount() > 0) {
        streamsize bytesToSend = inFile.gcount();
        ssize_t sent = sendto(sockFd, buffer, bytesToSend, 0,
                              (sockaddr*)&receiverAddr, sizeof(receiverAddr));
        if (sent < 0) {
            perror("sendto failed");
            break;
        }

        totalBytesSent += sent;
        chunkCount++;

        // Pacing delay (500-1000 microseconds) to mitigate UDP datagram drops in socket buffer
        usleep(1000);

        if (chunkCount % 50 == 0) {
            cout << "." << flush;
        }
    }

    // Send the EOF terminator datagram multiple times (or with short delays) to ensure delivery
    for (int k = 0; k < 3; k++) {
        sendto(sockFd, END_MARKER, strlen(END_MARKER), 0,
               (sockaddr*)&receiverAddr, sizeof(receiverAddr));
        usleep(5000);
    }

    cout << "\nFile transfer finished successfully!\n";
    cout << "Total Chunks Sent : " << chunkCount << endl;
    cout << "Total Bytes Sent  : " << totalBytesSent << " bytes\n";

    inFile.close();
    close(sockFd);
    return 0;
}

