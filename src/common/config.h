#ifndef CONFIG_H
#define CONFIG_H

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cstdint>
#include <string>

namespace bsp_service
{

/**
 * @brief 配置数据结构体
 * 
 * 使用 cereal 进行 JSON 序列化/反序列化
 */
struct ConfigData
{
    std::string serviceName = "io-service";
    int zmqPort = 5555;
    int zmqPortRep = 5556;
    int zmqPortPub = 5557;
    bool daemon = false;

    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::make_nvp("service_name", serviceName),
                cereal::make_nvp("zmq_port", zmqPort),
                cereal::make_nvp("zmq_port_rep", zmqPortRep),
                cereal::make_nvp("zmq_port_pub", zmqPortPub),
                cereal::make_nvp("daemon", daemon));
    }
};

/**
 * @brief 配置解析类
 *
 * 基于 cereal 库实现 JSON 配置文件的解析
 */
class Config
{
public:
    Config();
    ~Config();

    /**
     * @brief 从 JSON 文件加载配置
     * @param config_file 配置文件路径
     * @return true 成功，false 失败
     */
    bool loadFromFile(const std::string &config_file);

    /**
     * @brief 获取字符串配置项
     * @param key 配置项键名
     * @param default_value 默认值
     * @return 配置值或默认值
     */
    std::string getString(const std::string &key, const std::string &default_value = "") const;

    /**
     * @brief 获取整数配置项
     * @param key 配置项键名
     * @param default_value 默认值
     * @return 配置值或默认值
     */
    int getInt(const std::string &key, int default_value = 0) const;

    /**
     * @brief 获取布尔配置项
     * @param key 配置项键名
     * @param default_value 默认值
     * @return 配置值或默认值
     */
    bool getBool(const std::string &key, bool default_value = false) const;

    /**
     * @brief 获取浮点数配置项
     * @param key 配置项键名
     * @param default_value 默认值
     * @return 配置值或默认值
     */
    double getDouble(const std::string &key, double default_value = 0.0) const;

    /**
     * @brief 检查配置项是否存在
     * @param key 配置项键名
     * @return true 存在，false 不存在
     */
    bool hasKey(const std::string &key) const;

private:
    ConfigData configData;
    bool parseJsonFile(const std::string &config_file);
};

} // namespace bsp_service

#endif // CONFIG_H
