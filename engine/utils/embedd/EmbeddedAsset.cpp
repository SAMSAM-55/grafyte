#include "EmbeddedAsset.h"

#include "basic_shader.embed"
#include "texture_shader.embed"
#include "text_shader.embed"
#include "inter_font.embed"
#include "default_texture.embed"

namespace grafyte::embedded {
    constexpr std::size_t basicShader_size   = sizeof(basicShader_embed);
    constexpr std::size_t textureShader_size = sizeof(textureShader_embed);
    constexpr std::size_t textShader_size    = sizeof(textShader_embed);
    constexpr std::size_t interFont_size     = sizeof(interFont_embed);
    constexpr std::size_t noTexture_size     = sizeof(defaultTexture_embed);

    const EmbeddedAsset basicShader   = { basicShader_embed,   basicShader_size };
    const EmbeddedAsset textureShader = { textureShader_embed, textureShader_size };
    const EmbeddedAsset textShader    = { textShader_embed,    textShader_size };
    const EmbeddedAsset baseFont      = { interFont_embed,     interFont_size };
    const EmbeddedAsset noTexture     = {defaultTexture_embed, noTexture_size};

}
