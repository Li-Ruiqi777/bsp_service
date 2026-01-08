#include "utils.h"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace bsp_service
{

std::string formatTimestamp(int64_t timestamp)
{
    std::time_t time = static_cast<std::time_t>(timestamp);
    std::tm *tm_info = std::localtime(&time);

    std::ostringstream oss;
    oss << (tm_info->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (tm_info->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << tm_info->tm_mday << " "
        << std::setw(2) << std::setfill('0') << tm_info->tm_hour << ":"
        << std::setw(2) << std::setfill('0') << tm_info->tm_min << ":"
        << std::setw(2) << std::setfill('0') << tm_info->tm_sec;
    return oss.str();
}

int64_t getCurrentTimestamp()
{
    return static_cast<int64_t>(std::time(nullptr));
}

std::string errorCodeToString(int code)
{
    switch (code)
    {
    case 0:
        return "Success";
    case -1:
        return "Invalid parameter";
    case -2:
        return "Device open failed";
    case -3:
        return "Device I/O failed";
    case -4:
        return "Device not ready";
    case -5:
        return "Memory allocation failed";
    case -6:
        return "Unsupported operation";
    default:
        return "Unknown error";
    }
}

bool isValidJson(const std::string &json_str)
{
    // 简化实现，仅检查基本格式
    if (json_str.empty())
    {
        return false;
    }

    int brace_count = 0;
    int bracket_count = 0;
    bool in_string = false;

    for (char c : json_str)
    {
        if (c == '"' && (json_str.empty() || json_str.back() != '\\'))
        {
            in_string = !in_string;
        }
        if (in_string)
            continue;

        if (c == '{')
            brace_count++;
        else if (c == '}')
            brace_count--;
        else if (c == '[')
            bracket_count++;
        else if (c == ']')
            bracket_count--;
    }

    return brace_count == 0 && bracket_count == 0;
}


} // namespace bsp_service
