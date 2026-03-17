#pragma once

#include <memory>

#include "types.h"
#include "GL-Core/Shader.h"
#include "GL-Core/Texture.h"

namespace grafyte {
    namespace types {
        struct  Material {
            Shader shader;
            Texture texture;
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
        MaterialManager() = default;
        ~MaterialManager() = default;

        types::MaterialHandle createAsset(const types::MaterialAsset& asset, const types::ObjectId& id);
        types::MaterialAsset* asset(const types::MaterialHandle& h);
        types::Material* mat(const types::MaterialHandle& h);

        void upload(const types::MaterialHandle& h);
        void clear();
    private:
        std::unordered_map<types::MaterialHandle, types::Material> m_mats;
        std::unordered_map<types::MaterialHandle, std::unique_ptr<types::MaterialAsset>> m_assets;
    };
}
