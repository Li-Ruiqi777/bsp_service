#include "zmq_wrapper.h"
#include <cstring>
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
    : context(other.context), socket(other.socket), mode(other.mode), initialized(other.initialized)
{
    other.context = nullptr;
    other.socket = nullptr;
    other.initialized = false;
}

ZmqComm &ZmqComm::operator=(ZmqComm &&other) noexcept
{
    if (this != &other)
    {
        close();
        context = other.context;
        socket = other.socket;
        mode = other.mode;
        initialized = other.initialized;
        other.context = nullptr;
        other.socket = nullptr;
        other.initialized = false;
    }
    return *this;
}

bool ZmqComm::init(ZmqMode mode, const std::string &addr)
{
    if (initialized)
    {
        close();
    }

    this->mode = mode;
    context = zmq_ctx_new();
    if (!context)
    {
        return false;
    }

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

    socket = zmq_socket(context, zmq_type);
    if (!socket)
    {
        zmq_ctx_destroy(context);
        context = nullptr;
        return false;
    }

    int ret = 0;
    if (mode == ZmqMode::REP || mode == ZmqMode::PUB)
    {
        // 服务端：绑定地址
        ret = zmq_bind(socket, addr.c_str());
    }
    else
    {
        // 客户端：连接地址
        ret = zmq_connect(socket, addr.c_str());
    }

    if (ret != 0)
    {
        zmq_close(socket);
        zmq_ctx_destroy(context);
        socket = nullptr;
        context = nullptr;
        return false;
    }

    initialized = true;
    return true;
}

int ZmqComm::sendMsg(const std::string &msg)
{
    if (!initialized || !socket)
    {
        return -1;
    }

    int ret = zmq_send(socket, msg.c_str(), msg.length(), 0);
    return (ret >= 0) ? 0 : -1;
}

int ZmqComm::recvMsg(std::string &msg)
{
    if (!initialized || !socket)
    {
        return -1;
    }

    char buffer[4096] = {0};
    int ret = zmq_recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (ret >= 0)
    {
        buffer[ret] = '\0';
        msg = std::string(buffer);
        return 0;
    }
    return -1;
}

void ZmqComm::subscribe(const std::string &topic)
{
    if (mode == ZmqMode::SUB && socket)
    {
        zmq_setsockopt(socket, ZMQ_SUBSCRIBE, topic.c_str(), topic.length());
    }
}

void ZmqComm::close()
{
    if (socket)
    {
        zmq_close(socket);
        socket = nullptr;
    }
    if (context)
    {
        zmq_ctx_destroy(context);
        context = nullptr;
    }
    initialized = false;
}

void ZmqComm::cleanup()
{
    close();
}

} // namespace bsp_service
