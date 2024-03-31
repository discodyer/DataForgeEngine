#ifndef SERIALPORTWRAPPER_H
#define SERIALPORTWRAPPER_H

#include <libserial/SerialPort.h>
#include <string>

#include <string>
#include <libserial/SerialStream.h> // 请确保已安装 serial 库
#include <libserial/SerialPort.h>

class SerialBase
{
public:
    SerialBase(const std::string &port, int baud_rate);
    ~SerialBase();

    bool open();
    bool isOpen();
    void close();
    bool readByte(uint8_t &data, size_t timeout);
    bool writeByte(const uint8_t &data);
    bool write(const std::vector<uint8_t>& raw_data);
    bool write(const std::string &str);
    bool read(std::vector<uint8_t>& buffer, size_t buffer_size, size_t timeout_ms);

protected:
    static bool is_open_;

private:
    std::string port_;
    int baud_rate_;
    static LibSerial::SerialPort serial_port_;
    LibSerial::BaudRate getBaudRate(const int baud_rate) const;
};

#endif // SERIALPORTWRAPPER_H
