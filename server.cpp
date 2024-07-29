#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>

#define PORT 8080

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);

    // Simple parsing to extract the requested file path
    std::istringstream request(buffer);
    std::string method, path, version;
    request >> method >> path >> version;

    // Remove leading '/' from the path
    if (path[0] == '/') {
        path = path.substr(1);
    }

    // Default file
    if (path.empty()) {
        path = "index.html";
    }

    // Open the requested file
    std::ifstream file(path);
    if (file) {
        std::stringstream response;
        response << "HTTP/1.1 200 OK\nContent-Type: text/html\n\n" << file.rdbuf();
        std::string response_str = response.str();
        send(client_socket, response_str.c_str(), response_str.length(), 0);
    } else {
        const char *not_found = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: 9\n\nNot Found";
        send(client_socket, not_found, strlen(not_found), 0);
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    std::vector<std::thread> threads;

    while (true) {
        // Accept a new connection
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Create a new thread to handle the client connection
        threads.push_back(std::thread(handle_client, client_socket));
    }

    // Join threads (not necessary in this simple example)
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Close the server socket
    close(server_fd);

    return 0;
}
