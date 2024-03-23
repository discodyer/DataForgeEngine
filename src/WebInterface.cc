#include "WebInterface.h"

void PostRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    // 添加CORS头部以允许所有来源的请求
    response.set("Access-Control-Allow-Origin", "*");
    response.set("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    response.set("Access-Control-Allow-Headers", "X-Requested-With, Content-Type");

    if (request.getMethod() == HTTPRequest::HTTP_OPTIONS)
    {
        // 对于OPTIONS预检请求，仅设置必要的头部并返回
        response.setStatus(HTTPResponse::HTTP_OK);
        return;
    }

    if (request.getMethod() == HTTPRequest::HTTP_POST)
    {
        if (request.getContentType().find("multipart/form-data") == std::string::npos)
        {
            // 处理错误的Content-Type
            response.setStatus(Poco::Net::HTTPResponse::HTTP_UNSUPPORTED_MEDIA_TYPE);
            response.send() << "Unsupported Media Type. Please use 'multipart/form-data'.";
            return;
        }
        Poco::Net::HTMLForm form(request, request.stream());
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("text/plain");
        if (form.has("protocol") && form.has("data"))
        {
            std::string protocolValue = form.get("protocol");
            std::string receivedData = form.get("data");
            std::cout << "Received protocol: " << protocolValue << std::endl;
            std::cout << "Received Data: " << receivedData << std::endl;
            std::ostream &out = response.send();
            if (protocolValue == "http")
            {
                out << fetchDataFromHttpGet(receivedData);
            }
            else if (protocolValue == "mqtt")
            {
                // out << "Mqtt:";
                out << convJsonToMqtt(receivedData);
            }
            else if (protocolValue == "serial")
            {
                out << "Serial:";
            }
            else
            {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                out << "Protocol is invalid!";
                return;
            }
        }
        else
        {
            response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            response.send() << "Protocol field(s) is missing.";
        }

        // response.setStatus(HTTPResponse::HTTP_OK);
        // response.setContentType("text/plain");
        // std::ostream &out = response.send();
        // out << "Message received successfully!";
    }
    else
    {
        response.setStatus(HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
        response.send() << "Only POST method is supported.";
    }
}

std::string PostRequestHandler::fetchDataFromHttpGet(const std::string &url)
{
    HttpClient client;
    return client.get(url, 1);
}

std::string PostRequestHandler::publishDataToMqtt(const std::string &url, int port, const std::string &client_name, const std::string &topic, const std::string &message)
{
    MqttClient client(client_name);

    client.connect(url, port, 60);

    if (client.isConnected())
    {
        std::cout << "Connected to MQTT broker." << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to MQTT broker." << std::endl;
        return "Failed to connect to MQTT broker.";
    }

    // 简单测试发布和订阅功能
    client.subscribe(topic, 0);
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待订阅完成
    client.publish(topic, message, 0, false);

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待消息发布

    client.disconnect();
    return "Message sent to MQTT broker.";
}

std::string PostRequestHandler::convJsonToMqtt(const std::string &payload)
{
    cJSON *json = cJSON_Parse(payload.c_str());
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return "Reading payload failed!";
    }

    const cJSON *url = cJSON_GetObjectItemCaseSensitive(json, "url");
    const cJSON *port = cJSON_GetObjectItemCaseSensitive(json, "port");
    const cJSON *client_name = cJSON_GetObjectItemCaseSensitive(json, "client_name");
    const cJSON *topic = cJSON_GetObjectItemCaseSensitive(json, "topic");
    const cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (cJSON_IsString(url) && (url->valuestring != NULL) &&
        cJSON_IsNumber(port) &&
        cJSON_IsString(client_name) && (client_name->valuestring != NULL) &&
        cJSON_IsString(topic) && (topic->valuestring != NULL) &&
        cJSON_IsString(message) && (message->valuestring != NULL))
    {

        publishDataToMqtt(std::string(url->valuestring), port->valueint, std::string(client_name->valuestring), std::string(topic->valuestring), std::string(message->valuestring));
    }
    else
    {
        return "Data Formate Error!";
    }

    cJSON_Delete(json);
    return "Data sent to Target MQTT broker.";
}

std::string PostRequestHandler::fetchDataFromSerialPort(const std::string &payload)
{
    return std::string();
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
