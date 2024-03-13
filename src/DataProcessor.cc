#include "DataProcessor.h"

DataProcessor::DataProcessor()
    : mqttClient("clientId"), serialPort("/dev/ttyUSB0", 9600), databaseManager("data.db") {
}

DataProcessor::~DataProcessor() {
}

void DataProcessor::processData() {
    // 示范性地调用每个组件的功能
    fetchDataFromHttp();
    fetchDataFromMqtt();
    fetchDataFromSerialPort();
}

void DataProcessor::fetchDataFromHttp() {
    std::string data = httpClient.get("http://example.com/data");
    std::string parsedData = parseData(data);
    storeDataInDatabase(parsedData);
}

void DataProcessor::fetchDataFromMqtt() {
    // 假设已经设置好MQTT客户端和回调来接收消息
}

void DataProcessor::fetchDataFromSerialPort() {
    std::string data = serialPort.read();
    std::string parsedData = parseData(data);
    storeDataInDatabase(parsedData);
}

void DataProcessor::storeDataInDatabase(const std::string& data) {
    databaseManager.open();
    databaseManager.execute("INSERT INTO data (content) VALUES ('" + data + "');");
    databaseManager.close();
}

std::string DataProcessor::parseData(const std::string& rawData) {
    // 使用JsonParser或其他逻辑来解析数据
    return rawData; // 简化示例，实际中应进行解析
}
