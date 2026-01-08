#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>

namespace bsp_service
{

/**
 * @brief 通用工具函数
 */

/**
 * @brief 格式化时间戳为字符串
 * @param timestamp Unix 时间戳（秒）
 * @return 格式化后的时间字符串
 */
std::string formatTimestamp(int64_t timestamp);

/**
 * @brief 获取当前时间戳（秒）
 * @return Unix 时间戳
 */
int64_t getCurrentTimestamp();

/**
 * @brief 将错误码转换为错误消息
 * @param code bsp::ErrorCode 错误码
 * @return 错误消息字符串
 */
std::string errorCodeToString(int code);

/**
 * @brief 检查字符串是否为有效的 JSON
 * @param json_str JSON 字符串
 * @return true 有效，false 无效
 */
bool isValidJson(const std::string &json_str);


} // namespace bsp_service

#endif // UTILS_H
