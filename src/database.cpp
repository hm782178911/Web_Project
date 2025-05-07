#include "../include/database.h"
#include <iostream>

Database::Database(const std::string& host, const std::string& user, 
                   const std::string& password, const std::string& db)
    : host_(host), user_(user), password_(password), db_(db), conn_(nullptr) {}

Database::~Database() {
    if (conn_) {
        mysql_close(conn_);
    }
}

bool Database::connect() {
    conn_ = mysql_init(nullptr);
    if (!conn_) {
        std::cerr << "MySQL initialization failed" << std::endl;
        return false;
    }
    
    if (!mysql_real_connect(conn_, host_.c_str(), user_.c_str(), 
                          password_.c_str(), db_.c_str(), 0, nullptr, 0)) {
        std::cerr << "Connection failed: " << mysql_error(conn_) << std::endl;
        return false;
    }
    
    return true;
}

bool Database::execute(const std::string& query) {
    if (mysql_query(conn_, query.c_str())) {
        std::cerr << "Query failed: " << mysql_error(conn_) << std::endl;
        return false;
    }
    return true;
}

std::vector<std::map<std::string, std::string>> Database::query(const std::string& query) {
    std::vector<std::map<std::string, std::string>> result;
    
    if (mysql_query(conn_, query.c_str())) {
        std::cerr << "Query failed: " << mysql_error(conn_) << std::endl;
        return result;
    }
    
    MYSQL_RES* res = mysql_store_result(conn_);
    if (!res) {
        return result;
    }
    
    int num_fields = mysql_num_fields(res);
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        std::map<std::string, std::string> row_data;
        for (int i = 0; i < num_fields; ++i) {
            row_data[fields[i].name] = row[i] ? row[i] : "NULL";
        }
        result.push_back(row_data);
    }
    
    mysql_free_result(res);
    return result;
}