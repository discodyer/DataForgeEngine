#include "DataProcessor.h"

int main() {
    // Testing the StringUtils functions
    std::string str1 = "Hello, ";
    std::string str2 = "world!";
    std::cout << "concat: " << StringUtils::concat(str1, str2) << std::endl;
    std::cout << "endsWith: " << StringUtils::endsWith("Hello, world!", "world!") << std::endl;
    std::cout << "formatNumber: " << StringUtils::formatNumber(123) << std::endl;
    std::cout << "GUID: " << StringUtils::guid() << std::endl;
    std::cout << "indexOf: " << StringUtils::indexOf("Hello, world!", "world") << std::endl;
    std::cout << "lastIndexOf: " << StringUtils::lastIndexOf("Hello, world!", "l") << std::endl;
    std::cout << "length: " << StringUtils::length("Hello, world!") << std::endl;
    std::cout << "replace: " << StringUtils::replace("Hello, world!", "world", "everyone") << std::endl;
    std::cout << "split: ";
    for (const auto& token : StringUtils::split("Hello, world!", ' ')) {
        std::cout << token << " ";
    }
    std::cout << std::endl;
    std::cout << "substring: " << StringUtils::substring("Hello, world!", 7, 5) << std::endl;
    std::cout << "toLower: " << StringUtils::toLower("Hello, WORLD!") << std::endl;
    std::cout << "toUpper: " << StringUtils::toUpper("Hello, world!") << std::endl;
    std::cout << "trim: " << StringUtils::trim("   Hello, world!   ") << std::endl;

    // Testing the ConversionUtils functions
    // std::stringstream ss("Hello, world!");
    // std::cout << "inputStreamToString: " << ConversionUtils::inputStreamToString(ss) << std::endl;

    // Testing the Utils functions
    // Utils::setHeader(/* parameters */);
    // Utils::setBody(/* parameters */);

    return 0;
}