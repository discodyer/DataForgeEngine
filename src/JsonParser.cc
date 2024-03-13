#include "JsonParser.h"

JsonParser::JsonParser() {}

JsonParser::~JsonParser() {}

cJSON* JsonParser::parseJson(const std::string& content) {
    cJSON* json = cJSON_Parse(content.c_str());
    return json;
}

std::vector<std::string> JsonParser::extractSteps(const std::string& jsonContent) {
    std::vector<std::string> steps;
    cJSON* json = parseJson(jsonContent);
    if (json == nullptr) {
        return steps;
    }

    cJSON* stepsJson = cJSON_GetObjectItemCaseSensitive(json, "steps");
    if (!cJSON_IsArray(stepsJson)) {
        cJSON_Delete(json);
        return steps;
    }

    for (cJSON* step = stepsJson->child; step != nullptr; step = step->next) {
        if (cJSON_IsObject(step)) {
            cJSON* code = cJSON_GetObjectItemCaseSensitive(step, "code");
            if (cJSON_IsString(code) && (code->valuestring != nullptr)) {
                steps.push_back(std::string(code->valuestring));
            }
        }
    }

    cJSON_Delete(json);
    return steps;
}
