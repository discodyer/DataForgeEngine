#include "MqttClient.h"
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    try
    {
        MqttClient client("TestClient");
        client.connect("localhost", 1884, 60);

        if (client.isConnected())
        {
            std::cout << "Connected to MQTT broker." << std::endl;
        }
        else
        {
            std::cout << "Failed to connect to MQTT broker." << std::endl;
            return 1;
        }

        // 简单测试发布和订阅功能
        std::string topic = "test/topic";
        std::string message = "Hello MQTT";
        client.subscribe(topic, 0);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待订阅完成
        // client.publish(topic, message, 0, false);

        client.doLoop(10000); // 启动循环，并在10秒后停止
        
        // 断开与MQTT服务器的连接
        client.disconnect();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
