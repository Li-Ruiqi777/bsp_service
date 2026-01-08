#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "common/config.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace bsp_service
{

// 前向声明
class Config;

/**
 * @brief 请求消息结构体
 * 
 * 使用 cereal 进行 JSON 序列化/反序列化
 */
struct RequestMessage
{
    std::string cmd;
    std::string devName = "";
    std::map<std::string, std::string> params;
    int64_t seq = 0;

    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::make_nvp("cmd", cmd),
                cereal::make_nvp("dev_name", devName),
                cereal::make_nvp("params", params),
                cereal::make_nvp("seq", seq));
    }
};

/**
 * @brief 响应消息结构体
 * 
 * 使用 cereal 进行 JSON 序列化/反序列化
 */
struct ResponseMessage
{
    int code = 0;
    std::string msg = "success";
    std::map<std::string, std::string> data;
    int64_t seq = 0;

    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::make_nvp("code", code),
                cereal::make_nvp("msg", msg),
                cereal::make_nvp("data", data),
                cereal::make_nvp("seq", seq));
    }
};

/**
 * @brief 输入输出指令处理类
 *
 * 负责解析具体指令，调用 bsp-lib 接口执行硬件操作
 */
class IoHandler
{
public:
    IoHandler();
    ~IoHandler();

    /**
     * @brief 初始化处理器
     * @param config 配置对象指针
     * @return true 成功，false 失败
     */
    bool init(Config *config);

    /**
     * @brief 处理请求消息
     * @param request 请求消息（JSON 格式）
     * @return 响应消息（JSON 格式）
     */
    std::string handleRequest(const std::string &request);

private:
    /**
     * @brief 解析请求消息
     * @param request 请求消息 JSON 字符串
     * @param reqMsg 输出：请求消息结构体
     * @return true 解析成功，false 解析失败
     */
    bool parseRequest(const std::string &request, RequestMessage &reqMsg);

    /**
     * @brief 将响应消息序列化为 JSON 字符串
     * @param respMsg 响应消息结构体
     * @return JSON 字符串
     */
    std::string serializeResponse(const ResponseMessage &respMsg);

    /**
     * @brief 处理 LED 相关指令
     * @param reqMsg 请求消息
     * @return 响应消息 JSON 字符串
     */
    std::string handleLedCommand(const RequestMessage &reqMsg);

    /**
     * @brief 处理蜂鸣器相关指令（预留）
     */
    std::string handleBeepCommand(const RequestMessage &reqMsg);

    /**
     * @brief 处理按键相关指令（预留）
     */
    std::string handleKeyCommand(const RequestMessage &reqMsg);

    Config *config;
    std::mutex hardwareMutex; // 硬件资源锁，避免并发访问
};

} // namespace bsp_service

#endif // IO_HANDLER_H
