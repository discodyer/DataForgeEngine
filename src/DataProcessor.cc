#include "DataProcessor.h"

std::string StringUtils::concat(const std::string &str1, const std::string &str2)
{
    return str1 + str2;
}

bool StringUtils::endsWith(const std::string &str, const std::string &suffix)
{
    if (str.length() >= suffix.length())
    {
        return (str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
    }
    return false;
}

std::string StringUtils::formatNumber(int number)
{
    return std::to_string(number);
}

std::string StringUtils::guid()
{
    std::stringstream ss;
    ss << generate_hex(4) << "-";
    ss << generate_hex(2) << "-";
    ss << generate_hex(2) << "-";
    ss << generate_hex(2) << "-";
    ss << generate_hex(6);
    return ss.str();
}

size_t StringUtils::indexOf(const std::string &str, const std::string &substr)
{
    return str.find(substr);
}

size_t StringUtils::lastIndexOf(const std::string &str, const std::string &substr)
{
    return str.rfind(substr);
}

size_t StringUtils::length(const std::string &str)
{
    return str.length();
}

std::string StringUtils::replace(const std::string &str, const std::string &oldStr, const std::string &newStr)
{
    size_t pos = 0;
    std::string result = str;
    while ((pos = result.find(oldStr, pos)) != std::string::npos)
    {
        result.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
    return result;
}

std::vector<std::string> StringUtils::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string StringUtils::substring(const std::string &str, size_t start, size_t length)
{
    return str.substr(start, length);
}

std::string StringUtils::toLower(const std::string &str)
{
    std::string lowerStr = str;
    for (char &c : lowerStr)
    {
        c = tolower(c);
    }
    return lowerStr;
}

std::string StringUtils::toUpper(const std::string &str)
{
    std::string upperStr = str;
    for (char &c : upperStr)
    {
        c = toupper(c);
    }
    return upperStr;
}

std::string StringUtils::trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == std::string::npos)
    {
        return "";
    }
    return str.substr(first, last - first + 1);
}

unsigned char StringUtils::random_char()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return static_cast<unsigned char>(dis(gen));
}

std::string StringUtils::generate_hex(const unsigned int len)
{
    std::stringstream ss;
    for (auto i = 0; i < len; i++)
    {
        auto rc = random_char();
        std::stringstream hexstream;
        hexstream << std::hex << int(rc);
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}
