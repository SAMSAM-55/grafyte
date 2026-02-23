#include "Scene.h"
#include <iostream>

namespace grafyte {
    Scene::Scene(WorldContext *ctx)
        : m_ctx(ctx)
    {
    }

    std::shared_ptr<Object> Scene::spawnObject(const types::MeshAsset& mesh, const types::MaterialAsset& mat,
                                               const types::Vec2& pos, const int& zIndex) {
        // std::cout << "[Scene](SpawnObject): this = " << this << std::endl;

        const types::ObjectId id = allocateId();

        // std::cout << "[Scene](SpawnObject): Spawning object with ID: " << id
        //           << " at position: (" << pos.x << ", " << pos.y << ")"
        //           << " with zIndex: " << zIndex << std::endl;

        const auto meshH = m_ctx->meshes.createAsset(mesh, id);
        const auto matH = m_ctx->materials.createAsset(mat, id);
        m_ctx->meshes.upload(meshH);
        m_ctx->materials.upload(matH);

        const auto rc = types::RenderComponent{meshH, matH, zIndex};
        m_renderables.insert_or_assign(id, rc);
        m_objects.insert_or_assign(id, std::make_shared<Object>(id, this));
        m_transforms.insert_or_assign(id, types::Transform{
            .pos = pos,
            .rot = 0.0f,
            .scale = {1.0f, 1.0f}
        });

        const auto& t = m_transforms[id];
        // std::cout << "[Scene](SpawnObject): Object transform set: "
        //           << "pos=(" << t.pos.x << ", " << t.pos.y << "), "
        //           << "rot=" << t.rot << ", "
        //           << "scale=(" << t.scale.x << ", " << t.scale.y << ")" << std::endl;

        return m_objects[id];

    }

    void Scene::buildRenderList(std::vector<types::DrawItem> &out) const {
        // std::cout << "[Scene](BuildRenderList): this= " << this << std::endl;

        out.clear();
        out.reserve(m_renderables.size());

        // std::cout << "[Scene](BuildRenderList): Starting build. Renderables size: " << m_renderables.size() << std::endl;

        for (const auto&[id, rc]: m_renderables) {
            auto itT = m_transforms.find(id);

            if (itT == m_transforms.end()) {
                // std::cout << "[Scene](BuildRenderList): No transform for object ID: " << id << ". Skipping." << std::endl;
                continue;
            }

            // std::cout << "[Scene](BuildRenderList): Adding object ID: " << id << " with zIndex: " << rc.zIndex << std::endl;

            out.push_back(types::DrawItem{
                .objectId = id,
                .transform = itT->second,
                .mesh = rc.mesh,
                .material = rc.mat,
                .zIndex = rc.zIndex
            });
        }
        // std::cout << "[Scene](BuildRenderList): Build completed. Final list size: " << out.size() << std::endl;
    }

    void Scene::clear()
    {
        m_renderables.clear();
        m_transforms.clear();
        m_objects.clear();
    }
}
