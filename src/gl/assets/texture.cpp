#include "gl/assets/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/defines.hpp"

using namespace NEX::GL;

std::unique_ptr<Shader> Texture::default_shader = nullptr;

Texture::Texture(TextureConfig cfg)
{
    if (!default_shader)
        default_shader = std::make_unique<Shader>("assets/shaders/texture_default.vert", "assets/shaders/texture_default.frag");

    int ch;
    this->data = stbi_load(cfg.path.c_str(), &this->w, &this->h, &ch, 0);
    if (!this->data)
    {
        SDL_Log("%s[EROR] Could not load texture (path: %s): %s%s\n", TTY_RED, cfg.path.c_str(), stbi_failure_reason(), TTY_RESET);
        return;
    }

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    // OpenGL assumes everything is aligned to 4 bytes for some reason, tell it to add padding if
    // not.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLenum format = GL_RGB;
    if (ch == 3)
        format = GL_RGB;
    else if (ch == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, this->w, this->h, 0, format, GL_UNSIGNED_BYTE, this->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, cfg.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, cfg.mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, cfg.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, cfg.wrap_t);

    stbi_image_free(this->data);
    this->data = nullptr;
}

void Texture::activate(GLenum slot)
{
    glActiveTexture(slot);
    this->bind();
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}

Texture::~Texture()
{
    if (this->id)
    {
        if (this->data)
            stbi_image_free(this->data);

        glDeleteTextures(1, &this->id);
    }
}
