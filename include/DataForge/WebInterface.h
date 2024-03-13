#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <string>
#include <curl/curl.h>

class WebInterface {
public:
    WebInterface();

    ~WebInterface();

    // 发送 HTTP POST 请求
    std::string sendPostRequest(const std::string& url, const std::string& postData);

private:
    // 回调函数用于接收服务器响应数据
    static size_t writeCallback(void* ptr, size_t size, size_t nmemb, std::string* data);
};

#endif // WEBINTERFACE_H
