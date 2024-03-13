#include "DataProcessor.h"
#include <iostream>

int main() {
    DataProcessor dataProcessor;
    
    // 由于DataProcessor的操作可能依赖于外部资源和具体实现细节，
    // 这里的“测试”主要是调用processData方法，确保没有异常发生。
    // 实际的单元测试可能需要更详细的模拟和验证。

    try {
        dataProcessor.processData();
        std::cout << "Data processing completed successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Data processing failed: " << e.what() << std::endl;
    }

    return 0;
}
