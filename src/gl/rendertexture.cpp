#include "gl/rendertexture.hpp"

#include "core/defines.hpp"

using namespace NEX::GL;

RenderTexture::RenderTexture(int w, int h)
{
    this->w = w, this->h = h;

    glGenTextures(1, &this->id);
    this->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->w, this->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->id, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        SDL_Log("%s[EROR] Failed to create framebuffer object!%s\n", TTY_RED, TTY_RESET);

        glDeleteFramebuffers(1, &this->fbo), this->fbo = 0;
        glDeleteTextures(1, &this->id), this->id = 0;
        return;
    }
}

RenderTexture::~RenderTexture()
{
    if (this->fbo)
        glDeleteFramebuffers(1, &this->fbo), this->fbo = 0;
    if (this->id)
        glDeleteTextures(1, &this->id), this->id = 0;
}

void RenderTexture::use(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glViewport(0, 0, this->w, this->h);
}

void RenderTexture::unuse(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::activate(GLenum slot)
{
    glActiveTexture(slot);
    this->bind();
}

void RenderTexture::bind(void)
{
    glBindTexture(GL_TEXTURE_2D, this->id);
}
