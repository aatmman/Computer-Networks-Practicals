#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9201;

struct ConnectedClient {
    int fd;
    string name;
    string ip;
    int port;
};

vector<ConnectedClient> clients;
mutex clientsMutex;

void broadcast(const string &msg, int excludeFd) {
    lock_guard<mutex> lock(clientsMutex);
    for (const auto &client : clients) {
        if (client.fd != excludeFd) {
            send(client.fd, msg.c_str(), msg.size(), 0);
        }
    }
}

void handleClient(int clientFd, string clientIP, int clientPort) {
    char buffer[1024];

    // Prompt client for display name
    string prompt = "Enter your username: ";
    send(clientFd, prompt.c_str(), prompt.size(), 0);

    ssize_t n = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0) {
        close(clientFd);
        return;
    }
    buffer[n] = '\0';
    string username(buffer);
    // Trim newline or spaces
    while (!username.empty() && (username.back() == '\r' || username.back() == '\n' || username.back() == ' ')) {
        username.pop_back();
    }
    if (username.empty()) username = "Client" + to_string(clientFd);

    {
        lock_guard<mutex> lock(clientsMutex);
        clients.push_back({clientFd, username, clientIP, clientPort});
    }

    string joinMsg = "[Server Notice]: " + username + " joined the chat.";
    cout << joinMsg << " (" << clientIP << ":" << clientPort << ")" << endl;
    broadcast(joinMsg, clientFd);

    string welcome = "[Server]: Welcome " + username + "! Type messages and press Enter. Type 'exit' to leave.\n";
    send(clientFd, welcome.c_str(), welcome.size(), 0);

    while (true) {
        n = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) break;
        buffer[n] = '\0';

        string msg(buffer);
        while (!msg.empty() && (msg.back() == '\r' || msg.back() == '\n')) {
            msg.pop_back();
        }
        if (msg == "exit") break;

        cout << "[" << username << "]: " << msg << endl;
        string broadcastMsg = "[" + username + "]: " + msg;
        broadcast(broadcastMsg, clientFd);
    }

    // Client disconnection cleanup
    {
        lock_guard<mutex> lock(clientsMutex);
        clients.erase(
            remove_if(clients.begin(), clients.end(),
                      [clientFd](const ConnectedClient &c) { return c.fd == clientFd; }),
            clients.end());
    }

    string leaveMsg = "[Server Notice]: " + username + " has left the chat.";
    cout << leaveMsg << endl;
    broadcast(leaveMsg, -1);

    close(clientFd);
}

int main(int argc, char* argv[]) {
    int port = PORT;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

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
    serverAddr.sin_port = htons(port);

    if (::bind(serverFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverFd);
        return 1;
    }

    if (listen(serverFd, 10) < 0) {
        perror("Listen failed");
        close(serverFd);
        return 1;
    }

    cout << "========================================================\n";
    cout << "  TCP MULTIUSER CHAT SERVER LISTENING ON PORT " << port << "\n";
    cout << "========================================================\n";
    cout << "Waiting for clients to join...\n";

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientFd = accept(serverFd, (sockaddr*)&clientAddr, &clientLen);
        if (clientFd < 0) {
            perror("Accept error");
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        int clientPort = ntohs(clientAddr.sin_port);

        thread clientThread(handleClient, clientFd, string(clientIP), clientPort);
        clientThread.detach();
    }

    close(serverFd);
    return 0;
}
