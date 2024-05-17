#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <iostream>
#include <string>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/PartHandler.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include "HttpClient.h"
#include "SerialPort.h"
#include "MqttClient.h"
#include "cJSON.h"
#include <Poco/StreamCopier.h>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco;

// 返回网页主页回调函数
class GetPageRequestHandler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override ;
    explicit GetPageRequestHandler(const std::string& filePath);
private:
    std::string _filePath;
};

// HTTP 服务器主函数
class HTTPServerApp : public ServerApplication
{
protected:
    int main(const std::vector<std::string>& args) override ;
};

class PostRequestHandler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) override;
    std::string fetchDataFromHttpGet(const std::string& url);
    std::string fetchDataFromHttpPost(const std::string& url, const std::string& payload);
    std::string fetchDataFromMqtt(const std::string &url, int port, const std::string &client_name, const std::string &topic, size_t timeout);
    std::string publishDataToMqtt(const std::string &url, int port, const std::string &client_name, const std::string &topic, const std::string &message);
    std::string convJsonToMqtt(const std::string &payload);
    std::string fetchDataFromSerialPort(const std::string& payload);
};

class DataEngineRequestHandler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) override;
};

// 请求处理
class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
    HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request) override;
};

class WebInterface : public ServerApplication
{
protected:
    int main(const std::vector<std::string> &) override;
};

class FilePartHandler : public PartHandler {
private:
    std::string _data;

public:
    void handlePart(const MessageHeader& header, std::istream& stream) override {
        if (header.get("Content-Disposition").find("filename=") != std::string::npos) {
            // 说明这部分是一个文件
            std::ostringstream ss;
            StreamCopier::copyStream(stream, ss);
            _data = ss.str();
        }
    }

    const std::string& data() const {
        return _data;
    }
};

#endif // WEBINTERFACE_H
