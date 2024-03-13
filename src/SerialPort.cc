#include "SerialPort.h"

SerialPortWrapper::SerialPortWrapper(const std::string& portName, unsigned int baudRate)
    : portName(portName), baudRate(baudRate) {}

SerialPortWrapper::~SerialPortWrapper() {
    close();
}

void SerialPortWrapper::open() {
    if (!serialPort.IsOpen()) {
        serialPort.Open(portName);
        serialPort.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
    }
}

void SerialPortWrapper::close() {
    if (serialPort.IsOpen()) {
        serialPort.Close();
    }
}

bool SerialPortWrapper::isOpen() const {
    return serialPort.IsOpen();
}

void SerialPortWrapper::write(const std::string& data) {
    if (serialPort.IsOpen()) {
        serialPort.Write(data);
    }
}

std::string SerialPortWrapper::read() {
    std::string result;
    if (serialPort.IsOpen()) {
        serialPort.ReadLine(result);
    }
    return result;
}
