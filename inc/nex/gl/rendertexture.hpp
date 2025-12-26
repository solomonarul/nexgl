#pragma once

#include "defines.hpp"

namespace NEX::GL
{
    struct RenderTexture
    {
        GLuint id, fbo;
        int w, h;

        RenderTexture(int, int);
        ~RenderTexture();

        void use(void);
        void unuse(int, int, int, int);
        void activate(GLenum);
        void bind(void);
    };
}; // namespace NEX::GL
