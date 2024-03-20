#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>
#include <cJSON.h>

class JsonParser {
public:
    JsonParser();
    ~JsonParser();

    // 解析JSON字符串，提取特定结构
    std::vector<std::string> extractSteps(const std::string& jsonContent);

private:
    cJSON* parseJson(const std::string& content);
};

#endif // JSONPARSER_H
