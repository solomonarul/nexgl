#pragma once

#include "../gl/assets/texture.hpp"

namespace NEX::Prefabs
{
    struct VertexData
    {
        float x, y, u, v;
    };

    struct Sprite
    {
        GLuint vbo = 0;
        VertexData data[4];
        SDL_FColor color = {1, 1, 1, 1};

        Sprite(VertexData[4] = (VertexData*)GL::FULLSCREEN_RECT2D);
        ~Sprite();

        void draw(GL::Shader&, GLenum = GL_TRIANGLE_STRIP);

        // Call this after updating data to send to the GPU as well.
        void refresh(void);
    };
}; // namespace NEX::Prefabs