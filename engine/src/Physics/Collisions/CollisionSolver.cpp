#include "CollisionSolver.h"

#include "Scene/Scene.h"

namespace grafyte {
    float CollisionSolver::ComputePushBackT(const collision::AABB& A, const collision::AABB& B,
                                            const types::Vec2& aPos,  const types::Vec2& bPos,
                                            const types::Vec2& d) {
        const auto worldA = collision::AABB{A.pos + aPos, A.width, A.height};
        const auto worldB = collision::AABB{B.pos + bPos, B.width, B.height};

        const types::Vec2 hA = {A.width, A.height};
        const types::Vec2 hB = {B.width, B.height};

        const types::Vec2 p = worldA.pos - worldB.pos;
        const types::Vec2 h = hA + hB;

        if (std::abs(p.x) < h.x && std::abs(p.y) < h.y)
        {
            types::Vec2 t = {INFINITY, INFINITY};

            // For x
            if (d.x > 0) t.x = (h.x - p.x)/d.x;
            else if (d.x < 0) t.x = (- h.x - p.x)/d.x;

            // For y
            if (d.y > 0) t.y = (h.y - p.y)/d.y;
            else if (d.y < 0) t.y = (- h.y - p.y)/d.y;

            const float tmin = std::min(t.x, t.y);
            if (tmin < 0.0f) return -1.0f;

            return tmin + EPSILON;
        }

        return -1.0f;
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
