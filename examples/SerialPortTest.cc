#include "SerialPort.h"
#include <iostream>
#include <assert.h>

int main() {
    SerialBase serialPort("/dev/ttyUSB0", 9600); // 请根据实际情况调整串口名称和波特率
    serialPort.open();

    assert(serialPort.isOpen());

    // 写入数据到串口
    serialPort.write("Hello, SerialPort!\n");

    // 读取数据（这里假设有回显或有设备回应）
    // std::string response = serialPort.read();
    // std::cout << "Received response: " << response << std::endl;

    serialPort.close();
    assert(!serialPort.isOpen());

    return 0;
}
