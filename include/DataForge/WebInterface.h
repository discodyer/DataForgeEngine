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

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

class PostRequestHandler : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) override;
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
