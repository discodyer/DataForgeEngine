#include "cJSON.h"
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

int StepManager::buildStepTree(const std::string &jsonFilePath)
{
    root = std::make_shared<RootStep>();
    char *data = read_file(jsonFilePath.c_str());
    if (data == NULL)
        return EXIT_FAILURE;

    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        free(data);
        return EXIT_FAILURE;
    }

    // 解析 "steps" 数组
    cJSON *steps = cJSON_GetObjectItemCaseSensitive(json, "steps");
    if (cJSON_IsArray(steps))
    {
        parse_steps(steps);
    }

    cJSON_Delete(json);
    free(data);
    return EXIT_SUCCESS;
}

char *StepManager::read_file(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL)
    {
        perror("Unable to open file");
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = (char *)malloc(length + 1);
    if (data)
    {
        fread(data, 1, length, f);
        data[length] = '\0';
    }
    fclose(f);
    return data;
}

void StepManager::parse_steps(cJSON *steps, std::shared_ptr<BaseStep> &root)
{
    const cJSON *step = NULL;
    std::shared_ptr<BaseStep> current_step = root;
    cJSON_ArrayForEach(step, steps)
    {
        cJSON *step_name = cJSON_GetObjectItemCaseSensitive(step, "step_name");
        if (cJSON_IsString(step_name) && (step_name->valuestring != NULL))
        {
            printf("Step Name: %s\n", step_name->valuestring);
        }

        // 处理特定的步骤类型
        if (strcmp(step_name->valuestring, "concat") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) == 2)
            {
                std::shared_ptr<ConcatStep> childStep_ = std::make_shared<ConcatStep>(std::vector<std::any>());
                std::vector<std::any> input_;
                for (int i = 0; i < cJSON_GetArraySize(input); i++)
                {
                    cJSON *input_item = cJSON_GetArrayItem(input, i);
                    if (cJSON_IsString(input_item))
                    {
                        printf("Input %d: %s\n", i, input_item->valuestring);
                        input_.push_back(std::string(input_item->valuestring));
                    }
                }
                childStep_->setInputs(input_);
                current_step->addChild(childStep_);
            }
        }
        else if (strcmp(step_name->valuestring, "echo") == 0)
        {
            std::shared_ptr<EchoStep> childStep_ = std::make_shared<EchoStep>(std::vector<std::any>());
            current_step->addChild(childStep_);
        }
        else if (strcmp(step_name->valuestring, "ifelse") == 0)
        {
            // 示例：处理 ifelse 类型的步骤，类似地处理其他字段
            printf("This is an ifelse step.\n");
            cJSON *condition_name = cJSON_GetObjectItemCaseSensitive(step, "condition_name");
            if (cJSON_IsString(condition_name) && (condition_name->valuestring != NULL))
            {
                if (strcmp(condition_name->valuestring, "endsWith") == 0)
                {
                    cJSON *conditions = cJSON_GetObjectItemCaseSensitive(step, "conditions");
                    if (cJSON_IsArray(conditions) && cJSON_GetArraySize(conditions) == 1)
                    {
                        cJSON *condition = cJSON_GetArrayItem(conditions, 0);
                        // 创建 IfElseStep 对象
                        std::string conditionStr = condition->valuestring;
                        std::shared_ptr<IfElseStep> ifElseStep = std::make_shared<IfElseStep>(
                            std::vector<std::any>(), // 如果 IfElseStep 需要 inputs，可以在这里传递
                            [wp = std::weak_ptr<IfElseStep>(ifElseStep), conditionStr]() -> bool
                            {
                                if (auto sp = wp.lock())
                                { // Check if the weak_ptr can be promoted to shared_ptr
                                    try
                                    {
                                        // Attempt to use any_cast to cast to string
                                        std::string input = std::any_cast<std::string>(sp->getInput()[0]);
                                        return StringUtils::endsWith(input, conditionStr);
                                    }
                                    catch (const std::bad_any_cast &)
                                    {
                                        // Handle the case where the cast fails
                                        return false;
                                    }
                                }
                                return false; // If the weak_ptr cannot be promoted, return false
                            },
                            std::vector<std::shared_ptr<BaseStep>>());
                    }
                }
            }

            // 解析 "steps_true" 数组
            cJSON *steps_true_ = cJSON_GetObjectItemCaseSensitive(step, "steps_true");
            if (cJSON_IsArray(steps_true_))
            {
                parse_steps(steps_true_, current_step);
            }
            // 解析 "steps_false" 数组
            cJSON *steps_false_ = cJSON_GetObjectItemCaseSensitive(step, "steps_false");
            if (cJSON_IsArray(steps_false_))
            {
                parse_steps(steps_false_, current_step);
            }
        }
        if (current_step->has_next_step())
        {
            current_step = current_step->getNextStep();
        }
    }
}
void StepManager::parse_steps(cJSON *steps)
{
    parse_steps(steps, root);
}
