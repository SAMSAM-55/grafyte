#pragma once

#include "../Scene/Managers/MaterialManager.h"
#include "../Scene/Managers/MeshManager.h"
#include "Camera.h"
#include "Intermediate/Renderer.h"
#include "Scene/Managers/CollisionManager.h"

namespace grafyte
{
struct WorldContext
{
    MeshManager meshes;
    MaterialManager materials;
    CollisionManager collisions;
    Renderer renderer;
    Camera camera;

    WorldContext() : renderer(meshes, materials)
    {
    }

    void init()
    {
        materials.init();
    }
};

class World
{
  public:
    World() = default;
    ~World() = default;

    WorldContext &getCurrentContext()
    {
        return m_Ctx;
    };
    [[nodiscard]] const WorldContext &getCurrentContext() const
    {
        return m_Ctx;
    };

  private:
    WorldContext m_Ctx;
};
} // namespace grafyte
