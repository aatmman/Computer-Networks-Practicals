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
    sockaddr_in fromAddr{};
    socklen_t fromLen = sizeof(fromAddr);

    while (running) {
        ssize_t n = recvfrom(sockFd, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&fromAddr, &fromLen);
        if (n > 0) {
            buffer[n] = '\0';
            string msg(buffer);
            if (msg == "exit") {
                cout << "\n[Notice] Peer has left the chat. Press Enter to exit." << endl;
                running = false;
                break;
            }
            char senderIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &fromAddr.sin_addr, senderIP, sizeof(senderIP));
            cout << "\r[Peer " << senderIP << ":" << ntohs(fromAddr.sin_port) << "]: "
                 << buffer << "\n[You]: " << flush;
        } else if (n < 0 && running) {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: ./udp_p2p <local_port> <remote_port> [remote_ip]\n";
        cout << "Example (Terminal 1): ./udp_p2p 9250 9251 127.0.0.1\n";
        cout << "Example (Terminal 2): ./udp_p2p 9251 9250 127.0.0.1\n";
        return 1;
    }

    int localPort = atoi(argv[1]);
    int remotePort = atoi(argv[2]);
    string remoteIP = argc > 3 ? argv[3] : "127.0.0.1";

    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    int opt = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in localAddr{};
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(localPort);

    if (::bind(sockFd, (sockaddr*)&localAddr, sizeof(localAddr)) < 0) {
        perror("Bind failed");
        close(sockFd);
        return 1;
    }

    sockaddr_in remoteAddr{};
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(remotePort);
    if (inet_pton(AF_INET, remoteIP.c_str(), &remoteAddr.sin_addr) <= 0) {
        cerr << "Invalid remote IP address: " << remoteIP << endl;
        close(sockFd);
        return 1;
    }

    cout << "========================================================\n";
    cout << "         UDP PEER-TO-PEER (P2P) CHAT - C++              \n";
    cout << "========================================================\n";
    cout << "Bound to local port " << localPort << ".\n";
    cout << "Sending to remote peer at " << remoteIP << ":" << remotePort << ".\n";
    cout << "Type messages and press Enter. Type 'exit' to quit.\n";

    thread receiver(receiveLoop, sockFd);

    string msg;
    while (running) {
        cout << "[You]: " << flush;
        if (!getline(cin, msg)) break;
        if (msg == "exit") {
            sendto(sockFd, "exit", 4, 0, (sockaddr*)&remoteAddr, sizeof(remoteAddr));
            running = false;
            break;
        }
        if (!msg.empty() && running) {
            sendto(sockFd, msg.c_str(), msg.size(), 0,
                   (sockaddr*)&remoteAddr, sizeof(remoteAddr));
        }
    }

    running = false;
    close(sockFd);
    if (receiver.joinable()) {
        receiver.detach();
    }

    cout << "P2P chat ended.\n";
    return 0;
}
