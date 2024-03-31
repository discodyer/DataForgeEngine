#include "SerialPort.h"
#include <libserial/SerialStream.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

LibSerial::SerialPort SerialBase::serial_port_;
bool SerialBase::is_open_ = false;

SerialBase::SerialBase(const std::string &port, int baud_rate)
    : port_(port), baud_rate_(baud_rate)
{
}

SerialBase::~SerialBase()
{
    close();
}

bool SerialBase::open()
{
    try
    {
        serial_port_.Open(port_);
        serial_port_.SetBaudRate(getBaudRate(baud_rate_));
        serial_port_.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        serial_port_.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        serial_port_.SetParity(LibSerial::Parity::PARITY_NONE);
        serial_port_.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

        if (!serial_port_.IsOpen())
        {
            return false;
        }
        std::cout << "Serial port " << port_.c_str() << " opened!" << std::endl;
        is_open_ = true;
        return true;
    }
    catch (const LibSerial::OpenFailed &e)
    {
        std::cerr << "Serial port open failed : " << e.what() << std::endl;
        return false;
    }
}

LibSerial::BaudRate SerialBase::getBaudRate(const int baud_rate) const
{
    using namespace LibSerial;
    switch (baud_rate)
    {
    case 50:
        return BaudRate::BAUD_50;
        break;

    case 75:
        return BaudRate::BAUD_75;
        break;

    case 110:
        return BaudRate::BAUD_110;
        break;

    case 134:
        return BaudRate::BAUD_134;
        break;

    case 150:
        return BaudRate::BAUD_150;
        break;

    case 200:
        return BaudRate::BAUD_200;
        break;

    case 300:
        return BaudRate::BAUD_300;
        break;

    case 600:
        return BaudRate::BAUD_600;
        break;

    case 1200:
        return BaudRate::BAUD_1200;
        break;

    case 1800:
        return BaudRate::BAUD_1800;
        break;

    case 2400:
        return BaudRate::BAUD_2400;
        break;

    case 4800:
        return BaudRate::BAUD_4800;
        break;

    case 9600:
        return BaudRate::BAUD_9600;
        break;

    case 19200:
        return BaudRate::BAUD_19200;
        break;

    case 38400:
        return BaudRate::BAUD_38400;
        break;

    case 57600:
        return BaudRate::BAUD_57600;
        break;

    case 115200:
        return BaudRate::BAUD_115200;
        break;

    case 230400:
        return BaudRate::BAUD_230400;
        break;

    case 460800:
        return BaudRate::BAUD_460800;
        break;

    case 500000:
        return BaudRate::BAUD_500000;
        break;

    case 576000:
        return BaudRate::BAUD_576000;
        break;

    case 921600:
        return BaudRate::BAUD_921600;
        break;

    case 1000000:
        return BaudRate::BAUD_1000000;
        break;

    case 1152000:
        return BaudRate::BAUD_1152000;
        break;

    case 1500000:
        return BaudRate::BAUD_1500000;
        break;

    default:
        return BaudRate::BAUD_115200;
        break;
    }
}

bool SerialBase::isOpen()
{
    return is_open_;
}

void SerialBase::close()
{
    if (is_open_)
    {
        serial_port_.Close();
        is_open_ = false;
    }
}

bool SerialBase::readByte(uint8_t &data, size_t timeout_ms)
{
    if (isOpen())
    {
        if (!serial_port_.IsDataAvailable())
        {
            return false;
        }
        try
        {
            serial_port_.ReadByte(data, timeout_ms);
            return true;
        }
        catch (const LibSerial::ReadTimeout &e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
    }
    std::cerr << "Serial port not open." << '\n';
    return false;
}

bool SerialBase::writeByte(const uint8_t &data)
{
    if (isOpen())
    {
        serial_port_.WriteByte(data);
        return true;
    }
    std::cerr << "Serial port not open." << '\n';
    return false;
}

bool SerialBase::write(const std::vector<uint8_t> &raw_data)
{
    serial_port_.Write(raw_data);
    return true;
}

bool SerialBase::write(const std::string &str)
{
    std::vector<uint8_t> raw_data(str.begin(), str.end());
    serial_port_.Write(raw_data);
    return true;
}

bool SerialBase::read(std::vector<uint8_t> &buffer, size_t buffer_size, size_t timeout_ms)
{
    if (isOpen())
    {
        try
        {
            serial_port_.Read(buffer, buffer_size, timeout_ms);
            return true;
        }
        catch (const LibSerial::ReadTimeout &e)
        {
            std::cerr << "Serial read timeout: " << e.what() << '\n';
            return false;
        }
    }
    std::cerr << "Serial port not open." << '\n';
    return false;
}