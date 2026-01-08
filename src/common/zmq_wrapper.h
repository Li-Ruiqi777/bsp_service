#ifndef ZMQ_WRAPPER_H
#define ZMQ_WRAPPER_H

#include <string>
#include <zmq.h>

namespace bsp_service
{

// ZeroMQ 通信模式
enum class ZmqMode
{
    REQ, // 请求模式（客户端）
    REP, // 响应模式（服务端）
    PUB, // 发布模式（发布者）
    SUB  // 订阅模式（订阅者）
};

/**
 * @brief ZeroMQ 通信封装类
 *
 * 提供统一的 ZeroMQ 消息收发接口，支持 REQ/REP 和 PUB/SUB 模式
 */
class ZmqComm
{
public:
    ZmqComm();
    ~ZmqComm();

    // 禁止拷贝
    ZmqComm(const ZmqComm &) = delete;
    ZmqComm &operator=(const ZmqComm &) = delete;

    // 允许移动
    ZmqComm(ZmqComm &&other) noexcept;
    ZmqComm &operator=(ZmqComm &&other) noexcept;

    /**
     * @brief 初始化通信模式
     * @param mode 通信模式（REQ/REP/PUB/SUB）
     * @param addr 绑定/连接地址（如 tcp://*:5555 或 tcp://localhost:5555）
     * @return true 成功，false 失败
     */
    bool init(ZmqMode mode, const std::string &addr);

    /**
     * @brief 发送消息
     * @param msg 要发送的消息字符串
     * @return 0 成功，-1 失败
     */
    int sendMsg(const std::string &msg);

    /**
     * @brief 接收消息
     * @param msg 接收到的消息字符串（输出参数）
     * @return 0 成功，-1 失败
     */
    int recvMsg(std::string &msg);

    /**
     * @brief 订阅指定主题（仅 SUB 模式）
     * @param topic 主题字符串
     */
    void subscribe(const std::string &topic);

    /**
     * @brief 关闭连接，释放资源
     */
    void close();

    /**
     * @brief 检查是否已初始化
     * @return true 已初始化，false 未初始化
     */
    bool isInitialized() const
    {
        return initialized;
    }

private:
    void *context;    // ZeroMQ 上下文
    void *socket;     // ZeroMQ socket
    ZmqMode mode;     // 通信模式
    bool initialized; // 是否已初始化

    void cleanup();
};

} // namespace bsp_service

#endif // ZMQ_WRAPPER_H
