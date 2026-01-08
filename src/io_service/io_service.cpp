#include "io_service.h"
#include "io_handler.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace bsp_service
{

static IoService *g_service_instance = nullptr;

static void signalHandler(int sig)
{
    if (g_service_instance)
    {
        g_service_instance->stop();
    }
}

IoService::IoService() : running(false), serviceName("io-service"), zmqPort(5555)
{
    g_service_instance = this;
}

IoService::~IoService()
{
    cleanup();
}

bool IoService::init(const std::string &config_file)
{
    // 加载配置文件
    if (!config.loadFromFile(config_file))
    {
        std::cerr << "Failed to load file" << std::endl;
        return false;
    }

    // 读取配置项
    zmqPort = config.getInt("zmq_port", 5555);
    serviceName = config.getString("service_name", "io-service");

    // 初始化 ZeroMQ（REP 模式）
    zmqComm = std::make_unique<ZmqComm>();
    std::string addr = "tcp://*:" + std::to_string(zmqPort);
    if (!zmqComm->init(ZmqMode::REP, addr))
    {
        std::cerr << "Failed to initialize zmq" << std::endl;
        return false;
    }

    // 初始化指令处理器
    ioHandler = std::make_unique<IoHandler>();
    if (!ioHandler->init(&config))
    {
        std::cerr << "Failed to ioHandler" << std::endl;
        return false;
    }

    // 设置信号处理
    setupSignalHandlers();

    return true;
}

bool IoService::daemonize()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        return false;
    }
    else if (pid > 0)
    {
        // 父进程退出
        exit(0);
    }

    // 子进程继续
    // 创建新的会话
    if (setsid() < 0)
    {
        return false;
    }

    // 再次 fork，确保不是会话首进程
    pid = fork();
    if (pid < 0)
    {
        return false;
    }
    else if (pid > 0)
    {
        exit(0);
    }

    // 更改工作目录
    chdir("/");

    // 关闭文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // 重定向到 /dev/null
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);

    return true;
}

void IoService::setupSignalHandlers()
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGPIPE, SIG_IGN);
}

int IoService::run()
{
    running = true;

    // 是否守护进程化（根据配置决定）
    bool daemon = config.getBool("daemon", false);
    if (daemon)
    {
        if (!daemonize())
        {
            return -1;
        }
    }

    // 主循环：接收请求并处理
    while (running)
    {
        std::string request;
        if (zmqComm->recvMsg(request) == 0)
        {
            // 解析请求并处理
            std::string response = ioHandler->handleRequest(request);

            // 发送响应
            zmqComm->sendMsg(response);
        }
    }

    return 0;
}

void IoService::stop()
{
    running = false;
}

void IoService::cleanup()
{
    if (ioHandler)
    {
        ioHandler.reset();
    }
    if (zmqComm)
    {
        zmqComm->close();
        zmqComm.reset();
    }
}

} // namespace bsp_service
