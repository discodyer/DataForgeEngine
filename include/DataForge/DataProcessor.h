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
#include <memory>
#include <any>
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

class BaseStep
{
protected:
    std::string type;              // 步骤类型
    std::vector<std::any> inputs;  // 输入列表
    std::vector<std::any> outputs; // 输出列表
    BaseStep *last_step_;
    BaseStep *next_step_;

public:
    BaseStep(const std::string &type, std::vector<std::any> inputs)
        : type(type), inputs(inputs) {}

    BaseStep(const std::string &type)
        : type(type) {}

    virtual ~BaseStep() {}

    // 设置输入
    void setInputs(const std::vector<std::any> inputs)
    {
        for (auto input : inputs)
        {
            addInput(input);
        }
    }

    void addInput(const std::any input)
    {
        this->inputs.push_back(input);
    }

    // 获取输入
    std::vector<std::any> getInput() const
    {
        return inputs;
    }

    // 获取输出
    std::vector<std::any> getOutputs() const
    {
        return outputs;
    }

    // 纯虚函数，执行步骤的具体操作
    virtual void execute() = 0;

    // 获取步骤类型
    std::string getType() const
    {
        return type;
    }

    void setNextStep(BaseStep *next_step)
    {
        next_step_ = next_step;
    }

    void setLastStep(BaseStep *last_step)
    {
        last_step_ = last_step;
    }

    BaseStep *getNextStep()
    {
        return next_step_;
    }

    BaseStep *getLastStep()
    {
        return last_step_;
    }
};

class ConcatStep : public BaseStep
{
public:
    ConcatStep(const std::vector<std::any> &inputs)
        : BaseStep("concat", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            std::string result = std::any_cast<std::string>(inputs[0]);
            // outputs.push_back(StringUtils::concat(std::any_cast<std::string>(inputs[0]),
            //                                       std::any_cast<std::string>(inputs[1])));
            for (size_t i = 1; i < inputs.size(); ++i)
            {
                result = StringUtils::concat(result, std::any_cast<std::string>(inputs[i]));
            }
            outputs.push_back(result);
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class FormatNumberStep : public BaseStep
{
public:
    FormatNumberStep(const std::vector<std::any> &inputs)
        : BaseStep("formatNumber", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::formatNumber(std::any_cast<int>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class IndexOfStep : public BaseStep
{
public:
    IndexOfStep(const std::vector<std::any> &inputs)
        : BaseStep("indexOf", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::indexOf(std::any_cast<std::string>(inputs[0]),
                                                   std::any_cast<std::string>(inputs[1])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class LastIndexOfStep : public BaseStep
{
public:
    LastIndexOfStep(const std::vector<std::any> &inputs)
        : BaseStep("lastIndexOf", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::lastIndexOf(std::any_cast<std::string>(inputs[0]),
                                                       std::any_cast<std::string>(inputs[1])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class LengthStep : public BaseStep
{
public:
    LengthStep(const std::vector<std::any> &inputs)
        : BaseStep("length", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::length(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class ReplaceStep : public BaseStep
{
public:
    ReplaceStep(const std::vector<std::any> &inputs)
        : BaseStep("replace", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::replace(std::any_cast<std::string>(inputs[0]),
                                                   std::any_cast<std::string>(inputs[1]),
                                                   std::any_cast<std::string>(inputs[2])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class SplitStep : public BaseStep
{
public:
    SplitStep(const std::vector<std::any> &inputs)
        : BaseStep("split", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            auto strings = StringUtils::split(std::any_cast<std::string>(inputs[0]),
                                              (std::any_cast<std::string>(inputs[1])).c_str()[0]);
            // 将每个字符串作为 std::any 存储
            for (const auto &str : strings)
            {
                outputs.push_back(str);
            }
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class SubtringStep : public BaseStep
{
public:
    SubtringStep(const std::vector<std::any> &inputs)
        : BaseStep("substring", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::substring(std::any_cast<std::string>(inputs[0]),
                                                     std::any_cast<std::size_t>(inputs[1]),
                                                     std::any_cast<std::size_t>(inputs[2])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class ToLowerStep : public BaseStep
{
public:
    ToLowerStep(const std::vector<std::any> &inputs)
        : BaseStep("toLower", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::toLower(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class ToUpperStep : public BaseStep
{
public:
    ToUpperStep(const std::vector<std::any> &inputs)
        : BaseStep("toUpper", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::toUpper(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class TrimStep : public BaseStep
{
public:
    TrimStep(const std::vector<std::any> &inputs)
        : BaseStep("trim", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        try
        {
            outputs.push_back(StringUtils::trim(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
        getNextStep()->setInputs(getOutputs());
        getNextStep()->execute();
    }
};

class EchoStep : public BaseStep
{
public:
    EchoStep(const std::vector<std::any> &inputs)
        : BaseStep("echo", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        for (auto str : inputs)
        {
            std::cout << "Echo: " << std::any_cast<std::string>(str) << std::endl;
        }
        getNextStep()->setInputs(getLastStep()->getOutputs());
        getNextStep()->execute();
    }
};

class EndStep : public BaseStep
{
public:
    EndStep(const std::vector<std::any> &inputs)
        : BaseStep("end", inputs) {}

    // 实现具体的执行逻辑
    void execute() override
    {
        std::cout << "End." << std::endl;
    }
};

// class IfElseStep : public BaseStep
// {
// private:
//     std::function<bool()> condition; // 条件判断函数

// public:
//     IfElseStep(const std::vector<std::any> &inputs, std::function<bool()> condition, const std::vector<std::shared_ptr<BaseStep>> &childSteps)
//         : BaseStep("ifelse", inputs), condition(condition)
//     {
//         for (auto &child : childSteps)
//         {
//             // children.push_back(child); // 添加子步骤到 children 列表中
//         }
//     }

//     void execute() override
//     {
//         if (condition())
//         {
//             // 如果条件为真，执行第一个子节点
//             if (!children.empty())
//             {
//                 children[0]->setInputs(this->getInput());
//                 children[0]->execute();
//             }
//         }
//         else
//         {
//             // 如果条件为假，执行第二个子节点
//             if (children.size() > 1)
//             {
//                 children[1]->setInputs(this->getInput());
//                 children[1]->execute();
//             }
//         }
//     }
// };

class RootStep : public BaseStep
{
public:
    RootStep()
        : BaseStep("root")
    {
        next_step_ = nullptr;
        last_step_ = nullptr;
    }

    // 实现具体的执行逻辑
    void execute() override
    {
    }
};

class StepManager
{
private:
    RootStep root_;

    char *read_file(const char *filename);
    void parse_steps(cJSON *steps, BaseStep *root);
    void parse_steps(cJSON *steps);

public:
    StepManager() {}

    // 从 JSON 文件构建步骤树
    int buildStepTree(const std::string &jsonFilePath);

    // 执行步骤树
    void execute()
    {
        if (root_.getNextStep() == nullptr)
        {
            return;
        }

        root_.getNextStep()->execute();
    }
};

#endif // DATAPROCESSOR_H
