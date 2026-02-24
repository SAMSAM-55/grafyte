#pragma once

#include "Camera.h"
#include "Intermediate/Renderer.h"
#include "../Scene/Managers/MaterialManager.h"
#include "../Scene/Managers/MeshManager.h"
#include "Scene/Managers/CollisionManager.h"

namespace grafyte {
    struct WorldContext {
        MeshManager meshes;
        MaterialManager materials;
        CollisionManager collisions;
        Renderer renderer;
        Camera camera;

        WorldContext()
            : renderer(meshes, materials)
        {}
    };

    class World {
    public:
        World() = default;
        ~World() = default;

        WorldContext& getCurrentContext() {return m_ctx;};
        [[nodiscard]] const WorldContext& getCurrentContext() const {return m_ctx;};
    private:
        WorldContext m_ctx;
    };
}
