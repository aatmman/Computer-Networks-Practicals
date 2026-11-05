#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int PORT = 9003;

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
    cout << "  TCP CALCULATOR SERVER LISTENING ON PORT " << PORT << "\n";
    cout << "========================================================\n";
    cout << "Waiting for calculation requests...\n";

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    int clientFd = accept(serverFd, (sockaddr*)&clientAddr, &clientLen);
    if (clientFd < 0) {
        perror("Accept failed");
        close(serverFd);
        return 1;
    }

    char buffer[512] = {0};
    ssize_t bytesReceived = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Received arithmetic expression: \"" << buffer << "\"" << endl;

        double a, b;
        char op;
        istringstream iss(buffer);
        string reply;

        if (iss >> a >> op >> b) {
            double result = 0;
            bool valid = true;
            switch (op) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/':
                    if (b != 0) {
                        result = a / b;
                    } else {
                        valid = false;
                        reply = "Error: Division by zero is undefined.";
                    }
                    break;
                default:
                    valid = false;
                    reply = "Error: Unsupported operator '" + string(1, op) + "'. Supported: +, -, *, /";
            }
            if (valid) {
                reply = "Result: " + to_string(result);
            }
        } else {
            reply = "Error: Invalid expression format. Expected '<num1> <op> <num2>' (e.g. 15 + 4)";
        }

        cout << "Computed reply: \"" << reply << "\"" << endl;
        send(clientFd, reply.c_str(), reply.size(), 0);
    }

    close(clientFd);
    close(serverFd);
    cout << "Calculation finished. Server closed connection.\n";
    return 0;
}

