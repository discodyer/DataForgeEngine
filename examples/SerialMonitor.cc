#include <iostream>
#include <thread>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include "SerialPort.h"

// 终端设置为原始模式，禁用回显
void setTerminal(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        // 获取当前终端属性
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        // 修改终端设置
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        // 恢复终端设置
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

// 读取并发送用户输入
void readAndSendInput(SerialBase& serial) {
    char ch;
    while (true) {
        ch = getchar();
        if (!serial.writeByte(ch)) {
            std::cerr << "Failed to write byte to serial port." << std::endl;
        }
    }
}

// 读取并打印从串口接收到的数据
void readAndPrintOutput(SerialBase& serial) {
    uint8_t data;
    while (true) {
        if (serial.readByte(data, 100)) { // 假设100ms超时
            std::cout << data << std::flush;
        }
    }
}

int main() {
    SerialBase serial("/dev/ttyUSB0", 9600); // 例子: 使用USB0串口和9600波特率
    if (!serial.open()) {
        std::cerr << "Failed to open serial port." << std::endl;
        return 1;
    }

    setTerminal(true); // 禁用终端回显

    // 创建一个线程来处理串口的输入输出
    std::thread inputThread(readAndSendInput, std::ref(serial));
    std::thread outputThread(readAndPrintOutput, std::ref(serial));

    // 等待线程完成（在这个例子中，这实际上不会发生，除非你添加了退出条件）
    inputThread.join();
    outputThread.join();

    setTerminal(false); // 恢复终端设置

    return 0;
}
