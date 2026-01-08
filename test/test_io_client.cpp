#include "../src/common/utils.h"
#include "../src/common/zmq_wrapper.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace bsp_service;

int main()
{
    // 创建 ZeroMQ REQ socket 连接到 io-service
    ZmqComm client;
    if (!client.init(ZmqMode::REQ, "tcp://localhost:5555"))
    {
        std::cerr << "Failed to connect to io-service" << std::endl;
        return 1;
    }

    std::cout << "Connected to io-service" << std::endl;

    // 测试 LED 控制指令
    std::string request = R"({"cmd":"led_turn_on","dev_name":"led0","params":{},"seq":1})";

    std::cout << "Sending request: " << request << std::endl;

    if (client.sendMsg(request) != 0)
    {
        std::cerr << "Failed to send message" << std::endl;
        return 1;
    }

    std::string response;
    if (client.recvMsg(response) == 0)
    {
        std::cout << "Received response: " << response << std::endl;
    }
    else
    {
        std::cerr << "Failed to receive response" << std::endl;
        return 1;
    }

    // 等待一下
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 测试关闭 LED
    request = R"({"cmd":"led_turn_off","dev_name":"led0","params":{},"seq":2})";
    std::cout << "Sending request: " << request << std::endl;

    if (client.sendMsg(request) != 0)
    {
        std::cerr << "Failed to send message" << std::endl;
        return 1;
    }

    if (client.recvMsg(response) == 0)
    {
        std::cout << "Received response: " << response << std::endl;
    }
    else
    {
        std::cerr << "Failed to receive response" << std::endl;
        return 1;
    }

    client.close();
    std::cout << "Test completed" << std::endl;

    return 0;
}
