#pragma once

#include "GL-Core/IndexBuffer.h"
#include "GL-Core/VertexArray.h"
#include "GL-Core/VertexBuffer.h"
#include "Scene/Managers/MaterialManager.h"
#include "Scene/Managers/MeshManager.h"
#include "World/Camera.h"

namespace grafyte
{
namespace types
{
struct BatchingKey
{
    TextureHandle th;
    ShaderHandle sh;
    int zIndex = 0;

    bool operator<(const BatchingKey &o) const
    {
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
} // namespace types

class Renderer
{
  public:
    Renderer(MeshManager &meshes, MaterialManager &materials);

    void render(const std::vector<types::BatchGroup> &groups,
                const std::unordered_map<types::ObjectId, types::Transform> &transforms,
                const std ::unordered_map<types::ObjectId, types::Color4> &colors, const Camera &camera);
    static void clearScreen();
    void clear();

  private:
    void draw(const types::BatchGroup &group, const std::unordered_map<types::ObjectId, types::Transform> &transforms,
              const std::unordered_map<types::ObjectId, types::Color4> &colors, const Camera &camera);
    static glm::mat4 computeModel(const types::Transform &t);
    static bool inCamera(const types::Vec2 &pos, const types::Vec2 &scale, const types::Vec2 &size,
                         const Camera &camera);

    MeshManager &m_Meshes;
    MaterialManager &m_Materials;

    std::vector<types::BatchedVertex> m_VertexScratch;
    std::vector<unsigned int> m_IndexScratch;
    VertexArray m_Va;
    VertexBuffer m_Vb;
    IndexBuffer m_Ib;
};
} // namespace grafyte
