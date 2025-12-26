#include "gl/rendertexture.hpp"

#include <iostream>

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

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << TTY_RED << "[EROR] Failed to create framebuffer object!\n" << TTY_RESET;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &this->fbo), this->fbo = 0;
        glDeleteTextures(1, &this->id), this->id = 0;
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void RenderTexture::unuse(int original_x, int original_y, int original_w, int original_h)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(original_x, original_y, original_w, original_h);
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
