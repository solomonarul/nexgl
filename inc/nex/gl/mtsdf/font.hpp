#pragma once

#include "../../core/asset.hpp"
#include "../assets/shader.hpp"
#include "../assets/texture.hpp"
#include "../defines.hpp"

#include <map>
#include <memory>
#include <string>

namespace NEX::GL::MTSDF
{
    struct GlyphData
    {
        float advance;
        SDL_FRect quad;
        SDL_FRect atlas;
    };

    struct Font : public Core::IAsset
    {
        std::unique_ptr<Texture> texture;
        std::map<char, GlyphData> glyphs;

        static std::unique_ptr<GL::Shader> default_shader;

        Font(const std::string&, const std::string&);
    };
} // namespace NEX::GL::MTSDF
