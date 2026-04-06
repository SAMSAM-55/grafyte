#pragma once

#include "Scene/Managers/Shapes.h"

namespace grafyte {
    class Scene;

    class CollisionSolver {
    public:
        static float ComputePushBackT(const collision::AABB& A, const collision::AABB&B,
                                      const types::Vec2& aPos,  const types::Vec2& bPos,
                                      const types::Vec2& d);

        static collision::Hit Intersects(const collision::AABB& a, const collision::AABB& b);
        // static bool Intersects(const collision::AABB& a, const collision::Circle& b);
        // static bool Intersects(const collision::Circle& a, const collision::AABB& b) {return Intersects(b, a);};
        // static bool Intersects(const collision::Circle& a, const collision::Circle& b);
        // [[nodiscard]] bool Intersects(const collision::Collider& a, const collision::Collider& b) const;

    private:

    };
}
