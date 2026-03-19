#include "MaterialManager.h"

#include <format>
#include <iostream>
#include <stdexcept>

namespace grafyte {
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
                "[Material Manager] Invalid MaterialHandle provided ({}).",
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
                "[Material Manager] Invalid MaterialHandle provided ({}).",
                h.id
                )
                );
        }
        return it->second.get();
    }

    void MaterialManager::upload(const types::MaterialHandle &h) {
        // std::cout << "[MaterialManager](Upload): Uploading material for ID: " << h.id << std::endl;
        types::MaterialAsset* a = asset(h);
        // std::cout << "[MaterialManager](Upload): Shader path: " << a->shaderSourcePath << std::endl;


        const types::ShaderHandle sh = {h.id};
        m_shaders.insert_or_assign(sh, std::make_unique<Shader>(a->shaderSourcePath));

        const types::TextureHandle th = {a->textureSourcePath.empty() ? -1 : h.id};
        if (a->hasTexture) {
            // std::cout << "[MaterialManager](Upload): Material has texture. Path: " << a->textureSourcePath << " Slot: " << a->textureSlot << std::endl;
            if (a->textureSourcePath.empty()) {
                a->textureSourcePath = "@embed/Textures/No";
            }

            m_textures.insert_or_assign(th, std::make_unique<Texture>());
            m_textures[th]->Set(a->textureSourcePath);
        }

        m_mats.insert_or_assign(h, types::Material{sh, th});
        // std::cout << "[MaterialManager](Upload): Material uploaded successfully for ID: " << h.id << std::endl;
    }

    void MaterialManager::clear()
    {
        m_assets.clear();
        m_mats.clear();
        m_textures.clear();
        m_shaders.clear();
    }
}
