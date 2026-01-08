#include "io_handler.h"
#include "../common/config.h"
#include "../common/utils.h"
#include "bsp.h" // bsp-lib 总头文件
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <fstream>
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

bool IoHandler::parseRequest(const std::string &request, std::string &cmd, std::string &dev_name,
                             std::string &params, int64_t &seq)
{
    try
    {
        std::istringstream iss(request);
        cereal::JSONInputArchive archive(iss);

        // 使用 cereal 解析 JSON 请求
        archive(cereal::make_nvp("cmd", cmd));

        // dev_name 是可选字段
        try
        {
            archive(cereal::make_nvp("dev_name", dev_name));
        }
        catch (...)
        {
            dev_name = "";
        }

        // params 是可选字段，需要特殊处理（可能是对象）
        // 先尝试作为字符串读取，如果失败则作为空对象处理
        try
        {
            std::map<std::string, std::string> paramsMap;
            archive(cereal::make_nvp("params", paramsMap));
            // 将 map 转换为 JSON 字符串（简化处理）
            std::ostringstream oss;
            {
                cereal::JSONOutputArchive outArchive(oss);
                outArchive(cereal::make_nvp("params", paramsMap));
            }
            params = oss.str();
        }
        catch (...)
        {
            // 如果 params 不是 map，尝试作为字符串
            try
            {
                archive(cereal::make_nvp("params", params));
            }
            catch (...)
            {
                params = "{}";
            }
        }

        // seq 是可选字段
        try
        {
            archive(cereal::make_nvp("seq", seq));
        }
        catch (...)
        {
            seq = 0;
        }

        if (cmd.empty())
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

std::string IoHandler::handleRequest(const std::string &request)
{
    std::string cmd, dev_name, params;
    int64_t seq = 0;

    if (!parseRequest(request, cmd, dev_name, params, seq))
    {
        return createResponseJson(-1, "Invalid request format", "{}", seq);
    }

    // 根据指令前缀分发到对应处理器
    if (cmd.find("led_") == 0)
    {
        return handleLedCommand(cmd, dev_name, params, seq);
    }
    else if (cmd.find("beep_") == 0)
    {
        return handleBeepCommand(cmd, dev_name, params, seq);
    }
    else if (cmd.find("key_") == 0)
    {
        return handleKeyCommand(cmd, dev_name, params, seq);
    }
    else
    {
        return createResponseJson(-6, "Unsupported command: " + cmd, "{}", seq);
    }
}

std::string IoHandler::handleLedCommand(const std::string &cmd, const std::string &dev_name,
                                        const std::string &params, int64_t seq)
{
    std::lock_guard<std::mutex> lock(hardwareMutex);

    // 创建 LED 对象（注意：实际应该维护设备对象池，这里简化处理）
    bsp::Led led(dev_name.empty() ? "led0" : dev_name);

    bsp::ErrorCode ret = led.init();
    if (ret != bsp::ErrorCode::Ok)
    {
        return createResponseJson(static_cast<int>(ret), "LED init failed: " + bsp::errorToString(ret), "{}",
                                  seq);
    }

    // 根据指令执行操作
    if (cmd == "led_turn_on")
    {
        ret = led.turnOn();
    }
    else if (cmd == "led_turn_off")
    {
        ret = led.turnOff();
    }
    else if (cmd == "led_set_state")
    {
        // 解析参数中的 on/off
        try
        {
            std::istringstream iss(params);
            cereal::JSONInputArchive archive(iss);

            std::string stateStr;
            bool stateBool = false;

            // 先尝试作为字符串读取
            try
            {
                archive(cereal::make_nvp("state", stateStr));
                stateBool = (stateStr == "on" || stateStr == "true" || stateStr == "1");
            }
            catch (...)
            {
                // 如果失败，尝试作为布尔值读取
                try
                {
                    archive(cereal::make_nvp("state", stateBool));
                }
                catch (...)
                {
                    return createResponseJson(-1, "Missing or invalid 'state' parameter", "{}", seq);
                }
            }

            ret = led.setState(stateBool);
        }
        catch (const std::exception &e)
        {
            return createResponseJson(-1, "Invalid parameters: " + std::string(e.what()), "{}", seq);
        }
    }
    else
    {
        return createResponseJson(-6, "Unsupported LED command: " + cmd, "{}", seq);
    }

    if (ret == bsp::ErrorCode::Ok)
    {
        return createResponseJson(0, "Success", "{}", seq);
    }
    else
    {
        return createResponseJson(static_cast<int>(ret), "Operation failed: " + bsp::errorToString(ret), "{}",
                                  seq);
    }
}

std::string IoHandler::handleBeepCommand(const std::string &cmd, const std::string &dev_name,
                                         const std::string &params, int64_t seq)
{
    // 预留：蜂鸣器指令处理
    return createResponseJson(-6, "Beep command not implemented yet", "{}", seq);
}

std::string IoHandler::handleKeyCommand(const std::string &cmd, const std::string &dev_name,
                                        const std::string &params, int64_t seq)
{
    // 预留：按键指令处理
    return createResponseJson(-6, "Key command not implemented yet", "{}", seq);
}

} // namespace bsp_service
