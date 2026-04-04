#pragma once

#include <memory>

#include "types.h"
#include "GL-Core/Shader.h"
#include "GL-Core/Texture.h"

namespace grafyte {
    namespace types {
        struct  Material {
            ShaderHandle shader;
            TextureHandle texture;
            Color4 color;
            std::vector<ShaderUniform> staticUniforms;
        };

        struct MaterialAsset {
            std::string shaderSourcePath;

            bool hasTexture = false;
            std::string textureSourcePath;
            unsigned int textureSlot = 0;
        };
    }

    class MaterialManager {
    public:
        MaterialManager();

        void init();

        ~MaterialManager() = default;

        types::MaterialHandle createAsset(const types::MaterialAsset& asset, const types::ObjectId& id);
        types::MaterialAsset* asset(const types::MaterialHandle& h);
        types::Material* mat(const types::MaterialHandle& h);

        Texture* texture(const types::TextureHandle& h);
        Shader* shader(const types::ShaderHandle& h);

        void upload(const types::MaterialHandle& h);
        void clear();
    private:
        std::unordered_map<types::MaterialHandle, types::Material> m_mats;
        std::unordered_map<types::MaterialHandle, std::unique_ptr<types::MaterialAsset>> m_assets;
        std::unordered_map<types::TextureHandle, std::unique_ptr<Texture>> m_textures;
        std::unordered_map<std::string, types::TextureHandle> m_textureHandles;
        std::unordered_map<types::ShaderHandle, std::unique_ptr<Shader>> m_shaders;
        std::unordered_map<std::string, types::ShaderHandle> m_shaderHandles;

        uint32_t m_nextTextureId = 1;
        uint32_t m_nextShaderId = 1;

        const types::TextureHandle m_defaultTextureHandle = {0};
    };
}
