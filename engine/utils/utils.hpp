#pragma once
#include <string>

namespace grafyte::utils
{
inline std::string stripUtf8Bom(std::string s)
{
    if (s.size() >= 3 && static_cast<unsigned char>(s[0]) == 0xEF && static_cast<unsigned char>(s[1]) == 0xBB &&
        static_cast<unsigned char>(s[2]) == 0xBF)
    {
        s.erase(0, 3);
    }
    return s;
}
} // namespace grafyte::utils
