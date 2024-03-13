#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& dbPath) : dbPath(dbPath), db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open() {
    if (sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK) {
        return true;
    }
    return false;
}

void DatabaseManager::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

std::string DatabaseManager::query(const std::string& sql) {
    sqlite3_stmt* stmt;
    std::string data = "";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return "";
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        data += reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        data += "\n";
    }

    sqlite3_finalize(stmt);
    return data;
}
