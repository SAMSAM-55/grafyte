#pragma once

#include "types.h"

namespace grafyte::collision
{
struct Shape
{
    types::Vec2 pos;

    Shape() : pos{0.0f, 0.0f}
    {
    }
    explicit Shape(const types::Vec2 &p) : pos(p)
    {
    }
};

struct AABB : Shape
{
    float width, height;

    AABB() : Shape(), width(0), height(0)
    {
    }
    AABB(const types::Vec2 &p, const float &w, const float &h) : Shape(p), width(w), height(h)
    {
    }
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
    bool collision = false;
    Direction direction = Top; // arbitrary default here to avoid uninitialized field

    explicit operator bool() const
    {
        return collision;
    }
};
} // namespace grafyte::collision
