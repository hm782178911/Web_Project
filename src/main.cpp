#include "database.h"
#include "http_server.h"
#include "book.h"
#include "user.h"
#include <fstream>
#include <sstream>

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

int main() {
    // 初始化数据库连接
    Database db("localhost", "root", "password", "library");
    if (!db.connect()) {
        std::cerr << "Failed to connect to database" << std::endl;
        return 1;
    }
    
    // 初始化管理器
    BookManager bookManager(db);
    UserManager userManager(db);
    
    // 创建HTTP服务器
    HttpServer server(8080);
    
    // 添加路由
    server.addRoute("/", [](const std::map<std::string, std::string>&) {
        std::string content = readFile("web/index.html");
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + content;
    });
    
    server.addRoute("/books", [&bookManager](const std::map<std::string, std::string>&) {
        auto books = bookManager.getAllBooks();
        
        std::ostringstream oss;
        oss << R"(<html><head><link rel="stylesheet" href="/style.css"></head><body>)";
        oss << R"(<h1>Book List</h1><table border="1"><tr><th>ID</th><th>Title</th><th>Author</th><th>ISBN</th><th>Available</th></tr>)";
        
        for (const auto& book : books) {
            oss << "<tr>"
                << "<td>" << book.at("id") << "</td>"
                << "<td>" << book.at("title") << "</td>"
                << "<td>" << book.at("author") << "</td>"
                << "<td>" << book.at("isbn") << "</td>"
                << "<td>" << book.at("available") << "/" << book.at("quantity") << "</td>"
                << "</tr>";
        }
        
        oss << "</table></body></html>";
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + oss.str();
    });
    
    server.addRoute("/style.css", [](const std::map<std::string, std::string>&) {
        std::string content = readFile("web/style.css");
        return "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n" + content;
    });
    
    // 启动服务器
    server.start();
    
    return 0;
}