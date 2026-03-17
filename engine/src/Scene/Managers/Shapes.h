#pragma once

#include <variant>

#include "types.h"

namespace grafyte::collision {
    struct Shape {
        types::Vec2 pos;

        Shape(): pos{0.0f, 0.0f} {}
        explicit Shape(const types::Vec2& p) :pos(p) {}
    };

    struct AABB: Shape {
        float width, height;

        AABB(): Shape(), width(0), height(0) {}
        explicit AABB(const types::Vec2& p, const float& w, const float& h) :Shape(p), width(w), height(h) {}
    };

    enum Direction
    {
        Top,
        Bottom,
        Right,
        Left
    };

    struct Hit
    {
        AABB A;
        AABB B;
        bool collision;
        Direction direction;

        explicit operator bool() const { return collision; }
    };

    // struct Circle: Shape {
    //     float radius;
    //
    //     Circle(): Shape(), radius(0) {}
    //     explicit Circle(const types::Vec2& p, const float& r): Shape(p), radius(r) {};
    // };

    // using Collider = std::variant<AABB, Circle>;
}
