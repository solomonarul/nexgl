#pragma once

#include "../../core/asset.hpp"
#include "shader.hpp"

#include <memory>
#include <string>

namespace NEX::GL
{
    struct TextureConfig
    {
        std::string path;
        GLint min_filter = GL_NEAREST;
        GLint mag_filter = GL_NEAREST;
        GLint wrap_s = GL_CLAMP_TO_EDGE;
        GLint wrap_t = GL_CLAMP_TO_EDGE;
        GLint gpu_format = GL_RGB;
        GLint format = GL_RGB;
    };

    struct Texture : public Core::IAsset
    {
        GLuint id;
        int w, h;
        void *data;

        static std::unique_ptr<GL::Shader> default_shader;

        void bind();
        void activate(GLenum);
        Texture(TextureConfig);
        ~Texture();
    };
}; // namespace NEX::GL
