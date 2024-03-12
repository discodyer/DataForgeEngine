#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <sstream>

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    std::string get(const std::string& url);
    std::string post(const std::string& url, const std::string& postData);
};

#endif // HTTPCLIENT_H
