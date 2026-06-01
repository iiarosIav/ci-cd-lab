#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

std::atomic<int> request_counter(0);

void handle_client(int client_fd) {
    char buffer[4096] = {0};
    read(client_fd, buffer, 4096);
    
    std::string request(buffer);
    std::string response;
    
    if (request.find("GET / ") != std::string::npos || 
        request.find("GET / HTTP") != std::string::npos) {
        int count = request_counter++;
        std::stringstream ss;
        ss << "HTTP/1.1 200 OK\r\n";
        ss << "Content-Type: text/plain\r\n";
        ss << "Connection: close\r\n";
        ss << "\r\n";
        ss << "Hello from CI/CD #" << count;
        response = ss.str();
    }
    else if (request.find("GET /health") != std::string::npos) {
        response = "HTTP/1.1 200 OK\r\n"
                   "Content-Type: text/plain\r\n"
                   "Connection: close\r\n"
                   "\r\n"
                   "OK";
    }
    else if (request.find("GET /metrics") != std::string::npos) {
        std::stringstream ss;
        ss << "HTTP/1.1 200 OK\r\n";
        ss << "Content-Type: text/plain\r\n";
        ss << "Connection: close\r\n";
        ss << "\r\n";
        ss << "http_requests_total " << request_counter.load() << "\n";
        response = ss.str();
    }
    else {
        response = "HTTP/1.1 404 Not Found\r\n"
                   "Content-Type: text/plain\r\n"
                   "Connection: close\r\n"
                   "\r\n"
                   "Not Found";
    }
    
    send(client_fd, response.c_str(), response.size(), 0);
    close(client_fd);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    signal(SIGPIPE, SIG_IGN);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    std::cout << "Server listening on port 8080" << std::endl;
    
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        std::thread(handle_client, new_socket).detach();
    }
    
    return 0;
}