#include "../include/http_server.h"
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>

HttpServer::HttpServer(int port) : port_(port), server_socket_(-1) {}

HttpServer::~HttpServer() {
    if (server_socket_ != -1) {
        close(server_socket_);
    }
}

void HttpServer::addRoute(const std::string& path, Handler handler) {
    routes_[path] = handler;
}

void HttpServer::start() {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);
    
    if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_socket_, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Server started on port " << port_ << std::endl;
    
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            perror("accept");
            continue;
        }
        
        handleRequest(client_socket);
        close(client_socket);
    }
}

void HttpServer::handleRequest(int client_socket) {
    char buffer[4096] = {0};
    read(client_socket, buffer, 4095);
    
    std::string request(buffer);
    std::istringstream iss(request);
    std::string method, path, protocol;
    iss >> method >> path >> protocol;
    
    size_t query_pos = path.find('?');
    std::string route = query_pos != std::string::npos ? path.substr(0, query_pos) : path;
    std::string query = query_pos != std::string::npos ? path.substr(query_pos + 1) : "";
    
    auto params = parseQuery(query);
    std::string response;
    
    if (routes_.find(route) != routes_.end()) {
        response = routes_[route](params);
    } else {
        response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
    }
    
    send(client_socket, response.c_str(), response.size(), 0);
}

std::map<std::string, std::string> HttpServer::parseQuery(const std::string& query) {
    std::map<std::string, std::string> params;
    std::istringstream iss(query);
    std::string pair;
    
    while (std::getline(iss, pair, '&')) {
        size_t eq_pos = pair.find('=');
        if (eq_pos != std::string::npos) {
            std::string key = pair.substr(0, eq_pos);
            std::string value = pair.substr(eq_pos + 1);
            params[key] = value;
        }
    }
    
    return params;
}