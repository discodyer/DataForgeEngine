#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "HttpClient.h"
#include "MqttClient.h"
#include "JsonParser.h"
#include "SerialPort.h"
#include "DatabaseManager.h"
#include <string>

class DataProcessor {
public:
    DataProcessor();
    ~DataProcessor();

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

#endif // DATAPROCESSOR_H
