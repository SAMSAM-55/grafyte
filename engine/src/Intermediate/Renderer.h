#pragma once

#include "Scene/Managers/MaterialManager.h"
#include "Scene/Managers/MeshManager.h"
#include "World/Camera.h"

namespace grafyte {
    class Renderer
    {
    public:
        Renderer(MeshManager& meshes, MaterialManager& materials);

        void Render(const std::vector<types::DrawItem>& items,
                    const std::unordered_map<types::ObjectId, types::Transform> &transforms,
                    const Camera &camera) const;
        static void Clear();

    private:
        void Draw(const types::DrawItem& item) const;
        static glm::mat4 computeModel(const types::Transform& t);
        MeshManager& m_meshes;
        MaterialManager& m_materials;
    };
}
