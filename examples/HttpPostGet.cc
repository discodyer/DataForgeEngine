#include "HttpClient.h"
#include <iostream>

int main()
{
    HttpClient client;
    const std::string getUrl = "http://google.com";
    const std::string postUrl = "http://google.com";
    const std::string postData = "key=value";

    std::cout << "GET Request:" << std::endl;
    std::cout << client.get("http://google.com") << std::endl;

    std::cout << "POST Request:" << std::endl;
    std::cout << client.post("http://google.com", postData) << std::endl;

    std::cout << "Hello World!1" << std::endl;
    return 0;
}
