#include "common/utils.h"
#include "common/zmq_wrapper.h"
#include "io_service/io_handler.h"
#include <cereal/archives/json.hpp>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

using namespace bsp_service;

/**
 * @brief 序列化请求消息为 JSON 字符串
 */
std::string serializeRequest(const RequestMessage &req)
{
    std::stringstream ss;
    {
        cereal::JSONOutputArchive archive(ss);
        archive(req);
    }
    return ss.str();
}

/**
 * @brief 反序列化响应消息
 */
ResponseMessage deserializeResponse(const std::string &json)
{
    ResponseMessage resp;
    std::stringstream ss(json);
    {
        cereal::JSONInputArchive archive(ss);
        archive(resp);
    }
    return resp;
}

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

    // 测试 LED 开启指令
    RequestMessage ledOnReq;
    ledOnReq.cmd = "led_turn_on";
    ledOnReq.devName = "led";
    ledOnReq.params = {};
    ledOnReq.seq = 1;

    std::string request = serializeRequest(ledOnReq);
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
        try
        {
            ResponseMessage respMsg = deserializeResponse(response);
            std::cout << "Response Code: " << respMsg.code << ", Message: " << respMsg.msg << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to parse response: " << e.what() << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to receive response" << std::endl;
        return 1;
    }

    // 等待一下
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 测试关闭 LED
    RequestMessage ledOffReq;
    ledOffReq.cmd = "led_turn_off";
    ledOffReq.devName = "led";
    ledOffReq.params = {};
    ledOffReq.seq = 2;

    request = serializeRequest(ledOffReq);
    std::cout << "Sending request: " << request << std::endl;

    if (client.sendMsg(request) != 0)
    {
        std::cerr << "Failed to send message" << std::endl;
        return 1;
    }

    if (client.recvMsg(response) == 0)
    {
        std::cout << "Received response: " << response << std::endl;
        try
        {
            ResponseMessage respMsg = deserializeResponse(response);
            std::cout << "Response Code: " << respMsg.code << ", Message: " << respMsg.msg << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to parse response: " << e.what() << std::endl;
        }
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
