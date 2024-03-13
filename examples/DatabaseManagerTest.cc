#include "DatabaseManager.h"
#include <iostream>
#include <assert.h>

int main() {
    DatabaseManager dbManager("test.db");

    assert(dbManager.open());

    // 创建表
    assert(dbManager.execute("CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);"));

    // 插入数据
    assert(dbManager.execute("INSERT INTO test (name) VALUES ('John Doe');"));

    // 查询数据
    std::string result = dbManager.query("SELECT name FROM test WHERE id = 1;");
    assert(result.find("John Doe") != std::string::npos);

    std::cout << "All tests passed." << std::endl;

    dbManager.close();

    return 0;
}
