#pragma once
#include <cstdint>
#include <cstddef>

namespace grafyte::embedded {

    struct EmbeddedAsset {
        const std::uint8_t* data;
        std::size_t size;
    };
    // Helpers “assets”
    extern const EmbeddedAsset BasicShader;
    extern const EmbeddedAsset TextureShader;
    extern const EmbeddedAsset TextShader;
    extern const EmbeddedAsset BaseFont;
    extern const EmbeddedAsset NoTexture;

} // namespace grafyte::embedded
