#include "JsonParser.h"
#include <iostream>
#include <assert.h>
#include <fstream>
#include <string>

// 读取 JSON 文件内容到字符串
std::string readJSONFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    return content;
}

int main()
{
    // 读取 JSON 文件
    std::string filename = "example.json";
    std::string jsonContent = readJSONFile(filename);

    if (jsonContent.empty())
    {
        std::cerr << "Error: JSON file is empty or could not be read." << std::endl;
        return 1;
    }

    // 使用 cJSON 解析 JSON 字符串
    cJSON *root = cJSON_Parse(jsonContent.c_str());
    if (!root)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != nullptr)
        {
            std::cerr << "Error: Failed to parse JSON: " << error_ptr << std::endl;
        }
        else
        {
            std::cerr << "Error: Failed to parse JSON." << std::endl;
        }
        return 1;
    }

    // 获取 "flows" 数组
    cJSON *flows = cJSON_GetObjectItemCaseSensitive(root, "flows");
    if (!cJSON_IsArray(flows))
    {
        std::cerr << "Error: 'flows' is not an array." << std::endl;
        cJSON_Delete(root);
        return 1;
    }

    // 遍历 "flows" 数组
    cJSON *flow;
    cJSON_ArrayForEach(flow, flows)
    {
        // 获取当前流程的 "steps" 数组
        cJSON *steps = cJSON_GetObjectItemCaseSensitive(flow, "steps");
        if (!cJSON_IsArray(steps))
        {
            std::cerr << "Error: 'steps' is not an array." << std::endl;
            continue;
        }

        // 打印解析后的 JSON 内容
        std::cout << "Parsed JSON:\n"
                  << cJSON_Print(steps) << std::endl;
    }
    
    // 释放 cJSON 对象
    cJSON_Delete(root);

    return 0;
}
