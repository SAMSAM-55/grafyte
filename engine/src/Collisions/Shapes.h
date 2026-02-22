#pragma once

#include <variant>

#include "types.h"

namespace grafyte::collision {
    struct Shape {
        types::Vec2 pos;
    };

    struct AABB: Shape {
        float width, height;
    };

    struct Circle: Shape {
        float radius;
    };

    using Collider = std::variant<AABB, Circle>;
}
