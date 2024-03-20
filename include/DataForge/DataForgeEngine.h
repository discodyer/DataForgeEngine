#ifndef DATAFORGEENGINE_H
#define DATAFORGEENGINE_H

#include "HttpClient.h"
#include "MqttClient.h"
#include "JsonParser.h"
#include "SerialPort.h"
#include "DatabaseManager.h"

class DataForgeEngine {
public:
    DataForgeEngine();
    ~DataForgeEngine();

    void processData();

private:
    HttpClient httpClient;
    MqttClient mqttClient;
    JsonParser jsonParser;
    SerialPortWrapper serialPort;
    DatabaseManager databaseManager;

    void fetchDataFromHttp();
    void fetchDataFromMqtt();
    void fetchDataFromSerialPort();
    void storeDataInDatabase(const std::string& data);
    std::string parseData(const std::string& rawData);
};

#endif // DATAFORGEENGINE_H