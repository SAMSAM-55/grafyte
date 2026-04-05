#pragma once

#include "GL-Core/IndexBuffer.h"
#include "GL-Core/VertexArray.h"
#include "GL-Core/VertexBuffer.h"
#include "Scene/Managers/MaterialManager.h"
#include "Scene/Managers/MeshManager.h"
#include "World/Camera.h"

namespace grafyte {
    namespace types
    {
        struct BatchingKey
        {
            TextureHandle th;
            ShaderHandle sh;
            int zIndex = 0;

            bool operator<(const BatchingKey& o) const {
                return std::tie(zIndex, sh.id, th.id) < std::tie(o.zIndex, o.sh.id, o.th.id);
            }
        };

        struct BatchedVertex
        {
            Vec2 pos;
            Vec2 texPos;
            glm::vec4 color;
        };

        using BatchGroup = std::pair<BatchingKey, std::vector<DrawItem>>;
    }

    class Renderer
    {
    public:
        Renderer(MeshManager& meshes, MaterialManager& materials);

        void Render(
            const std::vector<types::BatchGroup> &groups, std::unordered_map<types::ObjectId, types::Transform> &transforms, std
            ::unordered_map<types::ObjectId, types::Color4> &colors, const Camera &camera);
        static void Clear();
        void clear();

    private:
        void Draw(const types::BatchGroup &group, const std::unordered_map<types::ObjectId, types::Transform> &transforms, const std::unordered_map<
                  types::ObjectId, types::Color4> &colors);
        static glm::mat4 computeModel(const types::Transform& t);
        MeshManager& m_meshes;
        MaterialManager& m_materials;

        std::vector<types::BatchedVertex> m_vertexScratch;
        std::vector<unsigned int> m_indexScratch;
        VertexArray m_va;
        VertexBuffer m_vb;
        IndexBuffer m_ib;
    };
}
