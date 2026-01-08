#include "io_handler.h"
#include "bsp.h"
#include <sstream>

namespace bsp_service
{

IoHandler::IoHandler() : config(nullptr)
{
}

IoHandler::~IoHandler()
{
}

bool IoHandler::init(Config *config)
{
    this->config = config;
    return true;
}

bool IoHandler::parseRequest(const std::string &request, RequestMessage &reqMsg)
{
    try
    {
        std::istringstream iss(request);
        cereal::JSONInputArchive archive(iss);
        archive(reqMsg);
        
        if (reqMsg.cmd.empty())
        {
            return false;
        }
        
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

std::string IoHandler::serializeResponse(const ResponseMessage &respMsg)
{
    try
    {
        std::ostringstream oss;
        {
            cereal::JSONOutputArchive archive(oss);
            archive(respMsg);
        }
        return oss.str();
    }
    catch (const std::exception &e)
    {
        // 如果序列化失败，返回一个简单的错误响应
        ResponseMessage errorResp;
        errorResp.code = -1;
        errorResp.msg = "Serialization failed: " + std::string(e.what());
        errorResp.seq = respMsg.seq;
        
        std::ostringstream oss;
        {
            cereal::JSONOutputArchive archive(oss);
            archive(errorResp);
        }
        return oss.str();
    }
}

std::string IoHandler::handleRequest(const std::string &request)
{
    RequestMessage reqMsg;
    
    if (!parseRequest(request, reqMsg))
    {
        ResponseMessage respMsg;
        respMsg.code = -1;
        respMsg.msg = "Invalid request format";
        respMsg.seq = 0;
        return serializeResponse(respMsg);
    }

    // 根据指令前缀分发到对应处理器
    if (reqMsg.cmd.find("led_") == 0)
    {
        return handleLedCommand(reqMsg);
    }
    else if (reqMsg.cmd.find("beep_") == 0)
    {
        return handleBeepCommand(reqMsg);
    }
    else if (reqMsg.cmd.find("key_") == 0)
    {
        return handleKeyCommand(reqMsg);
    }
    else
    {
        ResponseMessage respMsg;
        respMsg.code = -6;
        respMsg.msg = "Unsupported command: " + reqMsg.cmd;
        respMsg.seq = reqMsg.seq;
        return serializeResponse(respMsg);
    }
}

std::string IoHandler::handleLedCommand(const RequestMessage &reqMsg)
{
    std::lock_guard<std::mutex> lock(hardwareMutex);

    // 创建 LED 对象（注意：实际应该维护设备对象池，这里简化处理）
    std::string devName = reqMsg.devName.empty() ? "led0" : reqMsg.devName;
    bsp::Led led(devName);

    bsp::ErrorCode ret = led.init();
    ResponseMessage respMsg;
    respMsg.seq = reqMsg.seq;

    if (ret != bsp::ErrorCode::Ok)
    {
        respMsg.code = static_cast<int>(ret);
        respMsg.msg = "LED init failed: " + bsp::errorToString(ret);
        return serializeResponse(respMsg);
    }

    // 根据指令执行操作
    if (reqMsg.cmd == "led_turn_on")
    {
        ret = led.turnOn();
    }
    else if (reqMsg.cmd == "led_turn_off")
    {
        ret = led.turnOff();
    }
    else if (reqMsg.cmd == "led_set_state")
    {
        // 从 params map 中获取 state
        auto it = reqMsg.params.find("state");
        if (it != reqMsg.params.end())
        {
            std::string stateStr = it->second;
            bool stateBool = (stateStr == "on" || stateStr == "true" || stateStr == "1");
            ret = led.setState(stateBool);
        }
        else
        {
            respMsg.code = -1;
            respMsg.msg = "Missing 'state' parameter";
            return serializeResponse(respMsg);
        }
    }
    else
    {
        respMsg.code = -6;
        respMsg.msg = "Unsupported LED command: " + reqMsg.cmd;
        return serializeResponse(respMsg);
    }

    if (ret == bsp::ErrorCode::Ok)
    {
        respMsg.code = 0;
        respMsg.msg = "Success";
    }
    else
    {
        respMsg.code = static_cast<int>(ret);
        respMsg.msg = "Operation failed: " + bsp::errorToString(ret);
    }

    return serializeResponse(respMsg);
}

std::string IoHandler::handleBeepCommand(const RequestMessage &reqMsg)
{
    // 预留：蜂鸣器指令处理`
    ResponseMessage respMsg;
    respMsg.code = -6;
    respMsg.msg = "Beep command not implemented yet";
    respMsg.seq = reqMsg.seq;
    return serializeResponse(respMsg);
}

std::string IoHandler::handleKeyCommand(const RequestMessage &reqMsg)
{
    // 预留：按键指令处理
    ResponseMessage respMsg;
    respMsg.code = -6;
    respMsg.msg = "Key command not implemented yet";
    respMsg.seq = reqMsg.seq;
    return serializeResponse(respMsg);
}

} // namespace bsp_service
