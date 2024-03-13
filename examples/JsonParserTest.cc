#include "JsonParser.h"
#include <iostream>
#include <assert.h>

int main() {
    JsonParser parser;
    std::string jsonContent = "{\"steps\":[{\"code\":\"Step1\"}, {\"code\":\"Step2\"}]}";
    auto steps = parser.extractSteps(jsonContent);

    assert(steps.size() == 2);
    assert(steps[0] == "Step1");
    assert(steps[1] == "Step2");

    std::cout << "Test passed. Extracted steps are: " << std::endl;
    for(const auto& step : steps) {
        std::cout << step << std::endl;
    }

    return 0;
}
