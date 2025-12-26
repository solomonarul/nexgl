#pragma once

#include "texture.hpp"

#include <memory>
#include <vector>

namespace NEX::GL
{
    struct TextureArray : public Core::IAsset
    {
        std::vector<std::shared_ptr<Texture>> textures;
        TextureArray(std::vector<TextureConfig>);
    };
}; // namespace NEX::GL
