#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <vector>

#include "types.h"
#include "GL-Core/IndexBuffer.h"
#include "GL-Core/VertexArray.h"
#include "GL-Core/VertexBufferLayout.h"

namespace grafyte {
    namespace types {
        enum class AttribType : uint8_t { Float, UInt, UByte };

        struct LayoutSlot {
            AttribType type;
            uint32_t count;
        };

        struct  Mesh {
            std::vector<Vertex> vertices;
            std::vector<LayoutSlot> layoutSlots;
            std::vector<uint32_t> indices;
        };

        struct MeshAsset {
            Vec2 scale;
            std::vector<LayoutSlot> layoutSlots;
            std::vector<uint32_t> indices;
            PrimitiveGeometry geo;
        };
    }

    class MeshManager {
    public:
        MeshManager() {
            std::cout << "[MeshManager] ctor" << std::endl;
        }
        ~MeshManager() = default;

        types::MeshHandle createAsset(const types::MeshAsset& asset, const types::ObjectId& id);
        types::MeshAsset* asset(const types::MeshHandle& h);
        types::Mesh* mesh(const types::MeshHandle& h);

        void upload(const types::MeshHandle& h);
        void clear();
    private:
        std::unordered_map<types::MeshHandle, types::Mesh> m_meshes;
        std::unordered_map<types::MeshHandle, std::unique_ptr<types::MeshAsset>> m_assets;

        types::MeshHandle m_unitQuad = {0};
        types::MeshHandle m_unitTriangle = {0};
        types::ObjectId m_nextBuiltinId = 1;

        [[nodiscard]] types::MeshHandle getUnitQuad() const {return m_unitQuad;};
        static std::vector<types::Vertex> makeUnitQuad() ;
        [[nodiscard]] types::MeshHandle getUnitTriangle() const {return m_unitTriangle;};
        static std::vector<types::Vertex> makeUnitTriangle();
    };
}
