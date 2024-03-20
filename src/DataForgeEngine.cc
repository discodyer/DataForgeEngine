#include "DataForgeEngine.h"

DataForgeEngine::DataForgeEngine()
    : mqttClient("clientId"), serialPort("/dev/ttyUSB0", 9600), databaseManager("data.db") {
}

DataForgeEngine::~DataForgeEngine() {
}

void DataForgeEngine::processData() {
    // 示范性地调用每个组件的功能
    fetchDataFromHttp();
    fetchDataFromMqtt();
    fetchDataFromSerialPort();
}

void DataForgeEngine::fetchDataFromHttp() {
    std::string data = httpClient.get("http://example.com/data");
    std::string parsedData = parseData(data);
    storeDataInDatabase(parsedData);
}

void DataForgeEngine::fetchDataFromMqtt() {
    // 假设已经设置好MQTT客户端和回调来接收消息
}

void DataForgeEngine::fetchDataFromSerialPort() {
    std::string data = serialPort.read();
    std::string parsedData = parseData(data);
    storeDataInDatabase(parsedData);
}

void DataForgeEngine::storeDataInDatabase(const std::string& data) {
    databaseManager.open();
    databaseManager.execute("INSERT INTO data (content) VALUES ('" + data + "');");
    databaseManager.close();
}

std::string DataForgeEngine::parseData(const std::string& rawData) {
    // 使用JsonParser或其他逻辑来解析数据
    return rawData; // 简化示例，实际中应进行解析
}
