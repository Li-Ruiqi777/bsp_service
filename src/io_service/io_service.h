#ifndef IO_SERVICE_H
#define IO_SERVICE_H

#include "../common/config.h"
#include "../common/zmq_wrapper.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <string>

namespace bsp_service
{

// 前向声明
class IoHandler;

/**
 * @brief 输入输出服务核心类
 *
 * 负责服务初始化、守护进程化、通信连接管理、请求分发
 */
class IoService
{
public:
    IoService();
    ~IoService();

    // 禁止拷贝
    IoService(const IoService &) = delete;
    IoService &operator=(const IoService &) = delete;

    /**
     * @brief 初始化服务
     * @param config_file 配置文件路径
     * @return true 成功，false 失败
     */
    bool init(const std::string &config_file);

    /**
     * @brief 运行服务主循环
     * @return 0 正常退出，非 0 异常退出
     */
    int run();

    /**
     * @brief 停止服务
     */
    void stop();

    /**
     * @brief 检查服务是否正在运行
     * @return true 运行中，false 已停止
     */
    bool isRunning() const
    {
        return running;
    }

private:
    /**
     * @brief 转换为守护进程
     * @return true 成功，false 失败
     */
    bool daemonize();

    /**
     * @brief 注册信号处理
     */
    void setupSignalHandlers();

    /**
     * @brief 清理资源
     */
    void cleanup();

    Config config;
    std::unique_ptr<ZmqComm> zmqComm;
    std::unique_ptr<IoHandler> ioHandler;
    std::atomic<bool> running;
    std::string serviceName;
    int zmqPort;
};

} // namespace bsp_service

#endif // IO_SERVICE_H
