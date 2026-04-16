#pragma once

#include <cstdint>

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <string>
#include <variant>

namespace grafyte::types
{

using ObjectId = uint32_t;
using SceneId = uint32_t;
using UIId = uint32_t;

struct Float4
{
    float x, y, z, w;
};

using Color4 = Float4;

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;

    Vec2 operator+(const Vec2 &other) const
    {
        return {x + other.x, y + other.y};
    }
    Vec2 &operator+=(const Vec2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2 operator-(const Vec2 &other) const
    {
        return {x - other.x, y - other.y};
    }

    bool operator==(const Vec2 &other) const
    {
        return other.x == x && other.y == y;
    }

    Vec2 &operator-=(const Vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    };

    Vec2 operator*(const float &a) const
    {
        return {x * a, y * a};
    };

    Vec2 operator*(const Vec2 &other) const
    {
        return {x * other.x, y * other.y};
    };

    Vec2 operator/(const float &a) const
    {
        return {x / a, y / a};
    };

    Vec2 operator-() const
    {
        return {-x, -y};
    };
};

inline Vec2 operator*(const float &a, const Vec2 &v)
{
    return {v.x * a, v.y * a};
}

inline Vec2 abs(const Vec2 &v)
{
    return {std::abs(v.x), std::abs(v.y)};
}

struct Transform
{
    Vec2 pos{0.0f, 0.0f};
    float rot = 0.0f;
    Vec2 scale{1.0f, 1.0f};

    [[nodiscard]] glm::mat4 toMatrix() const
    {
        auto m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(pos.x, pos.y, 0.0f));
        m = glm::rotate(m, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::scale(m, glm::vec3(scale.x, scale.y, 1.0f));
        return m;
    }
};

struct Vertex
{
    Vec2 pos;
    Vec2 texPos;
};

struct MeshHandle
{
    ObjectId id = 0;

    bool operator==(const MeshHandle &other) const
    {
        return id == other.id;
    }
};

struct MaterialHandle
{
    ObjectId id = 0;

    bool operator==(const MaterialHandle &other) const
    {
        return id == other.id;
    }
};

struct TextureHandle
{
    ObjectId id = 0;

    bool operator==(const TextureHandle &other) const
    {
        return id == other.id;
    }
};

struct ShaderHandle
{
    ObjectId id = 0;

    bool operator==(const ShaderHandle &other) const
    {
        return id == other.id;
    }
};

using UniformValue = std::variant<int, float, Float4, glm::mat4>;

struct ShaderUniform
{
    std::string name;
    UniformValue value;
};

struct RenderComponent
{
    MeshHandle mesh;
    MaterialHandle mat;
    int zIndex = 0;
};

enum PrimitiveGeometry
{
    QUAD,
    TRIANGLE,
    CUSTOM
};

struct DrawItem
{
    ObjectId objectId = 0;
    MeshHandle mesh;
    PrimitiveGeometry geo = QUAD;
    MaterialHandle material;
    int zIndex = 0;
    Transform transform;
    Color4 color{};
};

struct TextData
{
    std::string text;
    Transform transform;
    Color4 color;
};

} // namespace grafyte::types

template <> struct std::hash<grafyte::types::MeshHandle>
{
    size_t operator()(const grafyte::types::MeshHandle &h) const noexcept
    {
        return std::hash<grafyte::types::ObjectId>()(h.id);
    }
};

namespace std
{

template <> struct hash<grafyte::types::MaterialHandle>
{
    size_t operator()(const grafyte::types::MaterialHandle &h) const noexcept
    {
        return hash<grafyte::types::ObjectId>()(h.id);
    }
};

template <> struct hash<grafyte::types::TextureHandle>
{
    size_t operator()(const grafyte::types::TextureHandle &h) const noexcept
    {
        return hash<grafyte::types::ObjectId>()(h.id);
    }
};

template <> struct hash<grafyte::types::ShaderHandle>
{
    size_t operator()(const grafyte::types::ShaderHandle &h) const noexcept
    {
        return std::hash<grafyte::types::ObjectId>()(h.id);
    }
};
} // namespace std
