#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9300;

struct ClientEntry {
    sockaddr_in addr;
    string name;
};

bool sameAddr(const sockaddr_in &a, const sockaddr_in &b) {
    return a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_port == b.sin_port;
}

int main(int argc, char* argv[]) {
    int port = PORT;
    if (argc > 1) {
        port = atoi(argv[1]);
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
    serverAddr.sin_port = htons(port);

    if (::bind(sockFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockFd);
        return 1;
    }

    cout << "========================================================\n";
    cout << "  UDP MULTIUSER CHAT RELAY SERVER LISTENING ON PORT " << port << "\n";
    cout << "========================================================\n";
    cout << "Waiting for client JOIN datagrams...\n";

    vector<ClientEntry> clients;
    char buffer[1024];
    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);

    while (true) {
        ssize_t n = recvfrom(sockFd, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&clientAddr, &clientLen);
        if (n <= 0) continue;
        buffer[n] = '\0';
        string msg(buffer);

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        int clientPort = ntohs(clientAddr.sin_port);

        // Check if message is a registration request: "JOIN:<username>" or "JOIN"
        if (msg.rfind("JOIN", 0) == 0) {
            string username = "Anonymous";
            if (msg.size() > 5 && msg[4] == ':') {
                username = msg.substr(5);
            }

            auto it = find_if(clients.begin(), clients.end(),
                              [&](const ClientEntry &c) { return sameAddr(c.addr, clientAddr); });

            if (it == clients.end()) {
                clients.push_back({clientAddr, username});
                cout << "[Registration] " << username << " registered from "
                     << clientIP << ":" << clientPort << " (Total: " << clients.size() << ")\n";

                string welcome = "[Server]: Welcome " + username + " to UDP Multiuser Chat!";
                sendto(sockFd, welcome.c_str(), welcome.size(), 0,
                       (sockaddr*)&clientAddr, sizeof(clientAddr));

                string joinNotice = "[Server Notice]: " + username + " joined the chat.";
                for (const auto &c : clients) {
                    if (!sameAddr(c.addr, clientAddr)) {
                        sendto(sockFd, joinNotice.c_str(), joinNotice.size(), 0,
                               (sockaddr*)&c.addr, sizeof(c.addr));
                    }
                }
            }
            continue;
        }

        // Check if message is exit
        if (msg == "exit" || msg == "LEAVE") {
            auto it = find_if(clients.begin(), clients.end(),
                              [&](const ClientEntry &c) { return sameAddr(c.addr, clientAddr); });
            if (it != clients.end()) {
                string leaverName = it->name;
                clients.erase(it);
                cout << "[Departure] " << leaverName << " has disconnected.\n";

                string leaveNotice = "[Server Notice]: " + leaverName + " left the chat.";
                for (const auto &c : clients) {
                    sendto(sockFd, leaveNotice.c_str(), leaveNotice.size(), 0,
                           (sockaddr*)&c.addr, sizeof(c.addr));
                }
            }
            continue;
        }

        // Standard chat message: lookup sender username
        string senderName = "Client";
        auto it = find_if(clients.begin(), clients.end(),
                          [&](const ClientEntry &c) { return sameAddr(c.addr, clientAddr); });
        if (it != clients.end()) {
            senderName = it->name;
        } else {
            // Unregistered client sending message -> auto register
            clients.push_back({clientAddr, "Guest" + to_string(clientPort)});
            senderName = "Guest" + to_string(clientPort);
        }

        string formattedMsg = "[" + senderName + "]: " + msg;
        cout << formattedMsg << endl;

        // Relay datagram to all other registered clients
        for (const auto &c : clients) {
            if (!sameAddr(c.addr, clientAddr)) {
                sendto(sockFd, formattedMsg.c_str(), formattedMsg.size(), 0,
                       (sockaddr*)&c.addr, sizeof(c.addr));
            }
        }
    }

    close(sockFd);
    return 0;
}
