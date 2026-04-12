#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "types.h"

namespace grafyte
{
namespace types
{
enum class AttribType : uint8_t
{
    Float,
    UInt,
    UByte
};

struct LayoutSlot
{
    AttribType type;
    uint32_t count;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<LayoutSlot> layoutSlots;
    std::vector<uint32_t> indices;
};

struct MeshAsset
{
    Vec2 scale;
    std::vector<LayoutSlot> layoutSlots;
    std::vector<uint32_t> indices;
    PrimitiveGeometry geo;
};
} // namespace types

class MeshManager
{
  public:
    MeshManager() = default;
    ~MeshManager() = default;

    types::MeshHandle createAsset(const types::MeshAsset &asset, const types::ObjectId &id);
    types::MeshAsset *asset(const types::MeshHandle &h);
    types::Mesh *mesh(const types::MeshHandle &h);

    void upload(const types::MeshHandle &h);
    void clear();

  private:
    std::unordered_map<types::MeshHandle, types::Mesh> m_Meshes;
    std::unordered_map<types::MeshHandle, std::unique_ptr<types::MeshAsset>> m_Assets;

    types::MeshHandle m_UnitQuad = {0};
    types::MeshHandle m_UnitTriangle = {0};
    types::ObjectId m_NextBuiltinId = 1;

    [[nodiscard]] types::MeshHandle getUnitQuad() const
    {
        return m_UnitQuad;
    };
    static std::vector<types::Vertex> makeUnitQuad();
    [[nodiscard]] types::MeshHandle getUnitTriangle() const
    {
        return m_UnitTriangle;
    };
    static std::vector<types::Vertex> makeUnitTriangle();
};
} // namespace grafyte
