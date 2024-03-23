#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <string>
#include <mosquitto.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>
#include <mutex>
#include <condition_variable>

class MqttClient {
public:
    MqttClient(const std::string& clientId);
    ~MqttClient();

    int connect(const std::string& host, int port, int keepalive);
    void disconnect();
    bool isConnected() const;

    void publish(const std::string& topic, const std::string& message, int qos = 0, bool retain = false);
    void subscribe(const std::string& topic, int qos = 0);

    void doLoop(int timeout);

    std::string getLastPayload();
    
private:
    struct mosquitto* mosq;
    bool connected;
    bool running;
    static bool received;
    static std::string lastMessage; // 用于存储最近一次接收到的消息
    
    /* 当客户端从代理接收到CONNACK消息时调用回调。 */
    static void on_connect(struct mosquitto *mosq, void *obj, int reason_code);
    /* 当代理在响应订阅发送SUBACK时调用回调。 */
    static void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos);
    /* 当客户端收到消息时调用回调。 */
    static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
};

#endif // MQTTCLIENT_H
