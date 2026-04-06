#include "CollisionSolver.h"

#include <cmath>

namespace grafyte {
    std::optional<types::Vec2> CollisionSolver::ComputePushBackTranslation(
        const collision::AABB& A,
        const collision::AABB& B,
        const types::Vec2& aPos,
        const types::Vec2& bPos)
    {
        const auto worldA = collision::AABB{A.pos + aPos, A.width, A.height};
        const auto worldB = collision::AABB{B.pos + bPos, B.width, B.height};

        const types::Vec2 delta = worldA.pos - worldB.pos;
        const float overlapX = (A.width + B.width) - std::abs(delta.x);
        const float overlapY = (A.height + B.height) - std::abs(delta.y);

        if (overlapX <= 0.0f || overlapY <= 0.0f) return std::nullopt;

        if (overlapX < overlapY) {
            const float sign = (delta.x > 0.0f) ? 1.0f : -1.0f;
            return types::Vec2{sign * overlapX, 0.0f};
        }

        const float sign = (delta.y > 0.0f) ? 1.0f : -1.0f;
        return types::Vec2{0.0f, sign * overlapY};
    }


    collision::Hit CollisionSolver::Intersects(const collision::AABB& a, const collision::AABB& b) {
        const float dx = a.pos.x - b.pos.x;
        const float dy = a.pos.y - b.pos.y;
        const float combinedWidth = a.width + b.width;
        const float combinedHeight = a.height + b.height;

        if (std::abs(dx) <= combinedWidth && std::abs(dy) <= combinedHeight) {
            const float overlapX = combinedWidth - std::abs(dx);
            const float overlapY = combinedHeight - std::abs(dy);

            collision::Direction dir;
            if (overlapX < overlapY) {
                dir = (dx > 0) ? collision::Right : collision::Left;
            } else {
                dir = (dy > 0) ? collision::Bottom : collision::Top;
            }

            return {a, b, true, dir};
        }

        return {a, b, false, collision::Top};
    }

    // bool CollisionSolver::Intersects(const collision::AABB &a, const collision::Circle &b) {
    //     const types::Vec2 bl = {a.pos.x - a.width, a.pos.y - a.height};
    //     const types::Vec2 tr = {a.pos.x + a.width, a.pos.y + a.height};
    //
    //     const types::Vec2 pt = {clampf(b.pos.x, bl.x, tr.x), clampf(b.pos.y, bl.y, tr.y)};
    //
    //     const float dx = pt.x - b.pos.x;
    //     const float dy = pt.y - b.pos.y;
    //     // Avoid using square root (as we don't need it here)
    //     return (dx * dx + dy * dy) <= (b.radius*b.radius);
    // }
    //
    // bool CollisionSolver::Intersects(const collision::Circle &a, const collision::Circle &b) {
    //     const float dx = b.pos.x - a.pos.x;
    //     const float dy = b.pos.y - a.pos.y;
    //     const float r = a.radius + b.radius;
    //
    //     return (dx * dx + dy * dy) <= (r * r);
    // }
    //
    // bool CollisionSolver::Intersects(const collision::Collider& a, const collision::Collider& b) const
    // {
    //     return std::visit([](auto const& lhs, auto const& rhs) -> bool {
    //         return Intersects(lhs, rhs);
    //     }, a, b);
    // }
}
