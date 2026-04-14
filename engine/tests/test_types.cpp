#include "test_support.h"

#include "types.h"

TEST_CASE("Vec2 operators behave as expected")
{
    using grafyte::types::Vec2;

    constexpr Vec2 a{1.0f, 2.0f};
    constexpr Vec2 b{3.0f, -4.0f};

    expectVec2(a + b, {4.0f, -2.0f});
    expectVec2(a - b, {-2.0f, 6.0f});
    expectVec2(a * 2.0f, {2.0f, 4.0f});
    expectVec2(2.0f * a, {2.0f, 4.0f});
    expectVec2(a / 2.0f, {0.5f, 1.0f});
    expectVec2(-a, {-1.0f, -2.0f});
    expectVec2(a * b, {3.0f, -8.0f});
    expectVec2(std::abs(b), {3.0f, 8.0f});

    CHECK(a == Vec2{1.0f, 2.0f});
}

TEST_CASE("Transform converts to a model matrix")
{
    grafyte::types::Transform t;
    t.pos = {2.0f, 3.0f};
    t.rot = 0.0f;
    t.scale = {2.0f, 4.0f};

    const glm::vec4 result = t.toMatrix() * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    expectNear(result.x, 4.0f);
    expectNear(result.y, 7.0f);
    expectNear(result.z, 0.0f);
    expectNear(result.w, 1.0f);
}
