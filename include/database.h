#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <map>

class Database {
public:
    Database(const std::string& host, const std::string& user, 
             const std::string& password, const std::string& db);
    ~Database();
    
    bool connect();
    bool execute(const std::string& query);
    std::vector<std::map<std::string, std::string>> query(const std::string& query);
    
private:
    MYSQL* conn_;
    std::string host_;
    std::string user_;
    std::string password_;
    std::string db_;
};

#endif