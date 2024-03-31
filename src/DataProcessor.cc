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

void StepManager::parse_steps(cJSON *steps, BaseStep *root)
{
    const cJSON *step = NULL;
    BaseStep *current_step = root;
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
            if (cJSON_IsArray(input))
            {
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
                auto next_step_ = new ConcatStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "formatNumber") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 1)
            {
                std::vector<std::any> input_;
                for (int i = 0; i < cJSON_GetArraySize(input); i++)
                {
                    cJSON *input_item = cJSON_GetArrayItem(input, i);
                    if (cJSON_IsNumber(input_item))
                    {
                        printf("Input %d: %d\n", i, input_item->valueint);
                        input_.push_back(int(input_item->valueint));
                    }
                }
                auto next_step_ = new FormatNumberStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "indexOf") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 2)
            {
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
                auto next_step_ = new IndexOfStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "lastIndexOf") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 2)
            {
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
                auto next_step_ = new LastIndexOfStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "length") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 1)
            {
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
                auto next_step_ = new LengthStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "replace") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 3)
            {
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
                auto next_step_ = new ReplaceStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "split") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 2)
            {
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
                auto next_step_ = new SplitStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "substring") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) == 3)
            {
                std::vector<std::any> input_;
                // for (int i = 0; i < cJSON_GetArraySize(input); i++)
                // {
                cJSON *input_item0 = cJSON_GetArrayItem(input, 0);
                cJSON *input_item1 = cJSON_GetArrayItem(input, 1);
                cJSON *input_item2 = cJSON_GetArrayItem(input, 2);
                // if (cJSON_IsString(input_item))
                //     {
                //         printf("Input %d: %d\n", i, input_item->valuestring);
                //         input_.push_back(std::string(input_item->valuestring));
                //     }
                // }
                if (cJSON_IsString(input_item0) && cJSON_IsNumber(input_item1) && cJSON_IsNumber(input_item2))
                {
                    printf("Input 0: %s\n", input_item0->valuestring);
                    printf("Input 1: %d\n", input_item1->valueint);
                    printf("Input 2: %d\n", input_item2->valueint);
                    input_.push_back(std::string(input_item0->valuestring));
                    input_.push_back(size_t(input_item1->valueint));
                    input_.push_back(size_t(input_item2->valueint));
                }
                auto next_step_ = new SubtringStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "toLower") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 1)
            {
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
                auto next_step_ = new ToLowerStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "toUpper") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 1)
            {
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
                auto next_step_ = new ToUpperStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "trim") == 0)
        {
            cJSON *input = cJSON_GetObjectItemCaseSensitive(step, "input");
            if (cJSON_IsArray(input) && cJSON_GetArraySize(input) <= 1)
            {
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
                auto next_step_ = new TrimStep(std::vector<std::any>());
                current_step->setNextStep(next_step_);
                next_step_->setLastStep(current_step);
                next_step_->setInputs(input_);
            }
        }
        else if (strcmp(step_name->valuestring, "echo") == 0)
        {
            auto next_step_ = new EchoStep(std::vector<std::any>());
            current_step->setNextStep(next_step_);
            next_step_->setLastStep(current_step);
        }
        else if (strcmp(step_name->valuestring, "end") == 0)
        {
            auto next_step_ = new EndStep(std::vector<std::any>());
            current_step->setNextStep(next_step_);
            next_step_->setLastStep(current_step);
        }
        else if (strcmp(step_name->valuestring, "ifelse") == 0)
        {
            auto ifelse_ = new IfElseStep(std::vector<std::any>());

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
                        std::vector<std::any> condition_;
                        condition_.push_back(std::string(condition->valuestring));
                        current_step->setNextStep(ifelse_);
                        ifelse_->setLastStep(current_step);
                        ifelse_->setConditions(condition_);
                    }
                }
            }

            // 解析 "steps_true" 数组
            cJSON *steps_true_ = cJSON_GetObjectItemCaseSensitive(step, "steps_true");
            if (cJSON_IsArray(steps_true_))
            {
                auto root_ = new RootStep();
                parse_steps(steps_true_, root_);
                ifelse_->setCondTrue(root_->getNextStep());
                root_->getNextStep()->setLastStep(ifelse_);
                delete(root_);
            }
            // 解析 "steps_false" 数组
            cJSON *steps_false_ = cJSON_GetObjectItemCaseSensitive(step, "steps_false");
            if (cJSON_IsArray(steps_false_))
            {
                auto root_ = new RootStep();
                parse_steps(steps_false_, root_);
                ifelse_->setCondFalse(root_->getNextStep());
                root_->getNextStep()->setLastStep(ifelse_);
                delete(root_);
            }
            return;
        }
        if (current_step->getNextStep() != nullptr)
        {
            current_step = current_step->getNextStep();
        }
    }
}
void StepManager::parse_steps(cJSON *steps)
{
    parse_steps(steps, &root_);
}
