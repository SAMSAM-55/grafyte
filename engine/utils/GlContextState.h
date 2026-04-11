#pragma once

namespace grafyte
{
inline bool &glContextAlive()
{
    static bool s_Alive = false;
    return s_Alive;
}
} // namespace grafyte
