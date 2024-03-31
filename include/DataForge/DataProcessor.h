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
    std::string type;                                // 步骤类型
    std::vector<std::any> inputs;                    // 输入列表
    std::vector<std::any> output;                    // 输出列表
    std::vector<std::shared_ptr<BaseStep>> children; // 子步骤列表

public:
    BaseStep(const std::string &type, const std::vector<std::any> &inputs)
        : type(type), inputs(inputs) {}

    virtual ~BaseStep() {}

    // 添加子步骤
    void addChild(std::shared_ptr<BaseStep> child)
    {
        children.push_back(child);
    }

    // 设置输入
    void setInputs(const std::vector<std::any> &inputs)
    {
        this->inputs = inputs;
    }

    // 获取输出
    std::vector<std::any> getOutput() const
    {
        return output;
    }

    // 纯虚函数，执行步骤的具体操作
    virtual void execute() = 0;

    // 执行所有子步骤
    void executeChildren()
    {
        for (auto &child : children)
        {
            child->execute();
        }
    }

    // 获取步骤类型
    std::string getType() const
    {
        return type;
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
            output.push_back(StringUtils::concat(std::any_cast<std::string>(inputs[0]),
                                                 std::any_cast<std::string>(inputs[1])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::formatNumber(std::any_cast<int>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::indexOf(std::any_cast<std::string>(inputs[0]),
                                                  std::any_cast<std::string>(inputs[1])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::lastIndexOf(std::any_cast<std::string>(inputs[0]),
                                                      std::any_cast<std::string>(inputs[1])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::length(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::replace(std::any_cast<std::string>(inputs[0]),
                                                  std::any_cast<std::string>(inputs[1]),
                                                  std::any_cast<std::string>(inputs[2])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
                                              std::any_cast<char>(inputs[1]));
            // 将每个字符串作为 std::any 存储
            for (const auto &str : strings)
            {
                output.push_back(str);
            }
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::substring(std::any_cast<std::string>(inputs[0]),
                                                    std::any_cast<std::size_t>(inputs[1]),
                                                    std::any_cast<std::size_t>(inputs[2])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::toLower(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::toUpper(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
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
            output.push_back(StringUtils::trim(std::any_cast<std::string>(inputs[0])));
        }
        catch (std::bad_any_cast &e)
        {
            std::cerr << "EXCEPTION: " << e.what() << '\n';
        }
    }
};

class IfElseStep : public BaseStep
{
private:
    std::function<bool()> condition; // 条件判断函数

public:
    IfElseStep(const std::vector<std::any> &inputs, std::function<bool()> condition, const std::vector<std::shared_ptr<BaseStep>>& childSteps)
        : BaseStep("ifelse", inputs), condition(condition) {
            for (auto& child : childSteps) {
            children.push_back(child); // 添加子步骤到 children 列表中
        }
        }

    void execute() override
    {
        if (condition())
        {
            // 如果条件为真，执行第一个子节点
            if (!children.empty())
            {
                children[0]->execute();
            }
        }
        else
        {
            // 如果条件为假，执行第二个子节点
            if (children.size() > 1)
            {
                children[1]->execute();
            }
        }
    }
};

class StepManager
{
private:
    std::shared_ptr<BaseStep> root;

public:
    StepManager() : root(nullptr) {}

    // 从 JSON 文件构建步骤树的方法（伪代码，需要实现JSON解析和树的构建）
    void buildStepTree(const std::string &jsonFilePath)
    {
        // 解析 JSON 文件，构建步骤树
        // 这里需要实现具体的 JSON 解析逻辑
    }

    // 执行步骤树
    void execute()
    {
        if (root != nullptr)
        {
            root->execute();
        }
    }

    // 其他方法...
};

#endif // DATAPROCESSOR_H
