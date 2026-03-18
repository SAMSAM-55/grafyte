#pragma once
#include <cstdint>
#include <string>

namespace grafyte::types {

    using ObjectId = uint32_t;

    struct Color4
    {
        float x, y, z, w;
    };

    struct Vec2 {
        float x = 0.0f;
        float y = 0.0f;

        Vec2 operator+(const Vec2& other) const {
            return {x + other.x, y + other.y};
        }
        Vec2& operator+=(const Vec2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vec2 operator-(const Vec2& other) const {
            return {x - other.x, y - other.y};
        };

        Vec2& operator-=(const Vec2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        };

        Vec2 operator*(const float& a) const {
            return {x * a, y * a};
        };

        Vec2 operator/(const float& a) const {
            return {x / a, y / a};
        };
    };

    inline Vec2 operator*(const float& a, const Vec2& v) {
        return {v.x * a, v.y * a};
    }

    struct Transform {
        Vec2 pos{0.0f, 0.0f};
        float rot = 0.0f;
        Vec2 scale{1.0f, 1.0f};
    };

    struct MeshHandle {
        ObjectId id = 0;

        bool operator==(const MeshHandle & other) const {
            return id == other.id;
        }
    };

    struct MaterialHandle {
        ObjectId id = 0;

        bool operator==(const MaterialHandle & other) const {
            return id == other.id;
        }
    };

    struct RenderComponent {
        MeshHandle mesh;
        MaterialHandle mat;
        int zIndex = 0;
    };

    struct DrawItem {
        ObjectId objectId = 0;
        MeshHandle mesh;
        MaterialHandle material;
        int zIndex = 0;
    };

    struct TextData
    {
        std::string text;
        Transform transform;
        Color4 color;
    };

}

template<>
struct std::hash<grafyte::types::MeshHandle> {
    size_t operator()(const grafyte::types::MeshHandle& h) const noexcept {
        return std::hash<grafyte::types::ObjectId>()(h.id);
    }
};

template<>
struct std::hash<grafyte::types::MaterialHandle> {
    size_t operator()(const grafyte::types::MaterialHandle& h) const noexcept {
        return std::hash<grafyte::types::ObjectId>()(h.id);
    }
};
