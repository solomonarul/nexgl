#include "gl/mtsdf/font.hpp"

using namespace NEX::GL;

#include "gl/assets/shader.hpp"

#include <sstream>

#ifdef ASSET_LOAD_LOG
#include <iostream>
#include "core/defines.hpp"
#endif

std::unique_ptr<Shader> MTSDF::Font::default_shader;

MTSDF::Font::Font(const std::string &path, const std::string &csv_path)
{
    if (!default_shader)
        default_shader = std::make_unique<Shader>("assets/shaders/mtsdf_default.vert", "assets/shaders/mtsdf_default.frag");

    this->texture = std::make_unique<Texture>(TextureConfig{
        .path = path,
        .min_filter = GL_LINEAR,
        .mag_filter = GL_LINEAR,
        .gpu_format = GL_RGBA,
        .format = GL_RGBA,
    });

    std::ifstream file(csv_path);
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string token;
        int column = 0;
        GlyphData g{};
        char c = 0;

        while (std::getline(ss, token, ','))
        {
            switch (column)
            {
                case 0:
                    c = std::stoi(token);
                    break;
                case 1:
                    g.advance = std::stof(token);
                    break;
                case 2:
                    g.quad.x = std::stof(token);
                    break;
                case 3:
                    g.quad.y = std::stof(token);
                    break;
                case 4:
                    g.quad.w = std::stof(token);
                    break;
                case 5:
                    g.quad.h = std::stof(token);
                    break;
                case 6:
                    g.atlas.x = std::stof(token) / this->texture->w;
                    break;
                case 7:
                    g.atlas.y = std::stof(token) / this->texture->h;
                    break;
                case 8:
                    g.atlas.w = std::stof(token) / this->texture->w;
                    break;
                case 9:
                    g.atlas.h = std::stof(token) / this->texture->h;
                    break;
            }
            column++;
        }

        glyphs[c] = g;
    }

    this->path = path;

#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Loaded MTSDF font (path: " << this->path << ") (csv: " << csv_path << ")\n" << TTY_RESET;
#endif
}

MTSDF::Font::~Font()
{
#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Destroyed MTSDF font (path: " << this->path << ")\n" << TTY_RESET;
#endif
}
