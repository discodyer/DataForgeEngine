#include "HttpClient.h"

HttpClient::HttpClient() {
    curlpp::initialize();
}

HttpClient::~HttpClient() {
    curlpp::terminate();
}

std::string HttpClient::get(const std::string& url, size_t timeout) {
    try {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        using namespace curlpp::Options;
        std::ostringstream responseStream;
        request.setOpt(new Url(url)); // 设置请求的 URL
        // 设置连接的超时时间，单位是秒
        request.setOpt(new curlpp::options::ConnectTimeout(timeout));
        // 设置总的请求超时时间，单位是秒
        request.setOpt(new curlpp::options::Timeout(timeout));
        // Url url_;
        // request.getOpt(url_);
        // std::cout<<"Url:"<<url_.getValue()<<std::endl;
        request.setOpt<WriteStream>(&responseStream);
        request.perform(); // 执行 HTTP GET 请求
        // 获取 HTTP 状态码
        // long responseCode = curlpp::infos::ResponseCode::get(request);
        // 获取响应内容
        // std::string responseBody = responseStream.str();
        return responseStream.str();
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
        using namespace curlpp::Options;
        request.setOpt<Url>(url);
        request.setOpt<PostFields>(postData);
        request.setOpt<PostFieldSize>(postData.length());
        request.setOpt<WriteStream>(&response);
        request.perform();
        return response.str();
    } catch (curlpp::RuntimeError &e) {
        return e.what();
    } catch (curlpp::LogicError &e) {
        return e.what();
    }
    return "";
}
