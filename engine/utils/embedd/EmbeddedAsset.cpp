#include "EmbeddedAsset.h"

#include "basic_shader.embed"
#include "texture_shader.embed"
#include "text_shader.embed"
#include "inter_font.embed"
#include "default_texture.embed"

namespace grafyte::embedded {
    constexpr std::size_t BasicShaderSize   = sizeof(basicShader_embed);
    constexpr std::size_t TextureShaderSize = sizeof(textureShader_embed);
    constexpr std::size_t TextShaderSize    = sizeof(textShader_embed);
    constexpr std::size_t InterFontSize     = sizeof(interFont_embed);
    constexpr std::size_t NoTextureSize     = sizeof(defaultTexture_embed);

    const EmbeddedAsset BasicShader   = { basicShader_embed,   BasicShaderSize };
    const EmbeddedAsset TextureShader = { textureShader_embed, TextureShaderSize };
    const EmbeddedAsset TextShader    = { textShader_embed,    TextShaderSize };
    const EmbeddedAsset BaseFont      = { interFont_embed,     InterFontSize };
    const EmbeddedAsset NoTexture     = {defaultTexture_embed, NoTextureSize};

}
