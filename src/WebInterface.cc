#include "WebInterface.h"

void PostRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    // 添加CORS头部以允许所有来源的请求
    response.set("Access-Control-Allow-Origin", "*");
    response.set("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    response.set("Access-Control-Allow-Headers", "X-Requested-With, Content-Type");

    if (request.getMethod() == HTTPRequest::HTTP_OPTIONS) {
        // 对于OPTIONS预检请求，仅设置必要的头部并返回
        response.setStatus(HTTPResponse::HTTP_OK);
        return;
    }

    if (request.getMethod() == HTTPRequest::HTTP_POST)
    {
        std::istream &in = request.stream();
        std::string payload;
        std::getline(in, payload, '\0'); // 读取整个payload
        std::cout << "Received payload: " << payload << std::endl;

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setContentType("text/plain");
        std::ostream &out = response.send();
        out << "Message received successfully!";
    }
    else
    {
        response.setStatus(HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        response.send() << "Only POST method is supported.";
    }
}

HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request)
{

    return new PostRequestHandler();
}

int WebInterface::main(const vector<string> &)
{

    HTTPServer s(new RequestHandlerFactory, ServerSocket(8080), new HTTPServerParams);

    s.start();
    cout << "Server started on port 8080." << endl;
    waitForTerminationRequest(); // 等待终止请求
    s.stop();

    return Application::EXIT_OK;
}
