#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    bool open();
    void close();

    bool execute(const std::string& sql);
    std::string query(const std::string& sql);

private:
    std::string dbPath;
    sqlite3* db;
};

#endif // DATABASEMANAGER_H
