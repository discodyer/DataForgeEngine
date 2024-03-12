#include "HttpClient.h"

HttpClient::HttpClient() {
    curlpp::initialize();
}

HttpClient::~HttpClient() {
    curlpp::terminate();
}

std::string HttpClient::get(const std::string& url) {
    try {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        
        using namespace curlpp::Options;

        request.setOpt(new Url(url)); // 设置请求的 URL
        request.setOpt(new Verbose(false)); // 设置是否显示详细信息
        request.perform(); // 执行 HTTP GET 请求

        // 获取响应状态码
        long responseCode = curlpp::infos::ResponseCode::get(request);
        std::cout << "Response Code:" << responseCode << std::endl;

        // 获取响应内容
        std::ostringstream responseBody;
        responseBody << Url("http://example.com");
        std::cout << "Response body:" << responseBody.str() <<std::endl;
        
        return responseBody.str();

    } catch (curlpp::RuntimeError &e) {
        return e.what();
    } catch (curlpp::LogicError &e) {
        return e.what();
    }
    return "";
}

std::string HttpClient::post(const std::string& url, const std::string& postData) {
    try {
        std::ostringstream response;
        curlpp::Easy request;
        request.setOpt<curlpp::options::Url>(url);
        request.setOpt<curlpp::options::PostFields>(postData);
        request.setOpt<curlpp::options::PostFieldSize>(postData.length());
        request.setOpt<curlpp::options::WriteStream>(&response);
        request.perform();
        return response.str();
    } catch (curlpp::RuntimeError &e) {
        return e.what();
    } catch (curlpp::LogicError &e) {
        return e.what();
    }
    return "";
}
