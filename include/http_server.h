#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <functional>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>

class HttpServer {
public:
    using Handler = std::function<std::string(const std::map<std::string, std::string>&)>;
    
    HttpServer(int port);
    ~HttpServer();
    
    void start();
    void addRoute(const std::string& path, Handler handler);
    
private:
    void handleRequest(int client_socket);
    std::map<std::string, std::string> parseQuery(const std::string& query);
    
    int port_;
    int server_socket_;
    std::map<std::string, Handler> routes_;
};

#endif