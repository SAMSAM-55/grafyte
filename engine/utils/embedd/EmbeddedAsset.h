#pragma once
#include <cstdint>
#include <cstddef>

// Assets
#include "basic_shader.embed"
#include "texture_shader.embed"

namespace grafyte::embedded {
    struct EmbeddedAsset {
        std::uint8_t* data;
        std::size_t size;
    };

    constexpr EmbeddedAsset basicShader = {basicShader_embed, sizeof(basicShader_embed)};
    constexpr EmbeddedAsset textureShader = {textureShader_embed, sizeof(textureShader_embed)};
}
