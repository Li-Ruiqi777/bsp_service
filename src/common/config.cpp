#include "config.h"
#include <fstream>
#include <sstream>

namespace bsp_service
{

Config::Config()
{
}

Config::~Config()
{
}

bool Config::loadFromFile(const std::string &config_file)
{
    return parseJsonFile(config_file);
}

bool Config::parseJsonFile(const std::string &config_file)
{
    std::ifstream file(config_file);
    if (!file.is_open())
    {
        std::cout << "File not found" << std::endl;
        return false;
    }

    try
    {
        cereal::JSONInputArchive archive(file);
        archive(cereal::make_nvp("ConfigData", configData));
        return true;
    }
    catch (const std::exception &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        return false;
    }
}

std::string Config::getString(const std::string &key, const std::string &default_value) const
{
    if (key == "service_name")
        return configData.serviceName;
    return default_value;
}

int Config::getInt(const std::string &key, int default_value) const
{
    if (key == "zmq_port")
        return configData.zmqPort;
    else if (key == "zmq_port_rep")
        return configData.zmqPortRep;
    else if (key == "zmq_port_pub")
        return configData.zmqPortPub;
    return default_value;
}

bool Config::getBool(const std::string &key, bool default_value) const
{
    if (key == "daemon")
        return configData.daemon;
    return default_value;
}

double Config::getDouble(const std::string &key, double default_value) const
{
    // 目前配置中没有浮点数类型，返回默认值
    return default_value;
}

bool Config::hasKey(const std::string &key) const
{
    return (key == "service_name" || key == "zmq_port" || key == "zmq_port_rep" || key == "zmq_port_pub" ||
            key == "daemon");
}

} // namespace bsp_service
