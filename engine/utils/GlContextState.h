#pragma once

namespace grafyte
{
    inline bool& GlContextAlive()
    {
        static bool alive = false;
        return alive;
    }
}
