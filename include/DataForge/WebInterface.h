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

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

class PostRequestHandler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) override;
    std::string fetchDataFromHttpGet(const std::string& url);
    std::string fetchDataFromHttpPost(const std::string& url, const std::string& payload);
    std::string publishDataToMqtt(const std::string &url, int port, const std::string &client_name, const std::string &topic, const std::string &message);
    std::string convJsonToMqtt(const std::string &payload);
    std::string fetchDataFromSerialPort(const std::string& payload);

};

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
    HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request) override;
};

class WebInterface : public ServerApplication
{
protected:
    int main(const vector<string> &) override;
};

#endif // WEBINTERFACE_H
