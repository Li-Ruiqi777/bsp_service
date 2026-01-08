#include "io_service.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    std::string config_file = "/usr/etc/bsp-service/io_service.json";

    // 解析命令行参数
    if (argc > 1)
    {
        config_file = argv[1];
    }

    bsp_service::IoService service;

    if (!service.init(config_file))
    {
        std::cerr << "Failed to initialize io-service" << std::endl;
        return 1;
    }

    std::cout << "io-service started" << std::endl;

    int ret = service.run();

    std::cout << "io-service stopped with code: " << ret << std::endl;

    return ret;
}
