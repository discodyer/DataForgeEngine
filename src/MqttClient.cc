#include "MqttClient.h"

bool MqttClient::received = false;
std::string MqttClient::lastMessage = "";

MqttClient::MqttClient(const std::string &clientId) : connected(false), running(false)
{
    /* 调用其他mosquitto函数之前需要初始化 */
    mosquitto_lib_init();
    mosq = mosquitto_new(clientId.c_str(), true, nullptr);
    if (!mosq)
    {
        throw std::runtime_error("Failed to create Mosquitto instance");
    }
    /* Configure callbacks. This should be done before connecting ideally. */
    /* 配置回调，这应该在连接之前完成。 */
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_subscribe_callback_set(mosq, on_subscribe);
    mosquitto_message_callback_set(mosq, on_message);
}

MqttClient::~MqttClient()
{
    if (connected)
    {
        mosquitto_disconnect(mosq);
    }
    mosquitto_loop_stop(mosq, true);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

int MqttClient::connect(const std::string &host, int port, int keepalive)
{
    // mosquitto_connect(mosq, host.c_str(), port, keepalive);
    int rc;
    // mosquitto_username_pw_set(mosq, username, password);
    rc = mosquitto_connect(mosq, host.c_str(), port, keepalive);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        return 1;
    }
    connected = true;
    // 在成功连接后，启动消息循环
    // mosquitto_loop_start(mosq);
    return 0;
}

void MqttClient::disconnect()
{
    mosquitto_disconnect(mosq);
    connected = false;
}

bool MqttClient::isConnected() const
{
    return connected;
}

void MqttClient::publish(const std::string &topic, const std::string &message, int qos, bool retain)
{
    mosquitto_publish(mosq, nullptr, topic.c_str(), message.length(), message.c_str(), qos, retain);
}

void MqttClient::subscribe(const std::string &topic, int qos)
{
    mosquitto_subscribe(mosq, nullptr, topic.c_str(), qos);
}

void MqttClient::doLoop(int timeout)
{
    // 异步循环
    int rc;
    rc = mosquitto_loop_start(mosq);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        mosquitto_destroy(mosq);
        fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
        return;
    }

    // 开始循环
    printf("Start!\n");
    while (!received && timeout--)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(1)); // 等待订阅完成
    }

    // 清理资源
    // mosquitto_disconnect(mosq);
    // mosquitto_destroy(mosq);
    // mosquitto_lib_cleanup();
}

void MqttClient::on_connect(mosquitto *mosq, void *obj, int reason_code)
{
    int rc;
    /* Print out the connection result. mosquitto_connack_string() produces an
     * appropriate string for MQTT v3.x clients, the equivalent for MQTT v5.0
     * clients is mosquitto_reason_string().
     打印连接结果。mosquitto_connack_string()为mqttv3.x客户端生成适当的字符串，与
     mqttv5.0客户端等效的是mosquitto_reason_string()。
     */
    std::cout << "on_connect: " << mosquitto_connack_string(reason_code) << std::endl;
    if (reason_code != 0)
    {
        /* If the connection fails for any reason, we don't want to keep on
         * retrying in this example, so disconnect. Without this, the client
         * will attempt to reconnect.
         如果由于任何原因连接失败，在本例中我们不想继续重试，所以断开连接。否则，客户端将尝
         试重新连接。
         */
        mosquitto_disconnect(mosq);
    }

    /* Making subscriptions in the on_connect() callback means that if the
     * connection drops and is automatically resumed by the client, then the
     * subscriptions will be recreated when the client reconnects.
     在on_connect()回调中进行订阅意味着如果连接断开并由客户端自动恢复，则在客户端重新连接时将重
     新创建订阅。
     */
    // rc = mosquitto_subscribe(mosq, NULL, "example/temperature", 1);
    // if (rc != MOSQ_ERR_SUCCESS)
    // {
    //     fprintf(stderr, "Error subscribing: %s\n", mosquitto_strerror(rc));
    //     /* We might as well disconnect if we were unable to subscribe */
    //     /* 如果无法订阅，我们应该断开连接。 */
    //     mosquitto_disconnect(mosq);
    // }
}

void MqttClient::on_subscribe(mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    int i;
    bool have_subscription = false;

    /* In this example we only subscribe to a single topic at once, but a
     * SUBSCRIBE can contain many topics at once, so this is one way to check
     * them all.
     在本例中，我们一次只订阅一个主题，但是订阅可以一次包含多个主题，因此这是检查所有主题的一种方
     法。
     */
    for (i = 0; i < qos_count; i++)
    {
        std::cout << "on_subscribe: " << i << ":granted qos = " << granted_qos[i];
        if (granted_qos[i] <= 2)
        {
            have_subscription = true;
        }
    }
    if (have_subscription == false)
    {
        /* The broker rejected all of our subscriptions, we know we only sent
         * the one SUBSCRIBE, so there is no point remaining connected.
         代理拒绝了我们所有的订阅，我们知道我们只发送了一个订阅，所以没有保持连接的点。
         */
        fprintf(stderr, "Error: All subscriptions rejected.\n");
        mosquitto_disconnect(mosq);
    }
}

void MqttClient::on_message(mosquitto *mosq, void *obj, const mosquitto_message *msg)
{
    std::cout << "topic: " << msg->topic << std::endl;
    std::cout << "qos: " << msg->qos << std::endl;
    std::cout << "payload: " << (char *)msg->payload << std::endl;

    received = true;
    lastMessage = std::string(static_cast<char*>(msg->payload), msg->payloadlen);
}