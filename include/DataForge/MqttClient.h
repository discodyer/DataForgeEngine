#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <string>
#include <mosquitto.h>

class MqttClient {
public:
    MqttClient(const std::string& clientId);
    ~MqttClient();

    void connect(const std::string& host, int port, int keepalive);
    void disconnect();
    bool isConnected() const;

    void publish(const std::string& topic, const std::string& message, int qos = 0, bool retain = false);
    void subscribe(const std::string& topic, int qos = 0);

private:
    struct mosquitto* mosq;
    bool connected;
};

#endif // MQTTCLIENT_H
