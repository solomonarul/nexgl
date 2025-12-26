#pragma once

#include "../assets/shader.hpp"
#include "../defines.hpp"
#include "font.hpp"

#include <memory>
#include <string>
#include <vector>

namespace NEX::GL::MTSDF
{
    struct Text
    {
        std::shared_ptr<Font> font;
        std::string text;
        float x = 9.0f, y = 9.0f, s = 1.0f;
        float o_x = 0.0f, o_y = 0.0f;
        float s_x = 1.0f, s_y = 1.0f;
        SDL_Color color = {255, 255, 255, 255};

        GLuint vbo = 0;
        std::vector<float> vertexData;

        Text(std::shared_ptr<Font>, const std::string &);
        ~Text();

        void draw(std::unique_ptr<GL::Shader> & = GL::MTSDF::Font::default_shader);
    };
} // namespace NEX::GL::MTSDF
