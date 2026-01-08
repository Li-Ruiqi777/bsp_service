#include "config.h"
#include <algorithm>
#include <cctype>
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
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
        return false;
    }

    try
    {
        cereal::JSONInputArchive archive(file);

        // 使用 cereal 的 JSON 输入归档逐字段读取配置
        std::string strValue;
        int intValue;
        bool boolValue;
        double doubleValue;

        // 读取字符串类型配置
        try
        {
            archive(cereal::make_nvp("service_name", strValue));
            configMap["service_name"] = strValue;
        }
        catch (...)
        {
            // 字段不存在或类型不匹配，忽略
        }

        // 读取整数类型配置
        try
        {
            archive(cereal::make_nvp("zmq_port", intValue));
            configMap["zmq_port"] = std::to_string(intValue);
        }
        catch (...)
        {
            // 字段不存在或类型不匹配，忽略
        }

        try
        {
            archive(cereal::make_nvp("zmq_port_rep", intValue));
            configMap["zmq_port_rep"] = std::to_string(intValue);
        }
        catch (...)
        {
            // 字段不存在或类型不匹配，忽略
        }

        try
        {
            archive(cereal::make_nvp("zmq_port_pub", intValue));
            configMap["zmq_port_pub"] = std::to_string(intValue);
        }
        catch (...)
        {
            // 字段不存在或类型不匹配，忽略
        }

        // 读取布尔类型配置
        try
        {
            archive(cereal::make_nvp("daemon", boolValue));
            configMap["daemon"] = boolValue ? "true" : "false";
        }
        catch (...)
        {
            // 字段不存在或类型不匹配，忽略
        }

        // 可以继续添加其他配置字段的解析
    }
    catch (const std::exception &e)
    {
        return false;
    }

    return true;
}

std::string Config::getString(const std::string &key, const std::string &default_value) const
{
    auto it = configMap.find(key);
    if (it != configMap.end())
    {
        return it->second;
    }
    return default_value;
}

int Config::getInt(const std::string &key, int default_value) const
{
    auto it = configMap.find(key);
    if (it != configMap.end())
    {
        try
        {
            return std::stoi(it->second);
        }
        catch (...)
        {
            return default_value;
        }
    }
    return default_value;
}

bool Config::getBool(const std::string &key, bool default_value) const
{
    auto it = configMap.find(key);
    if (it != configMap.end())
    {
        std::string value = it->second;
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        return (value == "true" || value == "1");
    }
    return default_value;
}

double Config::getDouble(const std::string &key, double default_value) const
{
    auto it = configMap.find(key);
    if (it != configMap.end())
    {
        try
        {
            return std::stod(it->second);
        }
        catch (...)
        {
            return default_value;
        }
    }
    return default_value;
}

bool Config::hasKey(const std::string &key) const
{
    return configMap.find(key) != configMap.end();
}

} // namespace bsp_service
