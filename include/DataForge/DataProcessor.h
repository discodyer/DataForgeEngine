#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>

class StringUtils
{
public:
    // 拼接字符串函数
    static std::string concat(const std::string &str1, const std::string &str2);

    // 判断某个字符是否以指定的字符串结尾
    static bool endsWith(const std::string &str, const std::string &suffix);

    // 格式化数字函数
    static std::string formatNumber(int number);

    // 生成全局唯一标识
    static std::string guid();

    // 返回字符串的起始位置
    static size_t indexOf(const std::string &str, const std::string &substr);

    // 返回最后一次出现的字符串的起始位置
    static size_t lastIndexOf(const std::string &str, const std::string &substr);

    // 返回字符串或数组中的项数
    static size_t length(const std::string &str);

    // 字符串替换
    static std::string replace(const std::string &str, const std::string &oldStr, const std::string &newStr);

    // 分割字符串
    static std::vector<std::string> split(const std::string &str, char delimiter);

    // 字符串截取
    static std::string substring(const std::string &str, size_t start, size_t length);

    // 字符串转换成小写
    static std::string toLower(const std::string &str);

    // 字符串转换成大写
    static std::string toUpper(const std::string &str);

    // 去除前置和尾随空格
    static std::string trim(const std::string &str);

private:
    static unsigned char random_char();
    static std::string generate_hex(const unsigned int len);
};

class ConversionUtils
{
public:
    // inputStream流转为字符串
    static std::string inputStreamToString(std::istream &stream);

    // 将json参数转成xml
    static std::string jsonToXml(const std::string &json);

    // 对象转json字符串
    static std::string objectToJson(const std::string &object);

    // 对象转xml
    static std::string objectToXml(const std::string &object);

    // 其他转换函数...
};

class Utils
{
public:
    // 设置头部信息
    static void setHeader(/* 参数 */);

    // 设置消息体
    static void setBody(/* 参数 */);

    // 重置streamCache流读取位置
    static void resetStreamCache(/* 参数 */);

    // 重置inputStream流读取位置
    static void resetStream(/* 参数 */);

    // 获取contentType
    static std::string getContentType(/* 参数 */);

    // 获取content编码
    static std::string getContentEncoding(/* 参数 */);
};

#endif // DATAPROCESSOR_H
