#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <mutex>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    typedef int socklen_t;
    #define close closesocket
#else
    #include <unistd.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

using namespace std;

const int PORT = 8080;
const int BACKLOG = 10;
const string ROOT_DIR = "./www";

void handleClient(int clientSocket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        istringstream request(buffer);
        string method, path, version;
        request >> method >> path >> version;

        if (method == "GET") {
            string filePath = ROOT_DIR + path;
            if (filePath == ROOT_DIR + "/") {
                filePath += "index.html";
            }

            ifstream file(filePath);
            if (file.good()) {
                stringstream response;
                response << "HTTP/1.1 200 OK\r\n";
                response << "Content-Type: text/html\r\n";
                response << "Connection: close\r\n\r\n";
                response << file.rdbuf();

                string responseStr = response.str();
                send(clientSocket, responseStr.c_str(), responseStr.size(), 0);
            } else {
                string response = "HTTP/1.1 404 Not Found\r\n\r\n";
                send(clientSocket, response.c_str(), response.size(), 0);
            }
        }
    }

    close(clientSocket);
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }
#endif

    int serverSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Failed to create socket" << endl;
        return 1;
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&opt), sizeof(opt)) == -1) {
        cerr << "Failed to set socket options" << endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        cerr << "Failed to bind socket" << endl;
        return 1;
    }

    if (listen(serverSocket, BACKLOG) == -1) {
        cerr << "Failed to listen on socket" << endl;
        return 1;
    }

    cout << "Server is listening on port " << PORT << endl;

    while (true) {
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
        if (clientSocket == -1) {
            cerr << "Failed to accept client connection" << endl;
            continue;
        }

        // Use a lambda function to ensure the client socket is properly passed to the thread
        thread([](int sock) { handleClient(sock); }, clientSocket).detach();
    }

    close(serverSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
