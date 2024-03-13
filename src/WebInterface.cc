#include "WebInterface.h"

WebInterface::WebInterface()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

WebInterface::~WebInterface()
{
    curl_global_cleanup();
}

// 发送 HTTP POST 请求
std::string WebInterface::sendPostRequest(const std::string &url, const std::string &postData)
{
    std::string responseData;

    CURL *curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            // 发送请求失败
            responseData = "Error: " + std::string(curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return responseData;
}

// 回调函数用于接收服务器响应数据
size_t WebInterface::writeCallback(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}
