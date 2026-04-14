#pragma once

#include <doctest/doctest.h>

#include "types.h"

inline void expectVec2(const grafyte::types::Vec2 &actual, const grafyte::types::Vec2 &expected, float epsilon = 0.0001f)
{
    CHECK(actual.x == doctest::Approx(expected.x).epsilon(epsilon));
    CHECK(actual.y == doctest::Approx(expected.y).epsilon(epsilon));
}

inline void expectNear(float actual, float expected, float epsilon = 0.0001f)
{
    CHECK(actual == doctest::Approx(expected).epsilon(epsilon));
}
