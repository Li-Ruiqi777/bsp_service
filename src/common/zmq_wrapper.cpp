#include "zmq_wrapper.h"
#include <iostream>
#include <stdexcept>

namespace bsp_service
{

ZmqComm::ZmqComm() : context(nullptr), socket(nullptr), mode(ZmqMode::REQ), initialized(false)
{
}

ZmqComm::~ZmqComm()
{
    close();
}

ZmqComm::ZmqComm(ZmqComm &&other) noexcept
    : context(std::move(other.context)), socket(std::move(other.socket)), mode(other.mode),
      initialized(other.initialized)
{
    other.initialized = false;
}

ZmqComm &ZmqComm::operator=(ZmqComm &&other) noexcept
{
    if (this != &other)
    {
        close();
        context = std::move(other.context);
        socket = std::move(other.socket);
        mode = other.mode;
        initialized = other.initialized;
        other.initialized = false;
    }
    return *this;
}

bool ZmqComm::init(ZmqMode mode, const std::string &addr)
{
    try
    {
        // 如果已初始化，先关闭
        if (initialized)
        {
            close();
        }

        this->mode = mode;

        // 创建上下文（1 个 I/O 线程）
        context = std::make_unique<zmq::context_t>(1);

        // 根据模式创建 socket
        int zmq_type = 0;
        switch (mode)
        {
        case ZmqMode::REQ:
            zmq_type = ZMQ_REQ;
            break;
        case ZmqMode::REP:
            zmq_type = ZMQ_REP;
            break;
        case ZmqMode::PUB:
            zmq_type = ZMQ_PUB;
            break;
        case ZmqMode::SUB:
            zmq_type = ZMQ_SUB;
            break;
        }

        socket = std::make_unique<zmq::socket_t>(*context, zmq_type);

        // 根据模式进行绑定或连接
        if (mode == ZmqMode::REP || mode == ZmqMode::PUB)
        {
            // 服务端：绑定地址
            socket->bind(addr);
        }
        else
        {
            // 客户端：连接地址
            socket->connect(addr);
        }

        initialized = true;
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ZmqComm init failed: " << e.what() << std::endl;
        close();
        return false;
    }
}

int ZmqComm::sendMsg(const std::string &msg)
{
    try
    {
        if (!initialized || !socket)
        {
            return -1;
        }

        zmq::message_t message(msg.begin(), msg.end());
        socket->send(message, zmq::send_flags::none);
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ZmqComm sendMsg failed: " << e.what() << std::endl;
        return -1;
    }
}

int ZmqComm::recvMsg(std::string &msg)
{
    try
    {
        if (!initialized || !socket)
        {
            return -1;
        }

        zmq::message_t message;
        auto result = socket->recv(message, zmq::recv_flags::none);

        if (result)
        {
            msg = message.to_string();
            return 0;
        }
        return -1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ZmqComm recvMsg failed: " << e.what() << std::endl;
        return -1;
    }
}

void ZmqComm::subscribe(const std::string &topic)
{
    try
    {
        if (mode == ZmqMode::SUB && socket)
        {
            socket->set(zmq::sockopt::subscribe, topic);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "ZmqComm subscribe failed: " << e.what() << std::endl;
    }
}

void ZmqComm::close()
{
    try
    {
        if (socket)
        {
            socket->close();
            socket.reset();
        }
        if (context)
        {
            context.reset();
        }
        initialized = false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ZmqComm close failed: " << e.what() << std::endl;
    }
}

} // namespace bsp_service
