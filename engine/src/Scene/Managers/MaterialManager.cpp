#include "MaterialManager.h"

#include <format>
#include <stdexcept>

namespace grafyte
{
MaterialManager::MaterialManager()
{
    m_Textures.insert_or_assign(m_DefaultTextureHandle, std::make_unique<Texture>());
}

void MaterialManager::init()
{
    m_Textures[m_DefaultTextureHandle]->set("@embed/Textures/No");
}

types::MaterialHandle MaterialManager::createAsset(const types::MaterialAsset &asset, const types::ObjectId &id)
{

    const types::MaterialHandle h = {id};
    m_Assets[h] = std::make_unique<types::MaterialAsset>(asset);
    return h;
}

types::MaterialAsset *MaterialManager::asset(const types::MaterialHandle &h)
{
    const auto it = m_Assets.find(h);
    if (it == m_Assets.end())
        throw std::runtime_error("[Material Manager] Invalid MaterialHandle provided.");
    return it->second.get();
}

types::Material *MaterialManager::mat(const types::MaterialHandle &h)
{
    const auto it = m_Mats.find(h);
    if (it == m_Mats.end())
    {
        throw std::runtime_error(std::format("[Material Manager] Invalid MaterialHandle provided ({}).", h.id));
    }
    return &it->second;
}

Texture *MaterialManager::texture(const types::TextureHandle &h)
{
    const auto it = m_Textures.find(h);
    if (it == m_Textures.end())
    {
        throw std::runtime_error(std::format("[Material Manager] Invalid TextureHandle provided ({}).", h.id));
    }
    return it->second.get();
}

Shader *MaterialManager::shader(const types::ShaderHandle &h)
{
    const auto it = m_Shaders.find(h);
    if (it == m_Shaders.end())
    {
        throw std::runtime_error(std::format("[Material Manager] Invalid ShaderHandle provided ({}).", h.id));
    }
    return it->second.get();
}

void MaterialManager::upload(const types::MaterialHandle &h)
{
    types::MaterialAsset *a = asset(h);

    types::ShaderHandle sh;
    if (!m_ShaderHandles.contains(a->shaderSourcePath))
    {
        sh = {m_NextShaderId++};
        m_Shaders.insert_or_assign(sh, std::make_unique<Shader>(a->shaderSourcePath));
        m_ShaderHandles.insert_or_assign(a->shaderSourcePath, sh);
    }
    else
    {
        sh = m_ShaderHandles[a->shaderSourcePath];
    }

    types::TextureHandle th = m_DefaultTextureHandle;
    if (a->hasTexture)
    {
        if (m_TextureHandles.contains(a->textureSourcePath))
        {
            th = m_TextureHandles[a->textureSourcePath];
        }
        else
        {
            if (a->textureSourcePath.empty())
            {
                a->textureSourcePath = "@embed/Textures/No";
            }

            th = {m_NextTextureId++};
            m_Textures.insert_or_assign(th, std::make_unique<Texture>());
            m_Textures[th]->set(a->textureSourcePath);
            m_TextureHandles.insert_or_assign(a->textureSourcePath, th);
        }
    }

    m_Mats.insert_or_assign(h, types::Material{sh, th});
}

void MaterialManager::clear()
{
    m_Assets.clear();
    m_Mats.clear();
    m_Textures.clear();
    m_TextureHandles.clear();
    m_Shaders.clear();
    m_ShaderHandles.clear();

    m_NextTextureId = 1;
    m_NextShaderId = 1;

    m_Textures.insert_or_assign(m_DefaultTextureHandle, std::make_unique<Texture>());
    m_Textures[m_DefaultTextureHandle]->set("@embed/Textures/No");
}
} // namespace grafyte
