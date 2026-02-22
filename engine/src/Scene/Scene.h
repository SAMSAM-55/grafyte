#pragma once

#include <unordered_map>
#include <memory>

#include "types.h"
#include "../World/World.h"
#include "Intermediate/Object.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"

namespace grafyte {

    class Scene {
    public:
        explicit Scene(WorldContext* ctx);

        types::ObjectId allocateId() {return m_nextId++;};
        std::shared_ptr<Object> spawnObject(const types::MeshAsset& mesh, const types::MaterialAsset& mat,
                                            const types::Vec2& pos, const int& zIndex);

        types::Transform& transform(const types::ObjectId& id) {return m_transforms[id];};
        types::RenderComponent& renderable(const types::ObjectId& id) {return m_renderables[id];};

        [[nodiscard]] types::MaterialAsset *mat(const types::MaterialHandle &h) const {
            return m_ctx->materials.asset(h);
        };

        [[nodiscard]] MaterialManager& materials() {return m_ctx->materials;};
        [[nodiscard]] const MaterialManager& materials() const {return m_ctx->materials;};

        void setTransform(const types::ObjectId& id, const types::Transform& t) {m_transforms[id] = t;};
        void setRenderable(const types::ObjectId& id, const types::RenderComponent& rc) {m_renderables[id] = rc;};

        // void destroyObject(types::ObjectId id);

        void buildRenderList(std::vector<types::DrawItem>& out) const;
        void clear();

    private:
        types::ObjectId m_nextId = 1;

        std::unordered_map<types::ObjectId, std::shared_ptr<Object>> m_objects;

        std::unordered_map<types::ObjectId, types::Transform> m_transforms;
        std::unordered_map<types::ObjectId, types::RenderComponent> m_renderables;

        WorldContext* m_ctx;
    };

}
