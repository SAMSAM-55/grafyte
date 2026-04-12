#pragma once

#include <memory>

#include "GL-Core/Shader.h"
#include "GL-Core/Texture.h"
#include "types.h"

namespace grafyte
{
namespace types
{
struct Material
{
    ShaderHandle shader;
    TextureHandle texture;
    Color4 color;
    std::vector<ShaderUniform> staticUniforms;
};

struct MaterialAsset
{
    std::string shaderSourcePath;

    bool hasTexture = false;
    std::string textureSourcePath;
    unsigned int textureSlot = 0;
};
} // namespace types

class MaterialManager
{
  public:
    MaterialManager();

    void init();

    ~MaterialManager() = default;

    types::MaterialHandle createAsset(const types::MaterialAsset &asset, const types::ObjectId &id);
    types::MaterialAsset *asset(const types::MaterialHandle &h);
    types::Material *mat(const types::MaterialHandle &h);

    Texture *texture(const types::TextureHandle &h);
    Shader *shader(const types::ShaderHandle &h);

    void upload(const types::MaterialHandle &h);
    void clear();

  private:
    std::unordered_map<types::MaterialHandle, types::Material> m_Mats;
    std::unordered_map<types::MaterialHandle, std::unique_ptr<types::MaterialAsset>> m_Assets;
    std::unordered_map<types::TextureHandle, std::unique_ptr<Texture>> m_Textures;
    std::unordered_map<std::string, types::TextureHandle> m_TextureHandles;
    std::unordered_map<types::ShaderHandle, std::unique_ptr<Shader>> m_Shaders;
    std::unordered_map<std::string, types::ShaderHandle> m_ShaderHandles;

    uint32_t m_NextTextureId = 1;
    uint32_t m_NextShaderId = 1;

    const types::TextureHandle m_DefaultTextureHandle = {0};
};
} // namespace grafyte
