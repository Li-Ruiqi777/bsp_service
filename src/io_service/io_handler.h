#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "../common/config.h"
#include <memory>
#include <mutex>
#include <string>

namespace bsp_service
{

// 前向声明
class Config;

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
     * @param cmd 输出：指令名
     * @param dev_name 输出：设备名
     * @param params 输出：参数 JSON 字符串
     * @param seq 输出：序列号
     * @return true 解析成功，false 解析失败
     */
    bool parseRequest(const std::string &request, std::string &cmd, std::string &dev_name,
                      std::string &params, int64_t &seq);

    /**
     * @brief 处理 LED 相关指令
     * @param cmd 指令名
     * @param dev_name 设备名
     * @param params 参数 JSON 字符串
     * @param seq 序列号
     * @return 响应消息 JSON 字符串
     */
    std::string handleLedCommand(const std::string &cmd, const std::string &dev_name,
                                 const std::string &params, int64_t seq);

    /**
     * @brief 处理蜂鸣器相关指令（预留）
     */
    std::string handleBeepCommand(const std::string &cmd, const std::string &dev_name,
                                  const std::string &params, int64_t seq);

    /**
     * @brief 处理按键相关指令（预留）
     */
    std::string handleKeyCommand(const std::string &cmd, const std::string &dev_name,
                                 const std::string &params, int64_t seq);

    Config *config;
    std::mutex hardwareMutex; // 硬件资源锁，避免并发访问
};

} // namespace bsp_service

#endif // IO_HANDLER_H
