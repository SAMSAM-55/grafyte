#pragma once
#include <cstdint>
#include <cstddef>

namespace grafyte::embedded {

    struct EmbeddedAsset {
        const std::uint8_t* data;
        std::size_t size;
    };
    // Helpers “assets”
    extern const EmbeddedAsset basicShader;
    extern const EmbeddedAsset textureShader;
    extern const EmbeddedAsset textShader;
    extern const EmbeddedAsset baseFont;

} // namespace grafyte::embedded
