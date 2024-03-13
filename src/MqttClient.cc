#include "MqttClient.h"

MqttClient::MqttClient(const std::string& clientId) : connected(false) {
    mosquitto_lib_init();
    mosq = mosquitto_new(clientId.c_str(), true, nullptr);
}

MqttClient::~MqttClient() {
    if (connected) {
        mosquitto_disconnect(mosq);
    }
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void MqttClient::connect(const std::string& host, int port, int keepalive) {
    mosquitto_connect(mosq, host.c_str(), port, keepalive);
    connected = true;
}

void MqttClient::disconnect() {
    mosquitto_disconnect(mosq);
    connected = false;
}

bool MqttClient::isConnected() const {
    return connected;
}

void MqttClient::publish(const std::string& topic, const std::string& message, int qos, bool retain) {
    mosquitto_publish(mosq, nullptr, topic.c_str(), message.length(), message.c_str(), qos, retain);
}

void MqttClient::subscribe(const std::string& topic, int qos) {
    mosquitto_subscribe(mosq, nullptr, topic.c_str(), qos);
}
