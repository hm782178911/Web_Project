#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>
#include <map>
#include "database.h"

class BookManager {
public:
    BookManager(Database& db);
    
    bool addBook(const std::string& title, const std::string& author, 
                const std::string& isbn, int quantity);
    bool deleteBook(int id);
    std::vector<std::map<std::string, std::string>> getAllBooks();
    std::map<std::string, std::string> getBookById(int id);
    bool borrowBook(int book_id, int user_id);
    bool returnBook(int book_id, int user_id);
    
private:
    Database& db_;
};

#endif