#include "MaterialManager.h"

#include <format>
#include <iostream>
#include <stdexcept>

namespace grafyte {
    MaterialManager::MaterialManager() {
        std::cout << "[MaterialManager] ctor" << std::endl;
        m_textures.insert_or_assign(m_defaultTextureHandle, std::make_unique<Texture>());
        std::cout << "[MaterialManager] ctor OK !" << std::endl;
    }

    void MaterialManager::init() {
        m_textures[m_defaultTextureHandle]->Set("@embed/Textures/No");
    }

    types::MaterialHandle MaterialManager::createAsset(const types::MaterialAsset &asset, const types::ObjectId& id) {


        const types::MaterialHandle h = {id};
        m_assets[h] = std::make_unique<types::MaterialAsset>(asset);
        return h;
    }

    types::MaterialAsset * MaterialManager::asset(const types::MaterialHandle& h) {
        const auto it = m_assets.find(h);
        if (it == m_assets.end()) throw std::runtime_error("[Material Manager] Invalid MaterialHandle provided.");
        return it->second.get();
    }

    types::Material * MaterialManager::mat(const types::MaterialHandle &h) {
        const auto it = m_mats.find(h);
        if (it == m_mats.end())
        {
            throw std::runtime_error(
            std::format(
                "[Material Manager] Invalid MaterialHandle provided ({}).",
                h.id
                )
                );
        }
        return &it->second;
    }

    Texture * MaterialManager::texture(const types::TextureHandle &h) {
        const auto it = m_textures.find(h);
        if (it == m_textures.end())
        {
            throw std::runtime_error(
            std::format(
                "[Material Manager] Invalid TextureHandle provided ({}).",
                h.id
                )
                );
        }
        return it->second.get();
    }

    Shader * MaterialManager::shader(const types::ShaderHandle &h) {
        const auto it = m_shaders.find(h);
        if (it == m_shaders.end())
        {
            throw std::runtime_error(
            std::format(
                "[Material Manager] Invalid ShaderHandle provided ({}).",
                h.id
                )
                );
        }
        return it->second.get();
    }

    void MaterialManager::upload(const types::MaterialHandle &h) {
        types::MaterialAsset* a = asset(h);

        types::ShaderHandle sh;
        if (!m_shaderHandles.contains(a->shaderSourcePath))
        {
            sh = {m_nextShaderId++};
            m_shaders.insert_or_assign(sh, std::make_unique<Shader>(a->shaderSourcePath));
            m_shaderHandles.insert_or_assign(a->shaderSourcePath, sh);
        }
        else
        {
            sh = m_shaderHandles[a->shaderSourcePath];
        }

        types::TextureHandle th = m_defaultTextureHandle;
        if (a->hasTexture) {
            if (m_textureHandles.contains(a->textureSourcePath)) {
                th = m_textureHandles[a->textureSourcePath];
            }
            else {
                if (a->textureSourcePath.empty()) {
                    a->textureSourcePath = "@embed/Textures/No";
                }

                th = {m_nextTextureId++};
                m_textures.insert_or_assign(th, std::make_unique<Texture>());
                m_textures[th]->Set(a->textureSourcePath);
                m_textureHandles.insert_or_assign(a->textureSourcePath, th);
            }
        }

        m_mats.insert_or_assign(h, types::Material{sh, th});
    }

    void MaterialManager::clear()
    {
        m_assets.clear();
        m_mats.clear();
        m_textures.clear();
        m_textureHandles.clear();
        m_shaders.clear();
        m_shaderHandles.clear();

        m_nextTextureId = 1;
        m_nextShaderId = 1;

        m_textures.insert_or_assign(m_defaultTextureHandle, std::make_unique<Texture>());
        m_textures[m_defaultTextureHandle]->Set("@embed/Textures/No");
    }
}
