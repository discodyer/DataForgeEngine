#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <libserial/SerialPort.h>
#include <string>

class SerialPortWrapper {
public:
    SerialPortWrapper(const std::string& portName, unsigned int baudRate = 9600);
    ~SerialPortWrapper();

    void open();
    void close();
    bool isOpen() const;

    void write(const std::string& data);
    std::string read();

private:
    std::string portName;
    unsigned int baudRate;
    LibSerial::SerialPort serialPort;
};

#endif // SERIALPORT_H
