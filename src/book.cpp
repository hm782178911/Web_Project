#include "../include/book.h"
#include <sstream>

BookManager::BookManager(Database& db) : db_(db) {}

bool BookManager::addBook(const std::string& title, const std::string& author, 
                         const std::string& isbn, int quantity) {
    std::ostringstream oss;
    oss << "INSERT INTO books (title, author, isbn, quantity, available) VALUES ('"
        << title << "', '" << author << "', '" << isbn << "', " << quantity << ", " << quantity << ")";
    return db_.execute(oss.str());
}

bool BookManager::deleteBook(int id) {
    std::ostringstream oss;
    oss << "DELETE FROM books WHERE id = " << id;
    return db_.execute(oss.str());
}

std::vector<std::map<std::string, std::string>> BookManager::getAllBooks() {
    return db_.query("SELECT * FROM books");
}

std::map<std::string, std::string> BookManager::getBookById(int id) {
    std::ostringstream oss;
    oss << "SELECT * FROM books WHERE id = " << id;
    auto result = db_.query(oss.str());
    return result.empty() ? std::map<std::string, std::string>() : result[0];
}

bool BookManager::borrowBook(int book_id, int user_id) {
    // 检查书籍是否可用
    auto book = getBookById(book_id);
    if (book.empty() || std::stoi(book["available"]) <= 0) {
        return false;
    }
    
    // 开始事务
    db_.execute("START TRANSACTION");
    
    // 更新书籍可用数量
    std::ostringstream oss1;
    oss1 << "UPDATE books SET available = available - 1 WHERE id = " << book_id;
    if (!db_.execute(oss1.str())) {
        db_.execute("ROLLBACK");
        return false;
    }
    
    // 创建借阅记录
    std::ostringstream oss2;
    oss2 << "INSERT INTO borrow_records (book_id, user_id, borrow_date) VALUES ("
         << book_id << ", " << user_id << ", CURDATE())";
    if (!db_.execute(oss2.str())) {
        db_.execute("ROLLBACK");
        return false;
    }
    
    db_.execute("COMMIT");
    return true;
}

bool BookManager::returnBook(int book_id, int user_id) {
    // 检查是否有借阅记录
    std::ostringstream oss1;
    oss1 << "SELECT id FROM borrow_records WHERE book_id = " << book_id 
         << " AND user_id = " << user_id << " AND return_date IS NULL";
    auto records = db_.query(oss1.str());
    if (records.empty()) {
        return false;
    }
    
    // 开始事务
    db_.execute("START TRANSACTION");
    
    // 更新书籍可用数量
    std::ostringstream oss2;
    oss2 << "UPDATE books SET available = available + 1 WHERE id = " << book_id;
    if (!db_.execute(oss2.str())) {
        db_.execute("ROLLBACK");
        return false;
    }
    
    // 更新借阅记录
    std::ostringstream oss3;
    oss3 << "UPDATE borrow_records SET return_date = CURDATE() WHERE id = " << records[0]["id"];
    if (!db_.execute(oss3.str())) {
        db_.execute("ROLLBACK");
        return false;
    }
    
    db_.execute("COMMIT");
    return true;
}