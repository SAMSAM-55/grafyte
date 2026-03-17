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
        struct  Mesh {
            VertexArray va;
            IndexBuffer ib;
            VertexBuffer vb;
            VertexBufferLayout layout;
        };

        enum class AttribType : uint8_t { Float, UInt, UByte };

        struct LayoutSlot {
            AttribType type;
            uint32_t count;
        };

        struct MeshAsset {
            std::vector<LayoutSlot> layoutSlots;

            std::vector<uint8_t> bytes;
            unsigned int sizeBytes = 0;
            unsigned int posOffsetBytes = 0;
            std::vector<uint32_t> indices;

            uint32_t vertexCount = 0;
        };
    }

    class MeshManager {
    public:
        MeshManager() = default;
        ~MeshManager() = default;

        types::MeshHandle createAsset(const types::MeshAsset& asset, const types::ObjectId& id);
        types::MeshAsset* asset(const types::MeshHandle& h);
        types::Mesh* mesh(const types::MeshHandle& h);

        void upload(const types::MeshHandle& h);
        void clear();
    private:
        std::unordered_map<types::MeshHandle, types::Mesh> m_meshes;
        std::unordered_map<types::MeshHandle, std::unique_ptr<types::MeshAsset>> m_assets;
    };
}
