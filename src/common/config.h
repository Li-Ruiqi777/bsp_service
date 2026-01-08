#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <map>
#include <string>

namespace bsp_service
{

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
    std::map<std::string, std::string> configMap;
    bool parseJsonFile(const std::string &config_file);
};

} // namespace bsp_service

#endif // CONFIG_H
