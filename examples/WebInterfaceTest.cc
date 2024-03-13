#include "WebInterface.h"
#include <iostream>

// 自定义简单单元测试函数
void runTest(const std::string& testName, bool condition) {
    std::cout << "Test \"" << testName << "\": ";
    if (condition) {
        std::cout << "PASSED" << std::endl;
    } else {
        std::cout << "FAILED" << std::endl;
    }
}

int main() {
    // 创建 WebInterface 对象
    WebInterface webInterface;

    // 准备测试数据
    std::string url = "http://localhost:8080";
    std::string postData = "Some data to send";

    // 发送 HTTP POST 请求
    std::string response = webInterface.sendPostRequest(url, postData);

    // 验证响应是否包含预期内容
    bool testPassed = response.find("Operation performed successfully") != std::string::npos;
    runTest("SendPostRequest", testPassed);

    return 0;
}
